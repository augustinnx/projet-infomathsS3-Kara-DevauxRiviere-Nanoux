#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

// Structure pour la cellule
typedef struct cell {
    int arriv;            
    float proba;          
    struct cell *next;    
} cell;

// Structure pour la liste 
typedef struct {
    cell *head;           
} list;

// Structure pour la liste d'adjacence
typedef struct {
    int n;               
    list *list;           
} liste_d_adjacence;


// Étape 1
cell *createCell(int arriv, float proba);
list createEmptyList(void);
void addCell(list *l, int arriv, float proba);
void printList(const list *l);
liste_d_adjacence createEmptyGraph(int n);
void printListe_d_adjacence(const liste_d_adjacence *g);
liste_d_adjacence readGraph(const char *filename);

// Étape 2
void checkMarkov(const liste_d_adjacence *G);

// Étape 3
const char *getID(int i);
int visualMermaid(const liste_d_adjacence *g, const char *filepath);

#endif
