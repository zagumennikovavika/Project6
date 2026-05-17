#ifndef GRAPH_MASTER_FRONT1_H
#define GRAPH_MASTER_FRONT1_H
#include "common.h"

void Dijkstra(Graph *g, const int *path, int pathLen, int totalWeight, double timeMS);
char* AskFilename();
int Menu();
void PrintGraphInfo(int numVertices,int numEdges);
void PrintComparison(const Comparison* cmp);
void PrintBFS(Graph *g, int *order, int orderLen, double timeMS);
void PrintMST(const MSTResult *res, Graph *g);

#endif //GRAPH_MASTER_FRONT1_H
