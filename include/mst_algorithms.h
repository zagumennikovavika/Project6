#ifndef MST_ALGORITHMS_H
#define MST_ALGORITHMS_H

#include "common.h"

MSTResult* kruskal(const Graph* graph);
MSTResult* prim(const Graph* graph);
void FreeMSTResult(MSTResult* result);

#endif