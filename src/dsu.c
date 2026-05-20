#include <stdlib.h>
#include "dsu.h"

DSU* DSUcreate(int n) {
    DSU* dsu = (DSU*)malloc(sizeof(DSU));
    if (!dsu) return NULL;

    dsu->elements = (DSUElement*)malloc(n * sizeof(DSUElement));
    if (!dsu->elements) {
        free(dsu);
        return NULL;
    }

    dsu->n = n;
    for (int i = 0; i < n; i++) {
        dsu->elements[i].parent = i;
        dsu->elements[i].rank = 0;
    }
    return dsu;
}

void DSUFree(DSU* dsu) {
    if (dsu == NULL) return;
    free(dsu->elements);
    free(dsu);
}

int DSUFind(DSU* dsu, int x) {
    if (dsu->elements[x].parent != x) {
        dsu->elements[x].parent = DSUFind(dsu, dsu->elements[x].parent);
    }
    return dsu->elements[x].parent;
}

void DSUUnion(DSU* dsu, int x, int y) {
    int rootX = DSUFind(dsu, x);
    int rootY = DSUFind(dsu, y);

    if (rootX != rootY) {
        if (dsu->elements[rootX].rank < dsu->elements[rootY].rank) {
            dsu->elements[rootX].parent = rootY;
        } else if (dsu->elements[rootX].rank > dsu->elements[rootY].rank) {
            dsu->elements[rootY].parent = rootX;
        } else {
            dsu->elements[rootY].parent = rootX;
            dsu->elements[rootX].rank++;
        }
    }
}
