#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"

//PARTIE 2

//ETAPE 2 :

// Question 2.2 : pile
// Structure de pile utilisée par l'algorithme de Tarjan
typedef struct {
    int *data;
    int top;
    int capacite;
} Stack;

// Crée une pile de taille donnée
static Stack* createStack(int capacite) {
    Stack *pile = (Stack*)malloc(sizeof(Stack));
    if (!pile) return NULL;

    pile->data = (int*)malloc(sizeof(int) * capacite);
    if (!pile->data) {
        free(pile);
        return NULL;
    }

    pile->top = -1;
    pile->capacite = capacite;
    return pile;
}

// Empile un indice de sommet
static void push(Stack *pile, int val) {
    if (!pile) return;
    if (pile->top < pile->capacite - 1) {
        pile->data[++pile->top] = val;
    }
}

// Dépile un indice de sommet
static int pop(Stack *pile) {
    if (!pile) return -1;
    if (pile->top >= 0) {
        return pile->data[pile->top--];
    }
    return -1;
}

// Libère la mémoire de la pile
static void freeStack(Stack *pile) {
    if (!pile) return;
    free(pile->data);
    free(pile);
}
// (fin question 2.2)

// Question 1.2/2.1: créer un tableau stockant tous les t_tarjan_vertex pour un graphe
t_tarjan_vertex* initTarjanVertices(const liste_d_adjacence *g) {
    if (!g || g->n <= 0) return NULL;

    t_tarjan_vertex *tab = (t_tarjan_vertex*)malloc(sizeof(t_tarjan_vertex) * g->n);
    if (!tab) return NULL;

    for (int i = 0; i < g->n; i++) { // question 2.1 > initalisation
        tab[i].id       = i + 1;
        tab[i].num      = -1;
        tab[i].num_acc  = -1;
        tab[i].ind_bool = 0;
    }
    return tab;
}


// ETAPE 3

// Question 3.1
static void parcours(int v, const liste_d_adjacence *g, t_tarjan_vertex *sommets, Stack *pile, int *compteur,t_stock_classe *stock)
{
    sommets[v].num = *compteur;
    sommets[v].num_acc = *compteur;
    (*compteur)++;

    push(pile, v);
    sommets[v].ind_bool = 1;

    cellule *cur = g->list[v].head;
    while (cur) {
        int w = cur->sommet_arrive - 1;

        if (sommets[w].num == -1) {
            parcours(w, g, sommets, pile, compteur, stock);

            if (sommets[w].num_acc < sommets[v].num_acc) {
                sommets[v].num_acc = sommets[w].num_acc;
            }
        } else if (sommets[w].ind_bool) {
            if (sommets[w].num < sommets[v].num_acc) {
                sommets[v].num_acc = sommets[w].num;
            }
        }

        cur = cur->next;
    }

    if (sommets[v].num_acc == sommets[v].num) {
        t_classe nouvelle;

        sprintf(nouvelle.name, "C%d", stock->nb_classes + 1);

        nouvelle.taille   = 0;
        nouvelle.capacite = 4;
        nouvelle.sommets  = (t_tarjan_vertex*)malloc(sizeof(t_tarjan_vertex) * nouvelle.capacite);
        if (!nouvelle.sommets) {
            return;
        }

        int w;
        do {
            w = pop(pile);
            sommets[w].ind_bool = 0;

            if (nouvelle.taille >= nouvelle.capacite) {
                nouvelle.capacite *= 2;
                nouvelle.sommets = (t_tarjan_vertex*)realloc(
                    nouvelle.sommets,
                    sizeof(t_tarjan_vertex) * nouvelle.capacite
                );
                if (!nouvelle.sommets) {
                    return;
                }
            }

            nouvelle.sommets[nouvelle.taille++] = sommets[w];

        } while (w != v);

        if (stock->nb_classes >= stock->capacite) {
            stock->capacite *= 2;
            stock->classes = (t_classe*)realloc(
                stock->classes,
                sizeof(t_classe) * stock->capacite
            );
            if (!stock->classes) {
                return;
            }
        }

        stock->classes[stock->nb_classes++] = nouvelle;
    }
}

// Question 3.2
t_stock_classe* tarjan(const liste_d_adjacence *g) {
    if (!g || g->n <= 0) return NULL;

    t_stock_classe *stock = (t_stock_classe*)malloc(sizeof(t_stock_classe));
    if (!stock) return NULL;

    stock->nb_classes = 0;
    stock->capacite   = 4;
    stock->classes    = (t_classe*)malloc(sizeof(t_classe) * stock->capacite);
    if (!stock->classes) {
        free(stock);
        return NULL;
    }

    t_tarjan_vertex *sommets = initTarjanVertices(g);
    if (!sommets) {
        free(stock->classes);
        free(stock);
        return NULL;
    }

    Stack *pile = createStack(g->n);
    if (!pile) {
        free(sommets);
        free(stock->classes);
        free(stock);
        return NULL;
    }

    int compteur = 0;

    for (int i = 0; i < g->n; i++) {
        if (sommets[i].num == -1) {
            parcours(i, g, sommets, pile, &compteur, stock);
        }
    }

    freeStack(pile);
    free(sommets);

    return stock;
}

// Affichage classes obtenues par Tarjan
void printPartition(const t_stock_classe *p) {
    if (!p) return;

    printf("\n - Partition en composantes fortement connexes - \n");
    for (int i = 0; i < p->nb_classes; i++) {
        printf("%s : {", p->classes[i].name);
        for (int j = 0; j < p->classes[i].taille; j++) {
            printf("%d", p->classes[i].sommets[j].id);
            if (j < p->classes[i].taille - 1) {
                printf(",");
            }
        }
        printf("}\n");
    }
}

// Libère mémoire associée aux classes
void freePartition(t_stock_classe *p) {
    if (!p) return;

    for (int i = 0; i < p->nb_classes; i++) {
        free(p->classes[i].sommets);
    }
    free(p->classes);
    free(p);
}
