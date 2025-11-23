#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

// Création d'une matrice n x n remplie de 0
t_matrix *createZeroMatrix(int n) {
    if (n <= 0) return NULL;

    t_matrix *m = (t_matrix *)malloc(sizeof(t_matrix));
    if (!m) return NULL;

    m->n = n;
    m->data = (double **)malloc(sizeof(double *) * n);
    if (!m->data) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < n; ++i) {
        m->data[i] = (double *)malloc(sizeof(double) * n);
        if (!m->data[i]) {
            for (int k = 0; k < i; ++k) {
                free(m->data[k]);
            }
            free(m->data);
            free(m);
            return NULL;
        }
        for (int j = 0; j < n; ++j) {
            m->data[i][j] = 0.0;
        }
    }

    return m;
}

// Libération de la mémoire d'une matrice
void freeMatrix(t_matrix *m) {
    if (!m) return;
    if (m->data) {
        for (int i = 0; i < m->n; ++i) {
            free(m->data[i]);
        }
        free(m->data);
    }
    free(m);
}

// Affichage d'une matrice carrée
void printMatrix(const t_matrix *m) {
    if (!m) {
        printf("(matrice NULL)\n");
        return;
    }
    printf("Matrice %d x %d :\n", m->n, m->n);
    for (int i = 0; i < m->n; ++i) {
        for (int j = 0; j < m->n; ++j) {
            printf("%7.4f ", m->data[i][j]);
        }
        printf("\n");
    }
}

// Construction de la matrice de transition M à partir du graphe
t_matrix *graphToTransitionMatrix(const liste_d_adjacence *g) {
    if (!g || g->n <= 0 || !g->list) return NULL;

    int n = g->n;
    t_matrix *M = createZeroMatrix(n);
    if (!M) return NULL;

    for (int i = 0; i < n; ++i) {
        cell *cur = g->list[i].head;
        while (cur) {
            int j = cur->arriv - 1;
            if (j >= 0 && j < n) {
                M->data[i][j] = (double)cur->proba;
            }
            cur = cur->next;
        }
    }

    return M;
}

// Copie du contenu d'une matrice source vers une matrice destination
void copyMatrix(const t_matrix *src, t_matrix *dst) {
    if (!src || !dst) return;
    if (src->n != dst->n) return;

    int n = src->n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dst->data[i][j] = src->data[i][j];
        }
    }
}

// Multiplication de deux matrices carrées
t_matrix *multiplyMatrix(const t_matrix *A, const t_matrix *B) {
    if (!A || !B) return NULL;
    if (A->n != B->n) return NULL;

    int n = A->n;
    t_matrix *C = createZeroMatrix(n);
    if (!C) return NULL;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += A->data[i][k] * B->data[k][j];
            }
            C->data[i][j] = sum;
        }
    }

    return C;
}

// Calcul de la somme des différences absolues entre deux matrices
double diffMatrix(const t_matrix *A, const t_matrix *B) {
    if (!A || !B) return -1.0;
    if (A->n != B->n) return -1.0;

    int n = A->n;
    double sum = 0.0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            sum += fabs(A->data[i][j] - B->data[i][j]);
        }
    }

    return sum;
}

// Extraction de la sous-matrice associée à une composante fortement connexe
t_matrix *subMatrix(const t_matrix *M, const t_stock_classe *part, int compo_index) {
    if (!M || !part) return NULL;
    if (compo_index < 0 || compo_index >= part->nb_classes) return NULL;

    t_classe *classe = &part->classes[compo_index];
    int k = classe->taille;
    if (k <= 0) return NULL;

    t_matrix *S = createZeroMatrix(k);
    if (!S) return NULL;

    for (int i = 0; i < k; ++i) {
        int u_id = classe->sommets[i].id;
        int u = u_id - 1;
        for (int j = 0; j < k; ++j) {
            int v_id = classe->sommets[j].id;
            int v = v_id - 1;
            if (u >= 0 && u < M->n && v >= 0 && v < M->n) {
                S->data[i][j] = M->data[u][v];
            } else {
                S->data[i][j] = 0.0;
            }
        }
    }

    return S;
}