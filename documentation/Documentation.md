## GraphMaster - is a console application written in C for graph analysis, shortest path finding, and minimum spanning tree construction.
### Main Menu
```
1) Load graph from file
2) Find the shortest path between points (Dijkstra's Algorithm)
3) Build an MST (Kruskal's Algorithm)
4) Build an MST (Prim's Algorithm)
5) Breadth-first search (BFS)
6) Comparison of MST construction algorithms
0) Exit
 ```
###  Data Structures
##### Graph
```c
typedef struct {
    int numVertices;      // Number of vertices
    int numEdges;         // Number of edges
    EdgeNode** matrix;    // Adjacency list array
    char** cityNames;     // Vertex names (cities)
} Graph;
```
##### DijkstraResult
```c
typedef struct {
    int* dist;           // Distance array from source
    int* prev;           // Previous vertices array
    int source;          // Source vertex
} DijkstraResult;
```
##### EdgeNode
```c
typedef struct EdgeNode {
    int number;           // Adjacent vertex number
    int weight;           // Edge weight
    struct EdgeNode* next; // Pointer to next edge
} EdgeNode;
```
##### Edge
```c
typedef struct {
    int u, v, weight;
} Edge;
```
##### MSTResult
```c
typedef struct {
    int* u;              // Array of starting vertices of edges
    int* v;              // Array of ending vertices of edges
    int* weights;        // Array of edge weights
    int totalWeight;     // Total weight of MST
    int numEdges;        // Number of edges in MST
} MSTResult;

```
### Algorithms

### Dijkstra's Algorithm

Finds the shortest paths from a single source vertex to all other vertices in a weighted graph with non-negative edge weights.

##### Working Principle

###### 1) Initialize distances to all vertices as INFINITY, distance to source as 0
###### 2) Create a visited vertices array (all false)
###### 3) At each iteration: Find the unvisited vertex with minimum distance, mark it as visited, update distances for all its neighbors if the new path is shorter
###### 4)  Repeat until all vertices are visited
##### Implementation
```c 
DijkstraResult* dijkstra(const Graph *graph, int start){
    int numVertices = graph -> numVertices;
    
    // Arrays for distances, visited vertices, and previous vertices
    int *distance = malloc(numVertices * sizeof(int));
    int *visited  = malloc(numVertices * sizeof(int));
    int *previous = malloc(numVertices * sizeof(int));
    
    // Initialization
    for (int i = 0; i < numVertices; i++) {
        distance[i] = INT_MAX;  // All distances = infinity
        visited[i]  = 0;         // No vertex visited
        previous[i] = -1;        // No previous vertices
    }
    distance[start] = 0;         // Distance to start = 0
    
    // Main algorithm loop
    for (int i = 0; i < numVertices; i++){
        // Find unvisited vertex with minimum distance
        int u = FindMinDistance(distance, visited, numVertices);
        if(u == -1) break;  // Graph may be disconnected
        
        visited[u] = 1;      // Mark as visited
        
        // Relax all edges from vertex u
        EdgeNode* node = graph -> matrix[u];
        while (node != NULL){
            int v = node -> number;
            int weight = node -> weight;
            
            // If found a shorter path through u
            if (!visited[v] && distance[u] != INT_MAX
                && distance[u] + weight < distance[v]){
                distance[v] = distance[u] + weight;
                previous[v] = u;
            }
            node = node -> next;
        }
    }
    
    // Save result
    DijkstraResult* result = malloc(sizeof(DijkstraResult));
    result -> dist = distance;
    result -> prev = previous;
    result -> source = start;
    free(visited);
    
    return result;
}
```
##### Helper function to find minimum distance
```c
int FindMinDistance(int *distance, int *visited, int numVertices){
    int minDistance = INT_MAX;
    int minVertex = -1;
    
    for(int i = 0; i < numVertices; i++)
        if(visited[i] == 0 && distance[i] < minDistance){
            minDistance = distance[i];
            minVertex = i;
        }
    return minVertex;
}
```
### Kruskal's Algorithm
Builds a minimum spanning tree (MST) - a connected subgraph that includes all vertices with the minimum sum of edge weights.
##### Working Principle
###### 1) Collect all graph edges into an array
###### 2) Sort edges by ascending weight
###### 3) Initialize a disjoint set union (DSU) structure
###### 4) Sequentially iterate through sorted edges: (If an edge connects vertices from different components, add it to the MST, merge components using DSU)
###### 5)  Repeat until V-1 edges are collected

