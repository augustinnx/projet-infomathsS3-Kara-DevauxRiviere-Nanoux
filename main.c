#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "hasse.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier_exemple> [threshold]\n", argv[0]);
        return 1;
    }
    const char *path = argv[1];
    double threshold = (argc >= 3 ? atof(argv[2]) : 0.5);

    int n = 0;
    double **mat = NULL;
    if (readWeightedTriplesToMatrix(path, &n, &mat) != 0) {
        fprintf(stderr, "Erreur de lecture du fichier: %s\n", path);
        return 2;
    }

    t_link_array L = {0};
    if (buildLinksFromWeightedMatrix(n, mat, threshold, &L) != 0) {
        fprintf(stderr, "Erreur buildLinksFromWeightedMatrix\n");
        freeMatrix(n, mat);
        return 3;
    }

    // Réduction transitive
    removeTransitiveLinks(&L);

    // Affichage
    printf("Hasse (seuil = %.4f) :\n", threshold);
    printLinksLabeled(stdout, n, (const void*)L.links, L.log_size);

    freeLinkArray(&L);
    freeMatrix(n, mat);
    return 0;
}
