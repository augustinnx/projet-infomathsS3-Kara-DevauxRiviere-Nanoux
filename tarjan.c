#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"

// Pile pour l'algorithme de Tarjan
typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

Stack* createStack(int capacity) {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    s->data = (int*)malloc(sizeof(int) * capacity);
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void push(Stack *s, int val) {
    if (s->top < s->capacity - 1) {
        s->data[++s->top] = val;
    }
}

int pop(Stack *s) {
    if (s->top >= 0) {
        return s->data[s->top--];
    }
    return -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

int contains(Stack *s, int val) {
    for (int i = 0; i <= s->top; i++) {
        if (s->data[i] == val) return 1;
    }
    return 0;
}

void freeStack(Stack *s) {
    if (s) {
        free(s->data);
        free(s);
    }
}

// Initialisation du tableau de sommets Tarjan
t_tarjan_vertex* initTarjanVertices(const liste_d_adjacence *g) {
    if (!g || g->n <= 0) return NULL;
    
    t_tarjan_vertex *vertices = (t_tarjan_vertex*)malloc(sizeof(t_tarjan_vertex) * g->n);
    for (int i = 0; i < g->n; i++) {
        vertices[i].id = i + 1;
        vertices[i].num = -1;
        vertices[i].low = -1;
        vertices[i].in_stack = 0;
    }
    return vertices;
}

// Fonction parcours (récursive)
void parcours(int v, const liste_d_adjacence *g, t_tarjan_vertex *vertices,
              Stack *stack, int *counter, t_partition *partition) {
    
    vertices[v].num = *counter;
    vertices[v].low = *counter;
    (*counter)++;
    
    push(stack, v);
    vertices[v].in_stack = 1;
    
    cell *cur = g->list[v].head;
    while (cur) {
        int w = cur->arriv - 1;
        
        if (vertices[w].num == -1) {
            parcours(w, g, vertices, stack, counter, partition);
            if (vertices[w].low < vertices[v].low) {
                vertices[v].low = vertices[w].low;
            }
        } else if (vertices[w].in_stack) {
            if (vertices[w].num < vertices[v].low) {
                vertices[v].low = vertices[w].num;
            }
        }
        
        cur = cur->next;
    }
    
    if (vertices[v].low == vertices[v].num) {
        t_classe *new_classe = (t_classe*)malloc(sizeof(t_classe));
        sprintf(new_classe->name, "C%d", partition->nb_classes + 1);
        new_classe->vertices = NULL;
        new_classe->size = 0;
        new_classe->capacity = 4;
        new_classe->vertices = (t_tarjan_vertex*)malloc(sizeof(t_tarjan_vertex) * new_classe->capacity);
        
        int w;
        do {
            w = pop(stack);
            vertices[w].in_stack = 0;
            
            if (new_classe->size >= new_classe->capacity) {
                new_classe->capacity *= 2;
                new_classe->vertices = (t_tarjan_vertex*)realloc(new_classe->vertices, 
                                        sizeof(t_tarjan_vertex) * new_classe->capacity);
            }
            
            new_classe->vertices[new_classe->size++] = vertices[w];
        } while (w != v);
        
        if (partition->nb_classes >= partition->capacity) {
            partition->capacity *= 2;
            partition->classes = (t_classe*)realloc(partition->classes,
                                  sizeof(t_classe) * partition->capacity);
        }
        partition->classes[partition->nb_classes++] = *new_classe;
    }
}

// Fonction principale Tarjan
t_partition* tarjan(const liste_d_adjacence *g) {
    if (!g || g->n <= 0) return NULL;
    
    t_partition *partition = (t_partition*)malloc(sizeof(t_partition));
    partition->nb_classes = 0;
    partition->capacity = 4;
    partition->classes = (t_classe*)malloc(sizeof(t_classe) * partition->capacity);
    
    t_tarjan_vertex *vertices = initTarjanVertices(g);
    Stack *stack = createStack(g->n);
    int counter = 0;
    
    for (int i = 0; i < g->n; i++) {
        if (vertices[i].num == -1) {
            parcours(i, g, vertices, stack, &counter, partition);
        }
    }
    
    freeStack(stack);
    free(vertices);
    
    return partition;
}

// Affichage de la partition
void printPartition(const t_partition *p) {
    if (!p) return;
    
    printf("\n=== Partition en composantes fortement connexes ===\n");
    for (int i = 0; i < p->nb_classes; i++) {
        printf("Composante %s: {", p->classes[i].name);
        for (int j = 0; j < p->classes[i].size; j++) {
            printf("%d", p->classes[i].vertices[j].id);
            if (j < p->classes[i].size - 1) printf(",");
        }
        printf("}\n");
    }
}

// Libération mémoire
void freePartition(t_partition *p) {
    if (!p) return;
    for (int i = 0; i < p->nb_classes; i++) {
        free(p->classes[i].vertices);
    }
    free(p->classes);
    free(p);
}