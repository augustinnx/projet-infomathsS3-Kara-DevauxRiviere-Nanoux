#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

/* ====== Gestion de base ====== */

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
            // en cas d'échec, cleanup
            for (int k = 0; k < i; ++k) free(m->data[k]);
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

/* ====== Partie 3.1 : construction et opérations de base ====== */

// Construit la matrice de transition M à partir de la liste d'adjacence g
t_matrix *graphToTransitionMatrix(const liste_d_adjacence *g) {
    if (!g || g->n <= 0 || !g->list) return NULL;

    int n = g->n;
    t_matrix *M = createZeroMatrix(n);
    if (!M) return NULL;

    // Pour chaque sommet i
    for (int i = 0; i < n; ++i) {
        cell *cur = g->list[i].head;
        // Pour chaque arête i -> j avec probabilité p
        while (cur) {
            int j = cur->arriv - 1;          // indices 0-based
            if (j >= 0 && j < n) {
                M->data[i][j] = (double)cur->proba;
            }
            cur = cur->next;
        }
    }

    return M;
}

// Recopie src dans dst (même taille n)
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

// C = A * B (nouvelle matrice)
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

// diff(A, B) = somme |a_ij - b_ij|
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

/* ====== Partie 3.2 : sous-matrice d'une classe ====== */

// Sous-matrice correspondant à la classe part->classes[compo_index]
// On suppose que t_classe ressemble à :
//   name, t_tarjan_vertex* vertices, int size, int capacity
t_matrix *subMatrix(const t_matrix *M, const t_partition *part, int compo_index) {
    if (!M || !part) return NULL;
    if (compo_index < 0 || compo_index >= part->nb_classes) return NULL;

    t_classe *classe = &part->classes[compo_index];
    int k = classe->size;   // nb de sommets dans cette classe

    if (k <= 0) return NULL;

    t_matrix *S = createZeroMatrix(k);
    if (!S) return NULL;

    
    for (int i = 0; i < k; ++i) {
        int u_id = classe->vertices[i].id;    // 1..M->n
        int u = u_id - 1;                     
        for (int j = 0; j < k; ++j) {
            int v_id = classe->vertices[j].id;
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
