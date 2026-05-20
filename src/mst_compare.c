#include <time.h>
#include "mst_compare.h"
#include "mst_algorithms.h"


double GetTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

Comparison CompareMST(const Graph* g) {
    Comparison cmp = {0.0, 0.0, 0, 0, 0};
    if (!g) return cmp;

    double startK = GetTime();
    MSTResult* resK = kruskal(g);
    double endK = GetTime();

    double startP = GetTime();
    MSTResult* resP = prim(g);
    double endP = GetTime();

    if (resK) {
        cmp.KruskalWeight = resK->totalWeight;
        cmp.numEdges = resK->numEdges;
        FreeMSTResult(resK);
    }
    cmp.timeKruskal = endK - startK;

    if (resP) {
        cmp.PrimWeight = resP->totalWeight;
        FreeMSTResult(resP);
    }
    cmp.timePrim = endP - startP;

    return cmp;
}
