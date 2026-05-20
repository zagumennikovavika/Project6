#ifndef DSU_H
#define DSU_H

typedef struct {
    int parent;
    int rank;
} DSUElement;

typedef struct {
    DSUElement* elements;
    int n;
} DSU;

DSU* DSUcreate(int n);
void DSUFree(DSU* dsu);
int DSUFind(DSU* dsu, int x);
void DSUUnion(DSU* dsu, int x, int y);

#endif