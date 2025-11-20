#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "tarjan.h"
#include "hasse.h"

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
        printf("0. Quitter\n");
        printf("Ton choix : ");

        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Erreur de saisie.\n");
            return 1;
        }

        switch (choice) {

        /* === 1. CHARGER GRAPHE === */
        case 1: {
            printf("Nom du fichier (ex: exemple1.txt) : ");
            scanf("%255s", filename);

            char filepath[300];
            snprintf(filepath, sizeof(filepath), "data/%s", filename);

            g = readGraph(filepath);
            graphLoaded = 1;
            printf("Graphe chargé depuis %s\n", filepath);
            break;
        }

        /* === 2. AFFICHAGE LISTE ADJ === */
        case 2:
            if (!graphLoaded) { printf("Charge un graphe d'abord.\n"); break; }
            printListe_d_adjacence(&g);
            break;

        /* === 3. CHECK MARKOV === */
        case 3:
            if (!graphLoaded) { printf("Charge un graphe d'abord.\n"); break; }
            checkMarkov(&g);   
            break;

        /* === 4. MERMAID GRAPHE ORIGINAL === */
        case 4:
            if (!graphLoaded) { printf("Charge un graphe d'abord.\n"); break; }
            visualMermaid(&g, "graph.mmd");
            printf("Graphe original généré → graph.mmd\n");
            break;

        /* === 5. TARJAN === */
        case 5:
            if (!graphLoaded) { printf("Charge un graphe d'abord.\n"); break; }

            if (p) { freePartition(p); p = NULL; }

            p = tarjan(&g);
            if (!p) { printf("Erreur pendant Tarjan.\n"); break; }

            partitionComputed = 1;
            printPartition(p);
            break;

        /* === 6. LIENS ENTRE CLASSES === */
        case 6:
            if (!partitionComputed) { printf("Lance Tarjan d'abord.\n"); break; }

            if (links) {
                free(links->links);
                free(links);
            }

            links = buildClassLinks(&g, p);
            if (!links) { printf("Erreur lors de la construction des liens.\n"); break; }

            linksBuilt = 1;
            printf("Liens entre classes générés.\n");
            break;

        /* === 7. REDUCTION Hasse === */
        case 7:
            if (!linksBuilt || !links) { printf("Construis les liens d'abord.\n"); break; }
            removeTransitiveLinks(links);
            printf("Liens transitifs supprimés.\n");
            break;

        /* === 8. MERMAID HASSE === */
        case 8:
            if (!partitionComputed || !linksBuilt || !links) {
                printf("Faut d'abord lancer Tarjan et créer les liens.\n");
                break;
            }
            generateHasseDiagram(p, links, "hasse.mmd");
            break;

        /* === 9. ANALYSE PROPRIÉTÉS === */
        case 9:
            if (!partitionComputed || !linksBuilt || !links) {
                printf("Faut d'abord lancer Tarjan et créer les liens.\n");
                break;
            }
            analyzeGraphProperties(p, links);
            break;

        /* === 0. QUITTER === */
        case 0:
            break;

        default:
            printf("Choix invalide.\n");
        }
    }

    if (p) freePartition(p);
    if (links) { free(links->links); free(links); }

    return 0;
}
