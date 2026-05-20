#include <time.h>
#include "mst_compare.h"
#include "mst_algorithms.h"

#ifdef _WIN32
    #include <windows.h>   
#endif
double GetTime() {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (double)count.QuadPart / (double)freq.QuadPart;
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
