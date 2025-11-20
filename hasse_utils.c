#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse_utils.h"
#include "hasse.h"

// Créer un tableau indiquant la classe de chaque sommet
int* createVertexToClassMap(const t_partition *p, int n) {
    int *map = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        map[i] = -1;
    }
    
    for (int c = 0; c < p->nb_classes; c++) {
        for (int v = 0; v < p->classes[c].size; v++) {
            int vertex_id = p->classes[c].vertices[v].id;
            map[vertex_id - 1] = c; // classe c
        }
    }
    
    return map;
}

// Construire les liens entre classes
t_link_array* buildClassLinks(const liste_d_adjacence *g, const t_partition *p) {
    t_link_array *links = (t_link_array*)malloc(sizeof(t_link_array));
    links->log_size = 0;
    links->capacity = 10;
    links->links = (t_link*)malloc(sizeof(t_link) * links->capacity);
    
    int *vertex_to_class = createVertexToClassMap(p, g->n);
    
    // Pour chaque sommet i
    for (int i = 0; i < g->n; i++) {
        int Ci = vertex_to_class[i];
        
        // Pour chaque successeur j de i
        cell *cur = g->list[i].head;
        while (cur) {
            int j = cur->arriv - 1;
            int Cj = vertex_to_class[j];
            
            // Si classes différentes
            if (Ci != Cj) {
                // Vérifier si le lien existe déjà
                int exists = 0;
                for (int k = 0; k < links->log_size; k++) {
                    if (links->links[k].from == Ci + 1 && links->links[k].to == Cj + 1) {
                        exists = 1;
                        break;
                    }
                }
                
                if (!exists) {
                    // Agrandir si nécessaire
                    if (links->log_size >= links->capacity) {
                        links->capacity *= 2;
                        links->links = (t_link*)realloc(links->links, 
                                        sizeof(t_link) * links->capacity);
                    }
                    
                    links->links[links->log_size].from = Ci + 1;
                    links->links[links->log_size].to = Cj + 1;
                    links->log_size++;
                }
            }
            
            cur = cur->next;
        }
    }
    
    free(vertex_to_class);
    return links;
}

// Générer le diagramme de Hasse en Mermaid
void generateHasseDiagram(const t_partition *p, const t_link_array *links, const char *filepath) {
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
    
    // Ajouter les liens
    for (int i = 0; i < links->log_size; i++) {
        fprintf(f, "C%d --> C%d\n", links->from, links->to);
    }
    
    fclose(f);
    printf("Diagramme de Hasse généré: %s\n", filepath);
}

// Analyser les caractéristiques du graphe
void analyzeGraphProperties(const t_partition *p, const t_link_array *links) {
    printf("\n=== Caractéristiques du graphe ===\n\n");
    
    // Classes transitoires et persistantes
    int *has_outgoing = (int*)calloc(p->nb_classes, sizeof(int));
    
    for (int i = 0; i < links->log_size; i++) {
        has_outgoing[links->from - 1] = 1;
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
                printf("    -> L'état %d est absorbant\n", p->classes[i].vertices[0].id);
                nb_absorbants++;
            }
        }
    }
    
    printf("\nÉtats absorbants: %d\n", nb_absorbants);
    
    if (p->nb_classes == 1) {
        printf("\nLe graphe est irréductible (1 seule composante)\n");
    } else {
        printf("\nLe graphe n'est pas irréductible (%d composantes)\n", p->nb_classes);
    }
    
    free(has_outgoing);
}
