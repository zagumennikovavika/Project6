#include "backendGraph.h"

#define MAX_LINE 256
void* CheckFileToLoad(const char* filename);
/*
each vertex stores a linked list of neighbors,
so one edge node represents one connection
to another vertex with a specific weight
*/
EdgeNode* CreateEdgeNode(int num, int weight){
    EdgeNode* Node = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (Node == NULL) return NULL;

    Node -> number = num;
    Node -> weight = weight;
    Node -> next = NULL;

    return Node;
}

/*
the graph structure consists of numVertices,
the number of vertices, numEdges,
the number of edges, matrix, an array of edge lists,
and cityNames, an array of pointers
*/

/*
the graph uses an adjacency list representation:
matrix[i] stores the head of the linked list
for all neighbors of vertex i
*/
Graph* CreateGraph(int numVertices){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) return NULL;
    
    EdgeNode **matrix = calloc(numVertices, sizeof(EdgeNode*));
    if (matrix == NULL) return NULL;

    char **Names = calloc(numVertices, sizeof(char*));
    if (Names == NULL) return NULL;

    graph -> numVertices = numVertices;
    graph -> numEdges = 0;
    graph -> matrix = matrix;
    graph -> cityNames = Names;
    
    return graph;
}

/*
because the graph is undirected,
the edge must be added in both directions:
u -> v and v -> u
*/
void AddEdge(Graph* graph, int u, int v, int weight){
    graph -> numEdges += 1;

    EdgeNode* edge1 = CreateEdgeNode(v, weight);
    edge1 -> next = graph -> matrix[u];
    graph -> matrix[u] = edge1;

    EdgeNode* edge2 = CreateEdgeNode(u, weight);
    edge2 -> next = graph -> matrix[v];
    graph -> matrix[v] = edge2;
}  

/*
first all city names are loaded,
then all edges are read and connected
*/
Graph* LoadGraphEdges(const char* filename) {
    if (CheckFileToLoad(filename) == NULL) return NULL;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    int vertexCount, format;
    
    fscanf(file, "%d\n%d", &format, &vertexCount);
    fgetc(file);   

    Graph* graph = CreateGraph(vertexCount);
    if (graph == NULL) {
        fclose(file);
        return NULL;
    }

    char line[MAX_LINE];
    for (int i = 0; i < vertexCount; i++) {
        if (fgets(line, MAX_LINE, file) == NULL) break;
        line[strcspn(line, "\n")] = '\0';
        
        graph->cityNames[i] = malloc(strlen(line) + 1);
        if (graph->cityNames[i]) 
            strcpy(graph->cityNames[i], line);
        
    }

    int u, v, weight;
    while (fscanf(file, "%d %d %d", &u, &v, &weight) == 3) 
        AddEdge(graph, u, v, weight);
    

    fclose(file);
    return graph;
}

/*
the matrix format stores all connections in NxN form,
but only the upper triangle is used
to avoid duplicating undirected edges
*/
Graph* LoadGraphMatrix(const char* filename){
    if (CheckFileToLoad(filename) == NULL) return NULL;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    int vertexCount, format;
    char line[MAX_LINE];

    fscanf(file, "%d\n%d", &format, &vertexCount);
    fgetc(file);

    Graph* graph = CreateGraph(vertexCount);
    if (graph == NULL) {
        fclose(file);
        return NULL;
    }

    for(int i = 0; i < vertexCount; i++){
        fgets(line, MAX_LINE, file);
        line[strcspn(line, "\n")] = '\0';
        graph -> cityNames[i] = malloc(strlen(line) + 1);
        strcpy(graph->cityNames[i], line);
    }
    
    for(int i = 0; i < vertexCount; i++)
        for(int j = 0; j < vertexCount; j++){
            int weight;
            fscanf(file, "%d", &weight);

            if (weight != 0 && i < j)
                AddEdge(graph, i, j, weight);   
        }
        
    fclose(file);
    return graph;
}

int SaveGraphEdges(const Graph* graph, const char* filename){
    FILE* file = fopen(filename, "w");
    if (file == NULL) return ERROR;

    int numVertices = graph -> numVertices;
    int format = 1;

    fprintf(file, "%d\n%d\n", format, numVertices);

    for(int i = 0; i < numVertices; i++)
        fprintf(file, "%s\n", graph -> cityNames[i]);
    
    for(int i = 0; i < numVertices; i++){
        EdgeNode* edge = graph -> matrix[i];
    
        /*
        only edges where i < edge->number are written,
        otherwise every undirected edge would appear twice
        */
        while (edge != NULL){
            if (i < edge -> number)
                fprintf(file, "%d %d %d\n", i, edge -> number,
                                                edge -> weight);
        
            edge = edge -> next;
        }
    }

    fclose(file);
    return 0;
}

