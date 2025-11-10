#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// -Étape 1-

// Création d'une cellule
cell *createCell(int arriv, float proba) {
    cell *newCell = (cell *)malloc(sizeof(cell));
    newCell->arriv = arriv;
    newCell->proba = proba;
    newCell->next  = NULL;
    return newCell;
}

// Création d'une liste vide
list createEmptyList(void) {
    list l;
    l.head = NULL;
    return l;
}

// Ajout d'une cellule 
void addCell(list *l, int arriv, float proba) {
    cell *newCell = createCell(arriv, proba);
    newCell->next = l->head;
    l->head = newCell;
}

// Affichage d'une liste
void printList(const list *l) {
    const cell *c = (l ? l->head : NULL);
    printf("[head @]");
    while (c) {
        printf(" -> (%d, %.2f)", c->arriv, c->proba);
        c = c->next;
    }
    printf("\n");
}

// Création d'une liste d'adjacence vide
liste_d_adjacence createEmptyGraph(int n) {
    liste_d_adjacence G;
    G.n = (n > 0 ? n : 0);
    G.list = NULL;
    if (G.n > 0) {
        G.list = (list *)malloc(sizeof(list) * G.n);
        for (int i = 0; i < G.n; ++i) {
            G.list[i] = createEmptyList();
        }
    }
    return G;
}

// Affichage de la liste d'adjacence
void printListe_d_adjacence(const liste_d_adjacence *G) {
    if (!G || !G->list || G->n <= 0) {
        printf("(graphe vide)\n");
        return;
    }
    for (int i = 0; i < G->n; ++i) {
        printf("Liste pour le sommet %d : ", i + 1);
        printList(&G->list[i]);
    }
}

// Lire la liste d'adjacence 
liste_d_adjacence readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, depart, arrivee;
    float proba;
    // declarer la variable pour la liste d’adjacence
    liste_d_adjacence G;

    if (file==NULL)
    {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }
    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // Initialiser une liste d’adjacence vide à partir du nombre de sommets
    G = createEmptyGraph(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
    {
        // on obtient, pour chaque ligne du fichier les valeurs
        // depart, arrivee, et proba
        // Ajouter l’arête qui va de ‘depart’ à ‘arrivée’ avec la probabilité ‘proba’ dans la liste d’adjacence
        if (depart >= 1 && depart <= G.n) {
            addCell(&G.list[depart - 1], arrivee, proba);
        }
    }

    fclose(file);
    // return la liste d’adjacence remplie;
    return G;
}

// -Étape 2-

// Vérification du graphe de Markov 
void checkMarkov(const liste_d_adjacence *G) {
    if (!G || !G->list || G->n <= 0) {
        printf("Erreur : graphe vide ou non initialisé.\n");
        return;
    }

    int estMarkov = 1;

    for (int i = 0; i < G->n; ++i) {
        float somme = 0.0f;
        for (cell *cur = G->list[i].head; cur; cur = cur->next) {
            somme += cur->proba;
        }
        if (somme < 0.99f || somme > 1.01f) {
            printf("Sommet %d : somme = %.2f \n", i + 1, somme);
            estMarkov = 0;
        }
    }

    if (estMarkov) {
        printf("\nLe graphe est un graphe de Markov.\n");
    } else {
        printf("\nLe graphe n'est pas un graphe de Markov.\n");
    }
}
