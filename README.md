# GraphMaster

## Console Application for Working with Graphs

---

## About the Project

**GraphMaster** is a console application written in C for working with graphs. The program loads a graph from a text file and allows users to execute algorithms for finding shortest paths, building minimum spanning trees, and traversing graphs.

---

## Functionality

- Load graph from file (edge list with weights / adjacency matrix)
- Input file format validation
- Algorithm selection via console menu
- Shortest path search (Dijkstra's algorithm)
- Minimum spanning tree construction (Kruskal's and Prim's algorithms)
- MST algorithm performance comparison
- Breadth-first search (BFS) graph traversal
- Results output in a user-friendly format
- Input/output error handling and memory management

---

## Graph File Format

### Format 1 (Edge List):
```
5
Moscow
Saint-Petersburg
Novosibirsk
Yekaterinburg
Kazan
0 1 743
0 2 3245
1 3 2356
3 4 2422
```

### Format 2 (Adjacency Matrix):
```
5
Moscow
Saint-Petersburg
Novosibirsk
Yekaterinburg
Kazan
0 743 3245 0 0
743 0 0 2356 0
3245 0 0 0 1560
0 2356 0 0 2422
0 0 1560 2422 0
```


## Build and Run