/*
recursive deletion walks through the entire linked list
before freeing nodes in reverse order
*/
void FreeAdgeNode(EdgeNode* node){
    if (node == NULL)
        return;
    
    FreeAdgeNode(node -> next);
    free(node);
}

void FreeGraph(Graph* graph){
    if(graph == NULL) return;

    for(int i = 0; i < graph -> numVertices; i++){
        FreeAdgeNode(graph->matrix[i]);
        free(graph->cityNames[i]);
    }

    free(graph->matrix);
    free(graph->cityNames);
    free(graph);
}

/*
the algorithm searches for the closest unvisited vertex,
which becomes the next step in Dijkstra's algorithm
*/
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

/*
distance[] stores the shortest known distance,
previous[] stores the previous vertex in the path,
visited[] prevents processing the same vertex twice
*/
DijkstraResult* Dijkstra(const Graph *graph, int start){
    int numVertices = graph -> numVertices;

    int *distance = malloc(numVertices * sizeof(int));
    int *visited  = malloc(numVertices * sizeof(int));
    int *previous = malloc(numVertices * sizeof(int));

    if (distance == NULL || visited == NULL || previous == NULL) {
        free(distance);
        free(visited);
        free(previous);
        return NULL;
    }

    for (int i = 0; i < numVertices; i++) {
        distance[i] = INT_MAX;
        visited[i]  = 0;
        previous[i] = -1;
    }

    distance[start] = 0;

    for (int i = 0; i < numVertices; i++){
        int u = FindMinDistance(distance, visited, numVertices);
        if(u == -1) break;

        visited[u] = 1;
        
        EdgeNode* node = graph -> matrix[u];

        while (node != NULL){
            int v = node -> number;
            int weight = node -> weight;

            /*
            every relaxation checks whether going through u
            creates a shorter path to vertex v
            */
            if (!visited[v] && distance[u] != INT_MAX
                && distance[u] + weight < distance[v]){
                distance[v] = distance[u] + weight;
                previous[v] = u;
            }

            node = node -> next;
        }

    }

    DijkstraResult* result = malloc(sizeof(DijkstraResult));
    if (result == NULL) {
        free(distance);
        free(visited);
        free(previous);
        return NULL;
    }

    result -> dist = distance;
    result -> prev = previous;
    result -> source = start;
    
    free(visited);  

    return result;
}

void PrintDijkstraResult(const DijkstraResult* result, const Graph* graph) {
    if (result == NULL || graph == NULL) return;

    printf("Кратчайшие расстояния от '%s':\n\n", 
           graph->cityNames[result->source]);

    for (int i = 0; i < graph->numVertices; i++) {
        printf("  %s: ", graph->cityNames[i]);
        
        if (result->dist[i] == INT_MAX) {
            printf("недостижим\n");
        } else {
            printf("%d\n", result->dist[i]);
        }
    }
}

void FreeDijkstraResult(DijkstraResult* result) {
    if (result == NULL) return;

    free(result->dist);
    free(result->prev);
    free(result);
}

int* GetPath(const DijkstraResult* result, const Graph* graph, int target, int *pathLen) {
    if (result == NULL || graph == NULL) return NULL;
    if (target < 0 || target >= graph -> numVertices) return NULL;
    if (result->dist[target] == INT_MAX) return NULL;

    /*
    the path is reconstructed backwards:
    target -> previous vertex -> ... -> source
    */
    int* path = (int*)malloc(graph -> numVertices * sizeof(int));
    if (path == NULL) return NULL;

    int pathLength = 0;
    int current = target;

    while (current != -1) {
        path[pathLength++] = current;
        current = result -> prev[current];
    }
    
    /*
    the path was built in reverse order,
    so it must be reversed to get source -> target
    */
    int l = 0, r = pathLength - 1;
    while (l < r) {

        int temp = path[l];
        path[l] = path[r];
        path[r] = temp;
        
        l++;
        r--;
    }
    
    path = realloc(path, pathLength * sizeof(int));
    *pathLen = pathLength;

    return path;
}

// Create an empty queue
Queue* CreateQueue() {
    Queue* q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;

    q -> head = NULL;
    q -> rear = NULL;

    return q;
}

// Add an element to the end of the queue
void Enqueue(Queue* q, int value) {
    if (q == NULL) return;

    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) return;

    newNode->number = value;
    newNode->next = NULL;

    if (q -> rear == NULL) {
        // The queue was empty — the new node is both head and tail
        q -> head = newNode;
        q -> rear = newNode;
    } else {
        // Attach to the tail and update the tail
        q -> rear->next = newNode;
        q -> rear = newNode;
    }
}

