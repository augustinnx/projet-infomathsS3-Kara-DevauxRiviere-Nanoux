#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"

//Utilitaires internes pour t_link_array

static void linkArrayInit(t_link_array *arr, int cap) {
    arr->links = (t_link *)malloc(sizeof(t_link) * (cap > 0 ? cap : 8));
    arr->log_size = 0;
    arr->capacity = (cap > 0 ? cap : 8);
}
static void linkArrayPush(t_link_array *arr, t_link e) {
    if (arr->log_size >= arr->capacity) {
        arr->capacity = (arr->capacity * 2);
        arr->links = (t_link *)realloc(arr->links, sizeof(t_link) * arr->capacity);
    }
    arr->links[arr->log_size++] = e;
}

//Fonctions génériques Hasse 

void freeLinkArray(t_link_array *arr) {
    if (!arr) return;
    free(arr->links);
    arr->links = NULL;
    arr->log_size = 0;
    arr->capacity = 0;
}

int buildLinksFromWeightedMatrix(int n, double **mat, double threshold, t_link_array *out) {
    if (!mat || !out || n <= 0) return -1;
    linkArrayInit(out, n * 2);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (mat[i][j] >= threshold) {
                t_link e = { .from = i + 1, .to = j + 1 }; // stocké en 1-based
                linkArrayPush(out, e);
            }
        }
    }
    return 0;
}

/* Version robuste et in-place de la réduction transitive.
 * Idée : on supprime un arc a->c s'il existe a->b et b->c.
 */
void removeTransitiveLinks(t_link_array *p_link_array) {
    if (!p_link_array || p_link_array->log_size <= 1) return;

    int i = 0;
    while (i < p_link_array->log_size) {
        t_link link1 = p_link_array->links[i];
        int to_remove = 0;

        for (int j = 0; j < p_link_array->log_size && !to_remove; ++j) {
            if (j == i) continue;
            t_link link2 = p_link_array->links[j];
            if (link1.from == link2.from) {
                // chercher link3: link2.to -> link1.to
                for (int k = 0; k < p_link_array->log_size && !to_remove; ++k) {
                    if (k == j || k == i) continue;
                    t_link link3 = p_link_array->links[k];
                    if (link3.from == link2.to && link3.to == link1.to) {
                        to_remove = 1;
                    }
                }
            }
        }

        if (to_remove) {
            // supprime link1 en remplaçant par le dernier
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        } else {
            i++;
        }
    }
}

//Fonctions spécifiques partie 2 (ancien hasse_utils.c) 

// Crée un tableau indiquant la classe de chaque sommet (taille n, indices 0..n-1)
 // map[i] = indice de la classe (0..nb_classes-1) qui contient le sommet (i+1)
 
int* createVertexToClassMap(const t_partition *p, int n) {
    int *map = (int*)malloc(sizeof(int) * n);
    if (!map) return NULL;

    for (int i = 0; i < n; i++) {
        map[i] = -1;
    }

    for (int c = 0; c < p->nb_classes; c++) {
        for (int v = 0; v < p->classes[c].size; v++) {
            int vertex_id = p->classes[c].vertices[v].id;  // 1..n 
            map[vertex_id - 1] = c; // classe c
        }
    }
    return map;
}

/* Construit les liens entre classes à partir du graphe (liste_d_adjacence) et
 * de la partition (t_partition). Retourne un t_link_array* alloué dynamiquement.
 */
t_link_array* buildClassLinks(const liste_d_adjacence *g, const t_partition *p) {
    t_link_array *links = (t_link_array*)malloc(sizeof(t_link_array));
    if (!links) return NULL;

    links->log_size = 0;
    links->capacity = 10;
    links->links = (t_link*)malloc(sizeof(t_link) * links->capacity);
    if (!links->links) {
        free(links);
        return NULL;
    }

    int *vertex_to_class = createVertexToClassMap(p, g->n);
    if (!vertex_to_class) {
        free(links->links);
        free(links);
        return NULL;
    }

    // Pour chaque sommet i
    for (int i = 0; i < g->n; i++) {
        int Ci = vertex_to_class[i];

        // Pour chaque successeur j de i
        cell *cur = g->list[i].head;
        while (cur) {
            int j = cur->arriv - 1;  // 0-based
            int Cj = vertex_to_class[j];

            // Si classes différentes
            if (Ci != Cj) {
                // Vérifier si le lien existe déjà
                int exists = 0;
                for (int k = 0; k < links->log_size; k++) {
                    if (links->links[k].from == Ci + 1 &&
                        links->links[k].to   == Cj + 1) {
                        exists = 1;
                        break;
                    }
                }

                if (!exists) {
                    // Agrandir si nécessaire
                    if (links->log_size >= links->capacity) {
                        links->capacity *= 2;
                        links->links = (t_link*)realloc(
                            links->links,
                            sizeof(t_link) * links->capacity
                        );
                    }

                    links->links[links->log_size].from = Ci + 1;  // classes numérotées 1..nb_classes
                    links->links[links->log_size].to   = Cj + 1;
                    links->log_size++;
                }
            }

            cur = cur->next;
        }
    }

    free(vertex_to_class);
    return links;
}

