#ifndef __HASSE_H__
#define __HASSE_H__

#include <stddef.h>

/* ========== Types de liens pour le diagramme de Hasse ========== */

typedef struct {
    int from;  // 1-based
    int to;    // 1-based
} t_link;

typedef struct {
    t_link *links;
    int log_size;
    int capacity;
} t_link_array;

/* ========== API Hasse ========== */

/**
 * @brief Supprime les arêtes transitives (réduction transitive) dans p_link_array.
 *        On conserve uniquement les arêtes de couverture (cover relation).
 */
void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Construit la liste de liens (arêtes) à partir d'une matrice d'adjacence pondérée.
 * @param n         nombre de sommets (1..n)
 * @param mat       matrice n x n (double), indices 0..n-1
 * @param threshold seuil (ex: 0.5). On crée un arc i->j si mat[i][j] >= threshold et i!=j
 * @param out       (sortie) tableau dynamique de liens
 * @return 0 si OK, non-0 sinon
 */
int buildLinksFromWeightedMatrix(int n, double **mat, double threshold, t_link_array *out);

/**
 * @brief Libère la mémoire d'un t_link_array.
 */
void freeLinkArray(t_link_array *arr);

#endif
