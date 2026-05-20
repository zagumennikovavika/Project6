#include <stdlib.h>
#include <limits.h>
#include "mst_algorithms.h"
#include "dsu.h"

typedef struct {
    int u, v, weight;
} Edge;

static int CompareEdges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

void FreeMSTResult(MSTResult* result) {
    if (result == NULL) return;
    free(result->u);
    free(result->v);
    free(result->weights);
    free(result);
}

static MSTResult* InitMSTResult(int V) {
    MSTResult* res = (MSTResult*)malloc(sizeof(MSTResult));
    if (!res) return NULL;
    res->u = (int*)malloc((V - 1) * sizeof(int));
    res->v = (int*)malloc((V - 1) * sizeof(int));
    res->weights = (int*)malloc((V - 1) * sizeof(int));
    res->totalWeight = 0;
    res->numEdges = 0;

    if (!res->u || !res->v || !res->weights) {
        FreeMSTResult(res);
        return NULL;
    }
    return res;
}

MSTResult* kruskal(const Graph* graph) {
    if (!graph || graph->numVertices == 0) return NULL;
    int V = graph->numVertices;

    int maxEdges = graph->numEdges > 0 ? graph->numEdges : 10;
    Edge* edges = (Edge*)malloc(maxEdges * sizeof(Edge));
    if (!edges) return NULL;

    int edgeCount = 0;
    for (int u = 0; u < V; u++) {
        EdgeNode* curr = graph->matrix[u];
        while (curr != NULL) {
            if (u < curr->number) {
                if (edgeCount >= maxEdges) {
                    maxEdges *= 2;
                    edges = realloc(edges, maxEdges * sizeof(Edge));
                }
                edges[edgeCount].u = u;
                edges[edgeCount].v = curr->number;
                edges[edgeCount].weight = curr->weight;
                edgeCount++;
            }
            curr = curr->next;
        }
    }

    qsort(edges, edgeCount, sizeof(Edge), CompareEdges);

    MSTResult* result = InitMSTResult(V);
    DSU* dsu = DSUcreate(V);
    if (!result || !dsu) {
        FreeMSTResult(result);
        DSUFree(dsu);
        free(edges);
        return NULL;
    }

    for (int i = 0; i < edgeCount && result->numEdges < V - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;

        if (DSUFind(dsu, u) != DSUFind(dsu, v)) {
            int idx = result->numEdges;
            result->u[idx] = u;
            result->v[idx] = v;
            result->weights[idx] = edges[i].weight;
            result->totalWeight += edges[i].weight;
            result->numEdges++;
            DSUUnion(dsu, u, v);
        }
    }

    DSUFree(dsu);
    free(edges);

    if (V > 1 && result->numEdges != V - 1) {
        FreeMSTResult(result);
        return NULL;
    }
    return result;
}

MSTResult* prim(const Graph* graph) {
    if (!graph || graph->numVertices == 0) return NULL;
    int V = graph->numVertices;

    int* parent = (int*)malloc(V * sizeof(int));
    int* key = (int*)malloc(V * sizeof(int));
    int* inMST = (int*)calloc(V, sizeof(int));
    
    if (!parent || !key || !inMST) {
        free(parent); free(key); free(inMST);
        return NULL;
    }

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }

    key[0] = 0;

    for (int count = 0; count < V - 1; count++) {
        int minKey = INT_MAX, u = -1;

        for (int v = 0; v < V; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break;
        inMST[u] = 1;

        EdgeNode* curr = graph->matrix[u];
        while (curr != NULL) {
            int v = curr->number;
            int weight = curr->weight;

            if (!inMST[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
            curr = curr->next;
        }
    }

    MSTResult* result = InitMSTResult(V);
    if (!result) {
        free(parent); free(key); free(inMST);
        return NULL;
    }

    for (int i = 1; i < V; i++) {
        if (parent[i] != -1) {
            int idx = result->numEdges;
            result->u[idx] = parent[i];
            result->v[idx] = i;
            result->weights[idx] = key[i];
            result->totalWeight += key[i];
            result->numEdges++;
        }
    }

    free(parent); free(key); free(inMST);

    if (V > 1 && result->numEdges != V - 1) {
        FreeMSTResult(result);
        return NULL;
    }
    return result;
}
