#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define MAX_LINE 256
#define ERROR -1

typedef struct Node {
int number;
struct Node* next;
} Node;

typedef struct {
int number;
Node* head;
Node* rear;
} Queue;

typedef struct EdgeNode {
int number;
int weight;
struct EdgeNode *next;
} EdgeNode;

typedef struct {
int numVertices;
int numEdges;
EdgeNode **matrix;
char **cityNames;
} Graph;

typedef struct {
int *dist;
int *prev;
int source;
} DijkstraResult;

EdgeNode* CreateEdgeNode(int num, int weight);
Graph* CreateGraph(int numVertices);
void AddEdge(Graph* graph, int u, int v, int weight);
Graph* LoadGraphEdges(const char* filename);
Graph* loadGraphMatrix(const char* filename);
int SaveGraphEdges(const Graph* graph, const char* filename);
void FreeAdgeNode(EdgeNode* node);
void FreeGraph(Graph* graph);
int FindMinDistance(int distance, int visited, int numVertices);
DijkstraResult dijkstra(const Graph graph, int start);
void PrintDijkstraResult(const DijkstraResult result, const Graph graph);
void FreeDijkstraResult(DijkstraResult* result);
int* GetPath(const DijkstraResult* result, const Graph* graph, int target);
Queue* CreateQueue();
void Enqueue(Queue* q, int value);
int Dequeue(Queue* q);
int IsEmpty(Queue* q);
void FreeQueue(Queue* q);
int* BFS(const Graph* graph, int* orderlen, int start);

#endif