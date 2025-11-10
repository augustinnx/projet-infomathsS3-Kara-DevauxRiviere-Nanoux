#include <stdio.h>
#include "utils.h"

int main(void)
{
    // Étape 1 : lire le graphe et afficher la liste d'adjacence
    liste_d_adjacence G = readGraph("../data/exemple1.txt");
    printListe_d_adjacence(&G);

    // Étape 2: vérifier la propriété de Markov
    checkMarkov(&G);

    // Étape 3 : générer le fichier Mermaid
    visualMermaid(&G, "graph.mmd");

    return 0;
}
