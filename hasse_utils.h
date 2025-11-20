#ifndef __HASSE_UTILS_H__
#define __HASSE_UTILS_H__

#include "tarjan.h"
#include "hasse.h"
#include "utils.h"

// Fonctions pour le diagramme de Hasse
int* createVertexToClassMap(const t_partition *p, int n);
t_link_array* buildClassLinks(const liste_d_adjacence *g, const t_partition *p);
void generateHasseDiagram(const t_partition *p, const t_link_array *links, const char *filepath);
void analyzeGraphProperties(const t_partition *p, const t_link_array *links);

#endif
