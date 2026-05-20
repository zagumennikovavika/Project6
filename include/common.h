
#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR (-1)

// Structure for comparing MST construction algorithms
typedef struct {
    double timeKruskal;
    double timePrim;
    int KruskalWeight;
    int PrimWeight;
    int numEdges;
}Comparison;

// Structure for storing MST construction result
typedef struct {
    int totalWeight;
    int *u, *v;
    int *weights;
    int numEdges;
}MSTResult;

// Structure for adjacency list node
typedef struct {
    int number;
    int weight;
    struct EdgeNode *next;
}EdgeNode;

// Graph structure
typedef struct {
    int numVertices;
    int numEdges;
    EdgeNode**matrix;
    char **cityNames;
}Graph;

// Structure for Dijkstra's algorithm result
typedef struct {
    int *dist;
    int *prev;
    int source;
}DijkstraResult;

typedef struct Node {
    int number;
    struct Node* next;
} Node;

typedef struct {
    int number;
    Node* head;
    Node* rear;
} Queue;

#endif //COMMON_H
