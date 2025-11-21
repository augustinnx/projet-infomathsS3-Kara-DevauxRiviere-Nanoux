#ifndef __TARJAN_H__
#define __TARJAN_H__

#include "utils.h"

// Structure pour un sommet 
typedef struct {
    int id;              
    int num;             
    int low;             
    int in_stack;        
} t_tarjan_vertex;

// Structure pour une classe/composante
typedef struct {
    char name[10];             
    t_tarjan_vertex *vertices; 
    int size;                  
    int capacity;              
} t_classe;

// Structure pour la partition
typedef struct {
    t_classe *classes;   
    int nb_classes;      
    int capacity;        
} t_partition;

// Fonctions principales
t_tarjan_vertex* initTarjanVertices(const liste_d_adjacence *g);
t_partition* tarjan(const liste_d_adjacence *g);
void printPartition(const t_partition *p);
void freePartition(t_partition *p);

#endif