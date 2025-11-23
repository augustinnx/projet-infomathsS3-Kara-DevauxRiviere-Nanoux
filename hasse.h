#ifndef __HASSE_H__
#define __HASSE_H__

#include <stddef.h>
#include "tarjan.h"
#include "utils.h"

// Représente un lien (arête) entre deux classes
typedef struct {
    int from;  // indice de classe 1-based
    int to;    // indice de classe 1-based
} t_link;

// Tableau dynamique de liens pour le diagramme de Hasse
typedef struct {
    t_link *links;
    int log_size;
    int capacity;
} t_link_array;

// Supprime les arêtes transitives dans le diagramme de Hasse
void removeTransitiveLinks(t_link_array *p_link_array);

// Libère la mémoire d'un tableau de liens
void freeLinkArray(t_link_array *arr);

// Crée un tableau sommet → indice de classe (0..nb_classes-1)
int* createVertexToClassMap(const t_stock_classe *p, int n);

// Construit les liens entre classes à partir du graphe et de la partition
t_link_array* buildClassLinks(const liste_d_adjacence *g, const t_stock_classe *p);

// Génère le diagramme de Hasse au format Mermaid
void generateHasseDiagram(const t_stock_classe *p,
                          const t_link_array *links,
                          const char *filepath);

// Analyse les propriétés du graphe à partir des classes et des liens
void analyzeGraphProperties(const t_stock_classe *p, const t_link_array *links);

#endif