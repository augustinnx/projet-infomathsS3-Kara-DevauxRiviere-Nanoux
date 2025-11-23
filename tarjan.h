#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "utils.h"

// PARTIE 2

// Question 1.1 
typedef struct {
    int id;
    int num;
    int num_acc;
    int ind_bool;
} t_tarjan_vertex;

// Question 1.2/2.1: créer un tableau stockant tous les t_tarjan_vertex pour un graphe
t_tarjan_vertex* initTarjanVertices(const liste_d_adjacence *g);

// question 1.3
typedef struct {
    char name[10];
    t_tarjan_vertex *sommets;
    int taille;
    int capacite;
} t_classe;

// Question 1.4
typedef struct {
    t_classe *classes;
    int nb_classes;
    int capacite;
} t_stock_classe;



// Applique l'algorithme de Tarjan au graphe et renvoie les classes
t_stock_classe* tarjan(const liste_d_adjacence *g);

// Affiche les classes obtenues par Tarjan
void printPartition(const t_stock_classe *p);

// Libère toute la mémoire associée aux classes
void freePartition(t_stock_classe *p);

#endif
