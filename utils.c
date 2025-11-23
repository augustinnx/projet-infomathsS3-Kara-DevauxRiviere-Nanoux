#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Création d'une cellule de liste d'adjacence
cellule *createCell(int arriv, float proba) {
    cellule *newCell = (cellule *)malloc(sizeof(cellule));
    newCell->sommet_arrive = arriv;
    newCell->proba = proba;
    newCell->next  = NULL;
    return newCell;
}

// Création d'une liste vide
liste createEmptyList(void) {
    liste l;
    l.head = NULL;
    return l;
}

// Ajout d'une cellule en tête de liste
void addCell(liste *l, int sommet_arrive, float proba) {
    cellule *newCell = createCell(sommet_arrive, proba);
    newCell->next = l->head;
    l->head = newCell;
}

// Affichage d'une liste chaînée
void printList(const liste *l) {
    const cellule *c = (l ? l->head : NULL);
    printf("[head @]");
    while (c) {
        printf(" -> (%d, %.2f)", c->sommet_arrive, c->proba);
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
        G.list = (liste *)malloc(sizeof(liste) * G.n);
        for (int i = 0; i < G.n; ++i) {
            G.list[i] = createEmptyList();
        }
    }
    return G;
}

// Affichage de la liste d'adjacence d'un graphe
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

// Lecture d'un graphe depuis un fichier texte
liste_d_adjacence readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;
    liste_d_adjacence G;

    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    G = createEmptyGraph(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart >= 1 && depart <= G.n) {
            addCell(&G.list[depart - 1], arrivee, proba);
        }
    }

    fclose(file);
    return G;
}

// Fonction qui vérifie si un graphe est bien un graphe de Markov (étape 2 - P1)
void checkMarkov(const liste_d_adjacence *G) {
    if (!G || !G->list || G->n <= 0) {
        printf("Erreur : graphe vide ou non initialisé.\n");
        return;
    }

    int estMarkov = 1;

    for (int i = 0; i < G->n; ++i) {
        float somme = 0.0f;
        for (cellule *cur = G->list[i].head; cur; cur = cur->next) {
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

// Conversion d'un indice de sommet en identifiant alphabétique (ETAPE3 - P1)
const char *getID(int i) {
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--;
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }
    for (int j = 0; j < index; j++) {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

// Génération d'un fichier Mermaid à partir du graphe (ETAPE3 - P1)
int visualMermaid(const liste_d_adjacence *g, const char *filepath) {
    if (!g || !g->list || g->n <= 0 || !filepath) {
        fprintf(stderr, "visualMermaid: graphe ou chemin invalide\n");
        return -1;
    }

    FILE *f = fopen(filepath, "wt");
    fprintf(f,
        "---\n"
        "config:\n"
        "   layout: elk\n"
        "   theme: neo\n"
        "   look: neo\n"
        "---\n\n"
        "flowchart LR\n"
    );

    for (int i = 1; i <= g->n; ++i) {
        char id[16];
        strcpy(id, getID(i));
        fprintf(f, "%s((%d))\n", id, i);
    }
    fprintf(f, "\n");

    for (int i = 0; i < g->n; ++i) {
        char src[16];
        strcpy(src, getID(i + 1));

        for (cellule *cur = g->list[i].head; cur; cur = cur->next) {
            char dst[16];
            strcpy(dst, getID(cur->sommet_arrive));

            fprintf(f, "%s -->|%.2f| %s\n", src, cur->proba, dst);
        }
    }

    fclose(f);
    return 0;
}
