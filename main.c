#include <stdio.h>
#include "utils.h"
#include "tarjan.h"
#include "hasse_utils.h"

int main(void)
{
    // Partie 1
    printf("=== PARTIE 1 ===\n");
    liste_d_adjacence G = readGraph("../data/exemple_valid_step3.txt");
    printListe_d_adjacence(&G);
    checkMarkov(&G);
    visualMermaid(&G, "graph.mmd");
    printf("Fichier Mermaid (graphe): graph.mmd\n");
    
    // Partie 2
    printf("\n=== PARTIE 2 ===\n");
    
    // Étape 1: Tarjan (composantes fortement connexes)
    t_partition *partition = tarjan(&G);
    printPartition(partition);
    
    // Étape 2: Diagramme de Hasse
    t_link_array *links = buildClassLinks(&G, partition);
    

}