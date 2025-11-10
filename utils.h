#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

/* ========== Lectures / outils généraux ========== */

/**
 * @brief Convertit 1 -> "A", 26 -> "Z", 27 -> "AA", etc.
 */
const char *getID(int i);

/**
 * @brief Charge une matrice pondérée n x n depuis un fichier.
 *        Format attendu:
 *          n
 *          u v w
 *          u v w
 *          ...
 *        avec 1 <= u,v <= n. Les lignes suivantes peuvent être en nombre variable.
 * @param path chemin du fichier
 * @param out_n (sortie) nombre de sommets
 * @param out_mat (sortie) matrice allouée dynamiquement [n][n]
 * @return 0 si OK, non-0 sinon
 */
int readWeightedTriplesToMatrix(const char *path, int *out_n, double ***out_mat);

/**
 * @brief Libère une matrice n x n de double.
 */
void freeMatrix(int n, double **mat);

/**
 * @brief Affiche une liste de liens (u->v) avec des IDs lettres (A,B,...).
 */
void printLinksLabeled(void printLinksLabeled(FILE *f, int n, const t_link *links, int count););

#endif
