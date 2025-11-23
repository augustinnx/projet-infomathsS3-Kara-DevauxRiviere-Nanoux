#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

// ETAPE 1 - P1
// Structure1
typedef struct cellule { 
    int sommet_arrive;
    float proba;
    struct cellule *next;
} cellule;

// Structure2
typedef struct {
    cellule *head;
} liste;

// Structure3
typedef struct {
    int n;
    liste *list;
} liste_d_adjacence;

// Fonction pour créer une "cellule" (fct1)
cellule *createCell(int sommet_arrive, float proba);

// Fonction pour créer une "liste" vide (fct2)
liste createEmptyList(void);

// Fonction pour ajout une cellule à une liste (fct3)
void addCell(liste *l, int sommet_arrive, float proba);

// Fonction pour afficher une liste (fct4)
void printList(const liste *l);

// Fonction pour créer une liste d’adjacence ‘vide’ à partir d’une taille donnée – on crée le tableau de listes, chacune étant initialisée avec la liste vide (fct5)
liste_d_adjacence createEmptyGraph(int n);

// Fonction pour afficher une liste d’adjacence (fct6)
void printListe_d_adjacence(const liste_d_adjacence *g);

// Lecture du fichier et construction de la liste d'adjacence. (validation)
liste_d_adjacence readGraph(const char *filename);



// ETAPE 2 - P1
// Déclaration de la fonction qui vérifie si un graphe est bien un graphe de Markov 
void checkMarkov(const liste_d_adjacence *G);



//ETAPE 3 - P1
// Conversion d'un indice de sommet en identifiant alphabétique
const char *getID(int i);

// Génération d'un fichier Mermaid à partir du graphe
int visualMermaid(const liste_d_adjacence *g, const char *filepath);

#endif
