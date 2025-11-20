#include <stdlib.h>
#include "hasse.h"

/* Petit utilitaire interne */
static void linkArrayInit(t_link_array *arr, int cap) {
    arr->links = (t_link *)malloc(sizeof(t_link) * (cap > 0 ? cap : 8));
    arr->log_size = 0;
    arr->capacity = (cap > 0 ? cap : 8);
}
static void linkArrayPush(t_link_array *arr, t_link e) {
    if (arr->log_size >= arr->capacity) {
        arr->capacity = (arr->capacity * 2);
        arr->links = (t_link *)realloc(arr->links, sizeof(t_link) * arr->capacity);
    }
    arr->links[arr->log_size++] = e;
}

void freeLinkArray(t_link_array *arr) {
    if (!arr) return;
    free(arr->links);
    arr->links = NULL;
    arr->log_size = 0;
    arr->capacity = 0;
}

int buildLinksFromWeightedMatrix(int n, double **mat, double threshold, t_link_array *out) {
    if (!mat || !out || n <= 0) return -1;
    linkArrayInit(out, n * 2);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (mat[i][j] >= threshold) {
                t_link e = { .from = i + 1, .to = j + 1 }; // stocké en 1-based
                linkArrayPush(out, e);
            }
        }
    }
    return 0;
}

/* Version robuste et in-place de la réduction transitive.
 * Idée : on supprime un arc a->c s'il existe a->b et b->c.
 * Complexité O(m^3) naïve mais OK pour tailles modestes.
 */
void removeTransitiveLinks(t_link_array *p_link_array) {
    if (!p_link_array || p_link_array->log_size <= 1) return;

    int i = 0;
    while (i < p_link_array->log_size) {
        t_link link1 = p_link_array->links[i];
        int to_remove = 0;

        for (int j = 0; j < p_link_array->log_size && !to_remove; ++j) {
            if (j == i) continue;
            t_link link2 = p_link_array->links[j];
            if (link1.from == link2.from) {
                // chercher link3: link2.to -> link1.to
                for (int k = 0; k < p_link_array->log_size && !to_remove; ++k) {
                    if (k == j || k == i) continue;
                    t_link link3 = p_link_array->links[k];
                    if (link3.from == link2.to && link3.to == link1.to) {
                        to_remove = 1;
                    }
                }
            }
        }

        if (to_remove) {
            // supprime link1 en remplaçant par le dernier
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        } else {
            i++;
        }
    }
}

