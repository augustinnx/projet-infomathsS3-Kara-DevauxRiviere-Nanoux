#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include "utils.h"
#include "tarjan.h"

// Matrice carrée n x n de doubles
typedef struct {
    int n;
    double **data;
} t_matrix;

// Création d'une matrice n x n remplie de 0
t_matrix *createZeroMatrix(int n);

// Libération de la mémoire d'une matrice
void freeMatrix(t_matrix *m);

// Affichage d'une matrice carrée
void printMatrix(const t_matrix *m);

// Construction de la matrice de transition M à partir du graphe
t_matrix *graphToTransitionMatrix(const liste_d_adjacence *g);

// Copie du contenu d'une matrice source vers une matrice destination
void copyMatrix(const t_matrix *src, t_matrix *dst);

// Multiplication de deux matrices carrées A et B
t_matrix *multiplyMatrix(const t_matrix *A, const t_matrix *B);

// Calcul de la somme des différences absolues entre deux matrices
double diffMatrix(const t_matrix *A, const t_matrix *B);

// Extraction de la sous-matrice associée à une composante fortement connexe
t_matrix *subMatrix(const t_matrix *M, const t_stock_classe *part, int compo_index);

#endif