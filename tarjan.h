#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "utils.h"

// Structure pour un sommet dans Tarjan
typedef struct {
    int id;              // Numéro du sommet (1..n)
    int num;             // Numéro de visite
    int low;             // Numéro accessible
    int in_stack;        // Booléen (0 ou 1)
} t_tarjan_vertex;

// Structure pour une classe/composante
typedef struct {
    char name[10];             // "C1", "C2", etc.
    t_tarjan_vertex *vertices; // Tableau dynamique de sommets
    int size;                  // Nombre de sommets
    int capacity;              // Capacité du tableau
} t_classe;

// Structure pour la partition
typedef struct {
    t_classe *classes;   // Tableau dynamique de classes
    int nb_classes;      // Nombre de classes
    int capacity;        // Capacité du tableau
} t_partition;

// Fonctions principales
t_tarjan_vertex* initTarjanVertices(const liste_d_adjacence *g);
t_partition* tarjan(const liste_d_adjacence *g);
void printPartition(const t_partition *p);
void freePartition(t_partition *p);

#endif