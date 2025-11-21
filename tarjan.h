#ifndef TARJAN_H
#define TARJAN_H

#include "utils.h"

//tructure pour un sommet dans l'algorithme de Tarjan
typedef struct {
    int id;
    int num;
    int num_acc;
    int ind_bool;
} t_tarjan_vertex;

// Structure pour une classe / composante fortement connexe
typedef struct {
    char name[10];
    t_tarjan_vertexsommets;
    int taille;
    int capacite;
} t_classe;

// Structure pour stocker toutes les classes /
typedef struct {
    t_classeclasses;
    int nb_classes;
    int capacite;
} t_stock_classe;

// Initialise le tableau de sommets pour l'algorithme de Tarjan 
t_tarjan_vertex initTarjanVertex(const liste_d_adjacence g);

// Applique l'algorithme de Tarjan au graphe 
t_stock_classe tarjan(const liste_d_adjacence g);

// Affiche la partition en classes
void printPartition(const t_stock_classep);

// Libère toute la mémoire associée aux classes /
void freePartition(t_stock_classep);

#endif