/* Génère le diagramme de Hasse entre classes au format Mermaid. */
void generateHasseDiagram(const t_partition *p,
                          const t_link_array *links,
                          const char *filepath) {
    FILE *f = fopen(filepath, "wt");
    if (!f) {
        fprintf(stderr, "Erreur: impossible d'ouvrir %s\n", filepath);
        return;
    }

    fprintf(f,
        "---\n"
        "config:\n"
        "   layout: elk\n"
        "   theme: neo\n"
        "   look: neo\n"
        "---\n\n"
        "flowchart TD\n"
    );

    // Définir les nœuds (classes)
    for (int i = 0; i < p->nb_classes; i++) {
        fprintf(f, "%s[\"%s: {", p->classes[i].name, p->classes[i].name);
        for (int j = 0; j < p->classes[i].size; j++) {
            fprintf(f, "%d", p->classes[i].vertices[j].id);
            if (j < p->classes[i].size - 1) fprintf(f, ",");
        }
        fprintf(f, "}\"]\n");
    }
    fprintf(f, "\n");

    // Ajouter les liens (note: from/to stockés en 1-based => C1, C2, ...)
    for (int i = 0; i < links->log_size; i++) {
        fprintf(f, "C%d --> C%d\n",
                links->links[i].from, links->links[i].to);
    }

    fclose(f);
    printf("Diagramme de Hasse généré: %s\n", filepath);
}

/* Analyse les caractéristiques du graphe à partir des classes et des liens. */
void analyzeGraphProperties(const t_partition *p, const t_link_array *links) {
    printf("\n=== Caractéristiques du graphe ===\n\n");

    // Classes transitoires et persistantes
    int *has_outgoing = (int*)calloc(p->nb_classes, sizeof(int));
    if (!has_outgoing) return;

    for (int i = 0; i < links->log_size; i++) {
        // from est 1-based, on convertit en 0-based
        int from_idx = links->links[i].from - 1;
        if (from_idx >= 0 && from_idx < p->nb_classes) {
            has_outgoing[from_idx] = 1;
        }
    }

    printf("Classes transitoires:\n");
    for (int i = 0; i < p->nb_classes; i++) {
        if (has_outgoing[i]) {
            printf("  - %s: états {", p->classes[i].name);
            for (int j = 0; j < p->classes[i].size; j++) {
                printf("%d", p->classes[i].vertices[j].id);
                if (j < p->classes[i].size - 1) printf(",");
            }
            printf("} sont transitoires\n");
        }
    }

    printf("\nClasses persistantes:\n");
    int nb_absorbants = 0;
    for (int i = 0; i < p->nb_classes; i++) {
        if (!has_outgoing[i]) {
            printf("  - %s: états {", p->classes[i].name);
            for (int j = 0; j < p->classes[i].size; j++) {
                printf("%d", p->classes[i].vertices[j].id);
                if (j < p->classes[i].size - 1) printf(",");
            }
            printf("} sont persistants\n");

            if (p->classes[i].size == 1) {
                printf("    -> L'état %d est absorbant\n",
                       p->classes[i].vertices[0].id);
                nb_absorbants++;
            }
        }
    }

    printf("\nÉtats absorbants: %d\n", nb_absorbants);

    if (p->nb_classes == 1) {
        printf("\nLe graphe est irréductible (1 seule composante)\n");
    } else {
        printf("\nLe graphe n'est pas irréductible (%d composantes)\n",
               p->nb_classes);
    }

    free(has_outgoing);
}
