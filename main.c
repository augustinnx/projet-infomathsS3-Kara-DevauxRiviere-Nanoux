#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"

int main(void) {
    liste_d_adjacence g;
    t_partition *p = NULL;
    t_link_array *links = NULL;

    int graphLoaded = 0;
    int partitionComputed = 0;
    int linksBuilt = 0;

    char filename[256];
    int choice = -1;

    while (choice != 0) {
        printf("\n===== MENU =====\n");
        printf("1. Charger un graphe depuis un fichier (dossier data)\n");
        printf("2. Afficher la liste d'adjacence\n");
        printf("3. Vérifier la propriété de Markov\n");
        printf("4. Générer le graphe original (Mermaid)\n");
        printf("5. Lancer Tarjan et afficher les classes\n");
        printf("6. Construire les liens entre classes (Hasse)\n");
        printf("7. Supprimer les liens transitifs (réduction Hasse)\n");
        printf("8. Générer le diagramme de Hasse (Mermaid)\n");
        printf("9. Analyser les propriétés du graphe\n");
        printf("10. Tests sur l'exemple météo (M, M^3, M^7)\n");
        printf("11. Test de convergence M^k pour un fichier\n");
        printf("0. Quitter\n");
        printf("Ton choix : ");

        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Erreur de saisie.\n");
            return 1;
        }

        switch (choice) {

        /*  1. CHARGER GRAPHE  */
        case 1: {
            printf("Nom du fichier (ex: exemple1.txt) : ");
            scanf("%255s", filename);

            char filepath[300];
            snprintf(filepath, sizeof(filepath), "data/%s", filename);

            g = readGraph(filepath);
            graphLoaded = 1;
            partitionComputed = 0;
            linksBuilt = 0;

            printf("Graphe chargé depuis %s.\n", filepath);
            break;
        }

        /*  2. AFFICHER LISTE ADJ  */
        case 2:
            if (!graphLoaded) { 
                printf("Charge un graphe d'abord.\n"); 
                break; 
            }
            printListe_d_adjacence(&g);
            break;

        /*  3. CHECK MARKOV  */
        case 3:
            if (!graphLoaded) { 
                printf("Charge un graphe d'abord.\n"); 
                break; 
            }
            checkMarkov(&g);   // renvoie void
            break;

        /*  4. MERMAID GRAPHE ORIGINAL  */
        case 4:
            if (!graphLoaded) { 
                printf("Charge un graphe d'abord.\n"); 
                break; 
            }
            visualMermaid(&g, "graph.mmd");
            printf("Graphe original généré → graph.mmd\n");
            break;

        /*  5. TARJAN  */
        case 5:
            if (!graphLoaded) { 
                printf("Charge un graphe d'abord.\n"); 
                break; 
            }

            if (p) { 
                freePartition(p); 
                p = NULL; 
            }

            p = tarjan(&g);
            if (!p) { 
                printf("Erreur pendant Tarjan.\n"); 
                break; 
            }

            partitionComputed = 1;
            printPartition(p);
            break;

        /*  6. LIENS ENTRE CLASSES  */
        case 6:
            if (!partitionComputed) { 
                printf("Lance Tarjan d'abord.\n"); 
                break; 
            }

            if (links) {
                free(links->links);
                free(links);
                links = NULL;
            }

            links = buildClassLinks(&g, p);
            if (!links) { 
                printf("Erreur lors de la construction des liens.\n"); 
                break; 
            }

            linksBuilt = 1;
            printf("Liens entre classes générés.\n");
            break;

        /*  7. RÉDUCTION TRANSITIVE (HASSE)  */
        case 7:
            if (!linksBuilt || !links) { 
                printf("Construis les liens d'abord.\n"); 
                break; 
            }
            removeTransitiveLinks(links);
            printf("Liens transitifs supprimés.\n");
            break;

        /*  8. MERMAID HASSE  */
        case 8:
            if (!partitionComputed || !linksBuilt || !links) {
                printf("Faut d'abord lancer Tarjan et créer les liens.\n");
                break;
            }
            generateHasseDiagram(p, links, "hasse.mmd");
            printf("Diagramme de Hasse généré → hasse.mmd\n");
            break;

        /*  9. ANALYSE PROPRIÉTÉS  */
        case 9:
            if (!partitionComputed || !linksBuilt || !links) {
                printf("Faut d'abord lancer Tarjan et créer les liens.\n");
                break;
            }
            analyzeGraphProperties(p, links);
            break;

        /*  10. TESTS EXEMPLE MÉTÉO (M, M^3, M^7)  */
        case 10: {
            const char *filepath = "data/exemple_meteo.txt";
            printf("\n--- Tests sur %s ---\n", filepath);

            liste_d_adjacence g_meteo = readGraph(filepath);
            t_matrix *M = graphToTransitionMatrix(&g_meteo);
            if (!M) {
                printf("Erreur: impossible de construire la matrice M.\n");
                break;
            }

            printf("\nMatrice M :\n");
            printMatrix(M);

            // M^3
            t_matrix *M2 = multiplyMatrix(M, M);      // M^2
            t_matrix *M3 = multiplyMatrix(M2, M);     // M^3
            printf("\nMatrice M^3 :\n");
            printMatrix(M3);

            // M^7
            t_matrix *Mk = createZeroMatrix(M->n);
            copyMatrix(M, Mk);  // Mk = M (puissance 1)
            for (int k = 2; k <= 7; ++k) {
                t_matrix *tmp = multiplyMatrix(Mk, M);
                freeMatrix(Mk);
                Mk = tmp;
            }
            printf("\nMatrice M^7 :\n");
            printMatrix(Mk);

            // libération
            freeMatrix(M);
            freeMatrix(M2);
            freeMatrix(M3);
            freeMatrix(Mk);

            printf("\n(Fin des tests sur l'exemple météo.)\n");
            break;
        }

        /*  11. TEST DE CONVERGENCE M^k POUR UN FICHIER */
        case 11: {
            printf("Nom du fichier (ex: exemple1.txt) : ");
            scanf("%255s", filename);

            char filepath[300];
            snprintf(filepath, sizeof(filepath), "data/%s", filename);

            printf("\n--- Test de convergence pour %s ---\n", filepath);

            liste_d_adjacence g_local = readGraph(filepath);
            t_matrix *M = graphToTransitionMatrix(&g_local);
            if (!M) {
                printf("Erreur: impossible de construire la matrice M.\n");
                break;
            }

            double eps = 0.01;
            int max_iter = 1000;
            int n = M->n;

            t_matrix *Mk_prev = createZeroMatrix(n);
            copyMatrix(M, Mk_prev);   // M^1

            int converged = 0;
            double last_diff = 0.0;

            for (int k = 2; k <= max_iter; ++k) {
                t_matrix *Mk = multiplyMatrix(Mk_prev, M);  // M^k = M^{k-1} * M
                last_diff = diffMatrix(Mk, Mk_prev);

                if (last_diff < eps) {
                    printf("Convergence atteinte à k = %d, diff = %.6f < %.6f\n",
                           k, last_diff, eps);
                    converged = 1;
                    freeMatrix(Mk);
                    break;
                }

                freeMatrix(Mk_prev);
                Mk_prev = Mk;
            }

            if (!converged) {
                printf("Pas de convergence (diff >= %.6f) après %d itérations.\n", 
                       eps, max_iter);
                printf("Dernière diff = %.6f\n", last_diff);
            }

            freeMatrix(M);
            freeMatrix(Mk_prev);
            break;
        }

        /*  0. QUITTER  */
        case 0:
            break;

        default:
            printf("Choix invalide.\n");
        }
    }

    if (p) freePartition(p);
    if (links) {
        free(links->links);
        free(links);
    }

    return 0;
}


