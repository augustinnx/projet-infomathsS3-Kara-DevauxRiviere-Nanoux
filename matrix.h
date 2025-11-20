#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include "utils.h"
#include "tarjan.h"

/**
 * Matrice carrée n x n de doubles
 */
typedef struct {
    int n;          // dimension (n x n)
    double **data;  // data[i][j]
} t_matrix;

/* ====== Gestion de base ====== */

// Crée une matrice n x n remplie de 0
t_matrix *createZeroMatrix(int n);

// Libère une matrice
void freeMatrix(t_matrix *m);

// Affiche une matrice (pour debug / validation)
void printMatrix(const t_matrix *m);

/* ====== Fonctions partie 3.1 ====== */

// Construit la matrice de transition M à partir de la liste d'adjacence G
// M[i][j] = proba(i -> j)
t_matrix *graphToTransitionMatrix(const liste_d_adjacence *g);

// Recopie src dans dst (même dimension n)
void copyMatrix(const t_matrix *src, t_matrix *dst);

// Renvoie C = A * B (nouvelle matrice allouée dynamiquement)
t_matrix *multiplyMatrix(const t_matrix *A, const t_matrix *B);

// Calcule diff(A, B) = somme_{i,j} |a_ij - b_ij|
double diffMatrix(const t_matrix *A, const t_matrix *B);

/* ====== Fonctions partie 3.2 ====== */

// Extrait la sous-matrice correspondant à la composante d'indice compo_index
// dans la partition part (0..part->nb_classes-1).
// Retourne une nouvelle matrice allouée dynamiquement de taille k x k
// où k = nombre de sommets dans cette classe.
t_matrix *subMatrix(const t_matrix *M, const t_partition *part, int compo_index);

#endif
