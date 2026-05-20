#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "common.h"

#define MAX_LINE 256

EdgeNode* CreateEdgeNode(int num, int weight);
Graph* CreateGraph(int numVertices);
void AddEdge(Graph* graph, int u, int v, int weight);
Graph* LoadGraphEdges(const char* filename);
Graph* LoadGraphMatrix(const char* filename);
int SaveGraphEdges(const Graph* graph, const char* filename);
void FreeAdgeNode(EdgeNode* node);
void FreeGraph(Graph* graph);
int FindMinDistance(int* distance, int* visited, int numVertices);
DijkstraResult* Dijkstra(const Graph* graph, int start);
void PrintDijkstraResult(const DijkstraResult* result, const Graph* graph);
void FreeDijkstraResult(DijkstraResult* result);
int* GetPath(const DijkstraResult* result, const Graph* graph, int target, int *pathLen);
Queue* CreateQueue();
void Enqueue(Queue* q, int value);
int Dequeue(Queue* q);
int IsEmpty(Queue* q);
void FreeQueue(Queue* q);
int* BFS(const Graph* graph, int start, int* orderlen);

#endif