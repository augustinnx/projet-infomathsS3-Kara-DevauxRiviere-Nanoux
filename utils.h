#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

typedef struct cell {
    int arriv;
    float proba;
    struct cell *next;
} cell;

typedef struct {
    cell *head;
} list;

typedef struct {
    int n;
    list *list;
} liste_d_adjacence;

// Création d'une cellule de liste d'adjacence
cell *createCell(int arriv, float proba);

// Création d'une liste vide
list createEmptyList(void);

// Ajout d'une cellule en tête de liste
void addCell(list *l, int arriv, float proba);

// Affichage d'une liste chaînée
void printList(const list *l);

// Création d'une liste d'adjacence vide
liste_d_adjacence createEmptyGraph(int n);

// Affichage de la liste d'adjacence d'un graphe
void printListe_d_adjacence(const liste_d_adjacence *g);

// Lecture d'un graphe depuis un fichier texte
liste_d_adjacence readGraph(const char *filename);

// Vérification de la propriété de Markov d'un graphe
void checkMarkov(const liste_d_adjacence *G);

// Conversion d'un indice de sommet en identifiant alphabétique
const char *getID(int i);

// Génération d'un fichier Mermaid à partir du graphe
int visualMermaid(const liste_d_adjacence *g, const char *filepath);

#endif
