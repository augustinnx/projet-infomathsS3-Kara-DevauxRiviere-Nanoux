#ifndef __HASSE_H__
#define __HASSE_H__

#include <stddef.h>
#include "tarjan.h"
#include "utils.h"

// PARTIE 2 - ETAPE 1 - HASSE : Créez une structure ‘lien’ qui stockera 2 entiers : un entier pour la classe de « départ », un entier pour la classe d’ « arrivée ».
typedef struct {
    int from; 
    int to;    
} t_link;

// PARTIE 2 - ETAPE 1 - HASSE :Créez une structure pour stocker plusieurs ‘liens’
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