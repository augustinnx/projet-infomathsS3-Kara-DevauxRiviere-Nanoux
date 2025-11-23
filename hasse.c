#include <stdio.h>
#include <stdlib.h>
#include "hasse.h"

// Initialise un tableau dynamique de liens
static void linkArrayInit(t_link_array *arr, int cap) {
    if (cap <= 0) cap = 8;
    arr->links = (t_link *)malloc(sizeof(t_link) * cap);
    arr->log_size = 0;
    arr->capacity = cap;
}

// Ajoute un lien dans le tableau dynamique
static void linkArrayPush(t_link_array *arr, t_link e) {
    if (arr->log_size >= arr->capacity) {
        arr->capacity *= 2;
        t_link *tmp = (t_link *)realloc(arr->links, sizeof(t_link) * arr->capacity);
        if (!tmp) return;
        arr->links = tmp;
    }
    arr->links[arr->log_size++] = e;
}

// Libère la mémoire d'un tableau de liens
void freeLinkArray(t_link_array *arr) {
    if (!arr) return;
    free(arr->links);
    arr->links = NULL;
    arr->log_size = 0;
    arr->capacity = 0;
}

// Supprime les arêtes transitives dans le diagramme de Hasse
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
            p_link_array->links[i] =
                p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        } else {
            i++;
        }
    }
}

// Crée un tableau sommet → indice de classe à partir de la partition
int* createVertexToClassMap(const t_stock_classe *p, int n) {
    int *map = (int*)malloc(sizeof(int) * n);
    if (!map) return NULL;

    for (int i = 0; i < n; i++) {
        map[i] = -1;
    }

    for (int c = 0; c < p->nb_classes; c++) {
        for (int v = 0; v < p->classes[c].taille; v++) {
            int vertex_id = p->classes[c].sommets[v].id;  // 1..n
            if (vertex_id >= 1 && vertex_id <= n) {
                map[vertex_id - 1] = c;                    // 0..nb_classes-1
            }
        }
    }
    return map;
}

// Construit les liens entre classes à partir du graphe et de la partition
t_link_array* buildClassLinks(const liste_d_adjacence *g, const t_stock_classe *p) {
    t_link_array *links = (t_link_array*)malloc(sizeof(t_link_array));
    if (!links) return NULL;

    linkArrayInit(links, 10);

    int *vertex_to_class = createVertexToClassMap(p, g->n);
    if (!vertex_to_class) {
        free(links->links);
        free(links);
        return NULL;
    }

    for (int i = 0; i < g->n; i++) {
        int Ci = vertex_to_class[i];
        cell *cur = g->list[i].head;
        while (cur) {
            int j = cur->arriv - 1;
            int Cj = vertex_to_class[j];

            if (Ci != Cj) {
                int exists = 0;
                for (int k = 0; k < links->log_size; k++) {
                    if (links->links[k].from == Ci + 1 &&
                        links->links[k].to   == Cj + 1) {
                        exists = 1;
                        break;
                    }
                }

                if (!exists) {
                    t_link e;
                    e.from = Ci + 1;  // classes numérotées 1..nb_classes
                    e.to   = Cj + 1;
                    linkArrayPush(links, e);
                }
            }

            cur = cur->next;
        }
    }

    free(vertex_to_class);
    return links;
}

// Génère le diagramme de Hasse au format Mermaid
void generateHasseDiagram(const t_stock_classe *p,
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

    // Nœuds (classes)
    for (int i = 0; i < p->nb_classes; i++) {
        fprintf(f, "%s[\"%s: {", p->classes[i].name, p->classes[i].name);
        for (int j = 0; j < p->classes[i].taille; j++) {
            fprintf(f, "%d", p->classes[i].sommets[j].id);
            if (j < p->classes[i].taille - 1) fprintf(f, ",");
        }
        fprintf(f, "}\"]\n");
    }
    fprintf(f, "\n");

    // Liens entre classes
    for (int i = 0; i < links->log_size; i++) {
        int from_idx = links->links[i].from - 1;
        int to_idx   = links->links[i].to   - 1;
        if (from_idx >= 0 && from_idx < p->nb_classes &&
            to_idx   >= 0 && to_idx   < p->nb_classes) {
            fprintf(f, "%s --> %s\n",
                    p->classes[from_idx].name,
                    p->classes[to_idx].name);
        }
    }

    fclose(f);
    printf("Diagramme de Hasse généré: %s\n", filepath);
}

// Analyse les propriétés du graphe à partir des classes et des liens
void analyzeGraphProperties(const t_stock_classe *p, const t_link_array *links) {
    printf("\n=== Caractéristiques du graphe ===\n\n");

    int *has_outgoing = (int*)calloc(p->nb_classes, sizeof(int));
    if (!has_outgoing) return;

    for (int i = 0; i < links->log_size; i++) {
        int from_idx = links->links[i].from - 1;
        if (from_idx >= 0 && from_idx < p->nb_classes) {
            has_outgoing[from_idx] = 1;
        }
    }

    printf("Classes transitoires:\n");
    for (int i = 0; i < p->nb_classes; i++) {
        if (has_outgoing[i]) {
            printf("  - %s: {", p->classes[i].name);
            for (int j = 0; j < p->classes[i].taille; j++) {
                printf("%d", p->classes[i].sommets[j].id);
                if (j < p->classes[i].taille - 1) printf(",");
            }
            printf("} sont transitoires\n");
        }
    }

    printf("\nClasses persistantes:\n");
    int nb_absorbants = 0;
    for (int i = 0; i < p->nb_classes; i++) {
        if (!has_outgoing[i]) {
            printf("  - %s: {", p->classes[i].name);
            for (int j = 0; j < p->classes[i].taille; j++) {
                printf("%d", p->classes[i].sommets[j].id);
                if (j < p->classes[i].taille - 1) printf(",");
            }
            printf("} sont persistants\n");

            if (p->classes[i].taille == 1) {
                printf("    -> L'état %d est absorbant\n",
                       p->classes[i].sommets[0].id);
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