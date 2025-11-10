#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* getID en version exportée (non static) */
const char *getID(int i) {
    // 1->A, 26->Z, 27->AA...
    static char buffer[16];
    char temp[16];
    int index = 0;

    if (i <= 0) {
        snprintf(buffer, sizeof(buffer), "?");
        return buffer;
    }

    i--; // 0-based
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }
    // reverse
    for (int j = 0; j < index; ++j) buffer[j] = temp[index - j - 1];
    buffer[index] = '\0';
    return buffer;
}

int readWeightedTriplesToMatrix(const char *path, int *out_n, double ***out_mat) {
    if (!path || !out_n || !out_mat) return -1;

    FILE *fp = fopen(path, "r");
    if (!fp) return -2;

    int n;
    if (fscanf(fp, "%d", &n) != 1 || n <= 0) {
        fclose(fp);
        return -3;
    }

    // Alloue matrice n x n et initialise à 0.0
    double **mat = (double **)malloc(sizeof(double *) * n);
    if (!mat) { fclose(fp); return -4; }
    for (int i = 0; i < n; ++i) {
        mat[i] = (double *)calloc(n, sizeof(double));
        if (!mat[i]) {
            for (int k = 0; k < i; ++k) free(mat[k]);
            free(mat);
            fclose(fp);
            return -4;
        }
    }

    // Lit des triplets jusqu'à la fin du fichier
    int u, v;
    double w;
    while (fscanf(fp, "%d %d %lf", &u, &v, &w) == 3) {
        if (u >= 1 && u <= n && v >= 1 && v <= n) {
            mat[u - 1][v - 1] = w;
        }
    }

    fclose(fp);
    *out_n = n;
    *out_mat = mat;
    return 0;
}

void freeMatrix(int n, double **mat) {
    if (!mat) return;
    for (int i = 0; i < n; ++i) free(mat[i]);
    free(mat);
}

void printLinksLabeled(void printLinksLabeled(FILE *f, int n, const t_link *links, int count);) {
    if (!f) f = stdout;
    for (int i = 0; i < count; ++i) {
        const char *A = getID(links[i].from);
        const char *B = getID(links[i].to);
        fprintf(f, "%s -> %s\n", A, B);
    }
}
