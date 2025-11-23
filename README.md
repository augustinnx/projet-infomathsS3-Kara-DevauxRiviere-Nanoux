# 🎲 Projet Graphes de Markov - TI301

[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/License-Academic-green.svg)]()

> Analyse automatique de chaînes de Markov à temps discret : partition en composantes fortement connexes, diagramme de Hasse, et calcul de distributions stationnaires.

## 📋 Table des matières

- [À propos](#-à-propos)
- [Fonctionnalités](#-fonctionnalités)
- [Structure du projet](#-structure-du-projet)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Exemples](#-exemples)
- [Algorithmes clés](#-algorithmes-clés)
- [Équipe](#-équipe)
- [Références](#-références)

---

## 🎯 À propos

Ce projet implémente une suite d'outils pour l'étude des **graphes de Markov** :
- Chargement et validation de graphes probabilistes
- Décomposition en composantes fortement connexes (algorithme de Tarjan)
- Génération de diagrammes de Hasse pour visualiser les relations entre classes
- Calculs matriciels pour déterminer les distributions stationnaires

**Contexte** : Projet commun Informatique-Mathématiques, Algorithmique et Structures de Données 2.

---

## ✨ Fonctionnalités

### Partie 1 : Gestion des graphes

- ✅ Chargement depuis fichier texte  
  **Fonctions utilisées :**  
  - `readGraph()`  
  - `createCell()`  
  - `addCell()`  
  - `printListe_d_adjacence()`

- ✅ Vérification de la propriété de Markov  
  La somme des probabilités sortantes doit être comprise entre **0.99 et 1.01**.  
  **Fonction :** `checkMarkov()`

- ✅ Visualisation du graphe original via Mermaid  
  Génération d’un fichier `.mmd` compatible MermaidChart  
  **Fonction :** `visualMermaid()`

---

### Partie 2 : Analyse structurelle

- ✅ Décomposition en composantes fortement connexes (Tarjan)  
  **Structures :**  
  - `t_tarjan_vertex`  
  - `t_classe`  
  - `t_stock_classe`  
  **Fonctions :**  
  - `tarjan()`  
  - `printPartition()`  
  - `freePartition()`

- ✅ Construction du diagramme de Hasse  
  **Fonctions :**  
  - `createVertexToClassMap()`  
  - `buildClassLinks()`  
  - `removeTransitiveLinks()`  
  - `generateHasseDiagram()`

- ✅ Analyse des propriétés du graphe  
  Détection automatique :  
  - classes **transitoires**  
  - classes **persistantes**  
  - **états absorbants**  
  - **irréductibilité**  
  **Fonction :** `analyzeGraphProperties()`

---

### Partie 3 : Calculs probabilistes

**Fonctions principales :**
- `createZeroMatrix()`  
- `graphToTransitionMatrix()`  
- `multiplyMatrix()`  
- `copyMatrix()`  
- `diffMatrix()`  
- `subMatrix()`

Ces fonctions permettent :
- de générer la **matrice de transition M**,  
- de calculer des puissances `M^n` (M³, M⁷…),  
- de détecter la **convergence** (`diff(M^k, M^(k-1)) < epsilon`),  
- d’extraire des sous-matrices par classe,  
- de calculer les **distributions stationnaires**.

---

### 🏁 Partie Bonus — Périodicité

Calcul automatique de la période des classes :  
- Détection des cycles  
- PGCD des longueurs  
- Analyse via puissances successives des sous-matrices  

---

## 📁 Structure du projet

```
.
├── data/                   # Fichiers de test (.txt)
│   ├── exemple1.txt
│   ├── exemple_meteo.txt
│   └── ...
├── hasse.c / hasse.h       # Diagramme de Hasse et analyse
├── matrix.c / matrix.h     # Opérations matricielles
├── tarjan.c / tarjan.h     # Algorithme de Tarjan
├── utils.c / utils.h       # Structures de base (liste d'adjacence)
├── main.c                  # Programme principal avec menu
└── README.md
```

---

## 🚀 Installation

### Prérequis
- Compilateur C (gcc, clang)
- Make (optionnel)

### Compilation

```bash
# Compilation simple
gcc -o markov main.c utils.c tarjan.c hasse.c matrix.c -lm

# Ou avec make (si Makefile fourni)
make
```

---

## 💻 Utilisation

### Lancer le programme

```bash
./markov
```





### Menu interactif

```
//================================\\
||              MENU              ||
//================================\\
1.  Charger un graphe depuis un fichier (dossier data)
2.  Afficher la liste d'adjacence
3.  Vérifier la propriété de Markov
4.  Générer le graphe original (Mermaid)
5.  Lancer Tarjan et afficher les classes
6.  Construire les liens entre classes (Hasse)
7.  Supprimer les liens transitifs (réduction Hasse)
8.  Générer le diagramme de Hasse (Mermaid)
9.  Analyser les propriétés du graphe
10. Tests sur l'exemple météo (M, M³, M⁷)
11. Test de convergence M^k pour un fichier
0.  Quitter
```

### Format des fichiers d'entrée

```
<nombre_de_sommets>
<sommet_départ> <sommet_arrivée> <probabilité>
...
```

**Exemple** (`exemple1.txt`) :
```
4
1 1 0.95
1 2 0.04
1 3 0.01
2 2 0.9
...
```

---

## 📊 Exemples

### Exemple 1 : Graphe simple à 4 états

```bash
Ton choix : 1
Nom du fichier : exemple1.txt
Graphe chargé depuis data/exemple1.txt.

Ton choix : 5
=== Partition en composantes fortement connexes ===
C1 : {1,2,3,4}
```

**Résultat** : Le graphe est irréductible (1 seule classe).

### Exemple 2 : Graphe météo

```bash
Ton choix : 10
--- Tests sur data/exemple_meteo.txt ---

Matrice M :
  0.3400   0.2700   0.0000   0.1800   0.2100
  0.2000   0.4000   0.2000   0.0000   0.2000
  ...

Matrice M^7 :
  0.1600   0.3600   0.1300   0.0500   0.2900
  0.1600   0.3600   0.1300   0.0500   0.2900
  ...
```

**Interprétation** : Convergence vers une distribution stationnaire après 7 jours.

### Exemple 3 : Visualisation Mermaid

Après avoir généré `graph.mmd` ou `hasse.mmd`, copiez le contenu sur [mermaidchart.com](https://www.mermaidchart.com/) pour visualiser le graphe.

---

## 🔧 Algorithmes clés

### Algorithme de Tarjan
- **Complexité** : O(V + E)
- **But** : Trouver les composantes fortement connexes en un seul parcours DFS
- **Principe** : Utilise une pile et deux numéros par sommet (num, num_acc)

### Réduction transitive
- **But** : Supprimer les arêtes redondantes (A→C si A→B et B→C existent)
- **Implémentation** : Parcours exhaustif avec suppression in-place

### Convergence matricielle
- **Méthode** : Calcul de Mⁿ jusqu'à ce que ||Mⁿ - Mⁿ⁻¹|| < ε
- **Seuil par défaut** : ε = 0.01, max 1000 itérations

---

## 👥 Équipe

| Nom | Rôle | Contributions principales |
|-----|------|--------------------------|
| **Walid** | Gestion des données | Chargement, fichiers, visualisation Mermaid |
| **Augustin** | Structures & Matrices | SDD Tarjan, opérations matricielles |
| **Aurélien** | Algorithmes avancés | Tarjan, liens entre classes, convergence |

---

## 📚 Références

- [Algorithme de Tarjan](https://fr.wikipedia.org/wiki/Algorithme_de_Tarjan)
- [Chaînes de Markov](https://fr.wikipedia.org/wiki/Cha%C3%AEne_de_Markov)
- [Mermaid Diagrams](https://mermaid.js.org/)

---



