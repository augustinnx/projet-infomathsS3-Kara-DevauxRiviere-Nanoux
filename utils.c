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
    FILE *file = fopen(filename, "rt"); 
    int nbvert, depart, arrivee;
    float proba;
    liste_d_adjacence G;

    if (file==NULL)
    {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", &nbvert) != 1)
    {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    G = createEmptyGraph(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
    {
        if (depart >= 1 && depart <= G.n) {
            addCell(&G.list[depart - 1], arrivee, proba);
        }
    }

    fclose(file);
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

// -Étape 3-

// Transformer les indicatif du sommets qui sont en chiffre en lettres
const char *getID(int i)
{
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; 
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

// Transformer la liste d'adjacence pour pour la visualiser sur Mermaidƒ

int visualMermaid(const liste_d_adjacence *g, const char *filepath)
{
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

        for (cell *cur = g->list[i].head; cur; cur = cur->next) {
            char dst[16];
            strcpy(dst, getID(cur->arriv));

            fprintf(f, "%s -->|%.2f| %s\n", src, cur->proba, dst);
        }
    }

    fclose(f);
    return 0;
}


// -Étape 4-