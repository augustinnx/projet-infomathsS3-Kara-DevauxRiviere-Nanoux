#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "utils.h"

/* Structure pour un sommet dans l'algorithme de Tarjan */
typedef struct {
    int id;        // Numéro du sommet (1..n)
    int num;       // Numéro de visite
    int num_acc;   // Numéro accessible (ancien low)
    int ind_bool;  // Indicateur : dans la pile ? (0 ou 1)
} t_tarjan_vertex;

/* Structure pour une classe / composante fortement connexe */
typedef struct {
    char name[10];        // "C1", "C2", ...
    t_tarjan_vertex *sommets; // Tableau dynamique de sommets
    int taille;               // Nombre de sommets
    int capacite;             // Capacité allouée
} t_classe;

/* Structure pour stocker toutes les classes (partition du graphe) */
typedef struct {
    t_classe *classes; // Tableau dynamique de classes
    int nb_classes;    // Nombre total de classes
    int capacite;      // Capacité allouée
} t_stock_classe;

/* Initialise le tableau de sommets pour l'algorithme de Tarjan */
t_tarjan_vertex* initTarjanVertices(const liste_d_adjacence *g);

/* Applique l'algorithme de Tarjan au graphe et renvoie les classes */
t_stock_classe* tarjan(const liste_d_adjacence *g);

/* Affiche les classes obtenues par Tarjan */
void printPartition(const t_stock_classe *p);

/* Libère toute la mémoire associée aux classes */
void freePartition(t_stock_classe *p);

#endif