##### Implementation
```c 
MSTResult* Kruskal(const Graph* graph) {
    int V = graph->numVertices;
    
    // 1. Collect all edges into an array
    Edge* edges = (Edge*)malloc(maxEdges * sizeof(Edge));
    int edgeCount = 0;
    
    for (int u = 0; u < V; u++) {
        EdgeNode* curr = graph->matrix[u];
        while (curr != NULL) {
            if (u < curr->number) {  // Add each edge once
                edges[edgeCount].u = u;
                edges[edgeCount].v = curr->number;
                edges[edgeCount].weight = curr->weight;
                edgeCount++;
            }
            curr = curr->next;
        }
    }
    
    // 2. Sort edges by weight
    qsort(edges, edgeCount, sizeof(Edge), CompareEdges);
    
    // 3. Initialize DSU
    MSTResult* result = InitMSTResult(V);
    DSU* dsu = DSUcreate(V);
    
    // 4. Main loop - MST construction
    for (int i = 0; i < edgeCount && result->numEdges < V - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        
        // Check if edge would create a cycle
        if (DSUFind(dsu, u) != DSUFind(dsu, v)) {
            // Add edge to MST
            int idx = result->numEdges;
            result->u[idx] = u;
            result->v[idx] = v;
            result->weights[idx] = edges[i].weight;
            result->totalWeight += edges[i].weight;
            result->numEdges++;
            
            // Merge components
            DSUUnion(dsu, u, v);
        }
    }
    
    return result;
}
```
### Breadth-First Search (BFS)

Traverses the graph level by level, visiting all neighboring vertices first, then neighbors of neighbors, etc.

##### Working Principle

###### 1) Create a queue and a visited vertices array
###### 2) Put the starting vertex in the queue and mark it as visited
###### 3) While queue is not empty: Dequeue a vertex from the front, process it (add to result), add all unvisited neighbors to the end of the queue
###### 4) Repeat until all reachable vertices are processed

##### Implementation
```c 
int* BFS(const Graph* graph, int* orderlen, int start) {
    int V = graph->numVertices;
    
    // Array for result (traversal order)
    int* order = malloc(V * sizeof(int));
    int* visited = calloc(V, sizeof(int));  // Visited array
    
    // Create a queue
    Queue* q = CreateQueue();
    
    // Start with the starting vertex
    visited[start] = 1;
    Enqueue(q, start);
    
    int orderIndex = 0;
    
    while (!IsEmpty(q)) {
        // Dequeue a vertex from the queue
        int u = Dequeue(q);
        order[orderIndex++] = u;  // Write to result
        
        // Add all unvisited neighbors
        EdgeNode* edge = graph->matrix[u];
        while (edge != NULL) {
            int v = edge->number;
            
            if (!visited[v]) {
                visited[v] = 1;
                Enqueue(q, v);  // Put at the end of the queue
            }
            edge = edge->next;
        }
    }
    
    *orderlen = orderIndex;  // Number of vertices traversed
    free(visited);
    FreeQueue(q);
    
    return order;
}
```
##### Queue Implementation
```c 
typedef struct Node {
    int number;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* head;
    Node* rear;
} Queue;

void Enqueue(Queue* q, int value) {
    Node* newNode = malloc(sizeof(Node));
    newNode->number = value;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->head = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int Dequeue(Queue* q) {
    Node* tmp = q->head;
    int value = tmp->number;
    q->head = q->head->next;
    
    if (q->head == NULL) 
        q->rear = NULL;
    
    free(tmp);
    return value;
}
```

### Prim's Algorithm

Builds a minimum spanning tree (MST) - a connected subgraph that includes all vertices with the minimum sum of edge weights, but in a different way - "growing" the tree from a single vertex.

##### Working Principle

###### 1) Choose an arbitrary starting vertex (usually 0)
###### 2) Create arrays: (key[] - minimum edge weight to connect a vertex to the tree, parent[] - parent vertex in the MST, inMST[] - flag indicating if vertex is in the tree)
###### 3) Initialize key[0] = 0, remaining key[] = INF
###### 4) Repeat V-1 times: (Select vertex with minimum key[] among those not yet added, add it to MST, update key[] for all its neighbors)
###### 5) Build the result from the parent[] array

##### Implementation

```c 
MSTResult* prim(const Graph* graph) {
    int V = graph->numVertices;
    
    // Auxiliary arrays
    int* parent = (int*)malloc(V * sizeof(int));   // Parents in MST
    int* key = (int*)malloc(V * sizeof(int));      // Minimum weights
    int* inMST = (int*)calloc(V, sizeof(int));     // Added flags
    
    // Initialization
    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    key[0] = 0;  // Start with vertex 0
    
    // Main loop - build MST from V-1 edges
    for (int count = 0; count < V - 1; count++) {
        // Find unvisited vertex with minimum key
        int minKey = INT_MAX, u = -1;
        for (int v = 0; v < V; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }
        
        if (u == -1) break;  // Graph is disconnected
        inMST[u] = 1;         // Add vertex to MST
        
        // Update keys for neighbors
        EdgeNode* curr = graph->matrix[u];
        while (curr != NULL) {
            int v = curr->number;
            int weight = curr->weight;
            
            // If neighbor not in MST and weight is less than current key
            if (!inMST[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
            }
            curr = curr->next;
        }
    }
    
    // Build result from parent[] and key[]
    MSTResult* result = InitMSTResult(V);
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
    
    return result;
}
```
## Graph File Format

### Format 1 (Edge List):
```
1
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
2
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

