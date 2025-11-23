#ifndef __HASSE_H__
#define __HASSE_H__

#include <stddef.h>
#include "tarjan.h"
#include "utils.h"

// Structure représentant un lien entre deux classes
typedef struct {
    int from;
    int to;
} t_link;

// Tableau dynamique de liens pour le diagramme de Hasse
typedef struct {
    t_link *links;
    int log_size;
    int capacity;
} t_link_array;

// Supprime les arêtes transitives dans le diagramme de Hasse
void removeTransitiveLinks(t_link_array *p_link_array);

// Construit les liens à partir d'une matrice d'adjacence pondérée
int buildLinksFromWeightedMatrix(int n, double **mat, double threshold, t_link_array *out);

// Libère la mémoire d'un tableau de liens
void freeLinkArray(t_link_array *arr);

// Crée une table sommet → classe à partir de la partition
int* createVertexToClassMap(const t_partition *p, int n);

// Construit les liens entre classes à partir du graphe et de la partition
t_link_array* buildClassLinks(const liste_d_adjacence *g, const t_partition *p);

// Génère le diagramme de Hasse au format Mermaid
void generateHasseDiagram(const t_partition *p, const t_link_array *links, const char *filepath);

// Analyse les propriétés du graphe à partir des classes et des liens
void analyzeGraphProperties(const t_partition *p, const t_link_array *links);

#endif
