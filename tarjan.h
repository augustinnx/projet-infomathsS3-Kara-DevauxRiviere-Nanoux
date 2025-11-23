#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "utils.h"

/* Structure pour un sommet dans l'algorithme de Tarjan */
typedef struct {
    int id;        
    int num;       
    int num_acc;   
    int ind_bool; 
} t_tarjan_vertex;

/* Structure pour une classe / composante fortement connexe */
typedef struct {
    char name[10];        
    t_tarjan_vertex *sommets; 
    int taille;               
    int capacite;             
} t_classe;

/* Structure pour stocker toutes les classes (partition du graphe) */
typedef struct {
    t_classe *classes; 
    int nb_classes;    
    int capacite;     
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