// Remove an element from the beginning of the queue
int Dequeue(Queue* q) {
    if (q == NULL || q -> head == NULL) return -1;
    
    Node* tmp = q -> head;
    int value = tmp->number;

    q -> head = q -> head -> next;

    if (q -> head == NULL) 
        q -> rear = NULL;  // The queue became empty
    

    free(tmp);
    return value;
}

// Check if the queue is empty (1 — empty, 0 — not empty)
int IsEmpty(Queue* q) {
    if (q == NULL) return 1;
    return q -> head == NULL;
}

// Free the entire queue
void FreeQueue(Queue* q) {
    if (q == NULL) return;

    while (q -> head != NULL) {
        Node* tmp = q -> head;
        q -> head = q -> head -> next;
        free(tmp);
    }

    free(q);
}

/*
BFS explores vertices level by level,
using a queue to process vertices
in the order they were discovered
*/
int* BFS(const Graph* graph, int start, int* orderlen) {
    if (graph == NULL || orderlen == NULL) return NULL;

    int V = graph -> numVertices;

    int* order = malloc(V * sizeof(int));
    if (order == NULL) return NULL;

    int* visited = calloc(V, sizeof(int));

    Queue* q = CreateQueue();

    visited[start] = 1;
    Enqueue(q, start);

    int orderIndex = 0;

    while (!IsEmpty(q)) {
        int u = Dequeue(q);
        order[orderIndex++] = u;

        EdgeNode* edge = graph->matrix[u];
        while (edge != NULL) {
            int v = edge->number;

            /*
            a vertex is marked visited immediately after insertion,
            otherwise the same vertex could be added multiple times
            */
            if (!visited[v]) {
                visited[v] = 1;
                Enqueue(q, v);
            }

            edge = edge -> next;
        }
    }

    *orderlen = orderIndex;

    free(visited);
    FreeQueue(q);

    return order;
}

/*
the validator first checks whether the file header
contains the correct format and vertex count
*/
void* CheckFileToLoad(const char* filename){
    int format, count;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    if (fscanf(file, "%d\n%d", &format, &count) != 2){
        fclose(file);
        return NULL;
    }
    fgetc(file);
    
    char line[MAX_LINE];

    // Save the position before reading the line
    long pos = ftell(file);

    fgets(line, MAX_LINE, file);

    int score = 0;

    /*
    city names are expected before graph data,
    so the function counts text lines first
    */
    while (isalpha(line[0]) != 0){
        score++;

        // Update the position before reading the next line
        pos = ftell(file);
        
        if (fgets(line, MAX_LINE, file) == NULL) 
            break;
    }

    if (score != count){
        fclose(file);
        return NULL;
    }

    /*
    after counting city names,
    the file pointer is returned to the start
    of the graph data section
    */
    fseek(file, pos, SEEK_SET);

    /*
    format 1 expects edge list representation:
    u v weight
    */
    if (format == 1){
        int u, v, weight, charN;
           
        while (fgets(line, MAX_LINE, file) != NULL) {
            char extra;
            line[strcspn(line, "\n")] = 0;

            /*
            %n stores the position after the last parsed number,
            which helps detect malformed lines
            */
            if (sscanf(line, "%d %d %d %n", &u, &v, &weight, &charN) != 3) {
                fclose(file);
                return NULL;
            }

            // There must be nothing after the three numbers
            if (sscanf(line, "%d %d %d %c", &u, &v, &weight, &extra) == 4) {
                fclose(file);
                return NULL;
            }
        }
        
        fclose(file);
        return (void*)1;
    }

    /*
    format 2 expects a square adjacency matrix,
    so every row must contain exactly count numbers
    */
    else if (format == 2){
        int rowCount = 0;

        while (fgets(line, MAX_LINE, file) != NULL) {

            int colCount = 0;

            char* ptr = line;
            char* end;

            while (1) {

                // Skip spaces
                while (isspace(*ptr))
                    ptr++;

                // End of line
                if (*ptr == '\0' || *ptr == '\n')
                    break;

                /*
                strtol moves the end pointer after the parsed number,
                allowing the line to be scanned step by step
                */
                strtol(ptr, &end, 10);

                // Number was not found
                if (ptr == end) {
                    fclose(file);
                    return NULL;
                }

                colCount++;

                ptr = end;
            }

            // Check the number of values
            if (colCount != count) {
                fclose(file);
                return NULL;
            }

            rowCount++;
        }

        // Check the number of rows
        if (rowCount != count) {
            fclose(file);
            return NULL;
        }

    fclose(file);
    return (void*)1;
    }

    fclose(file);
    return NULL;
}