#include "backendGraph.h"

EdgeNode* CreateEdgeNode(int num, int weight){
    EdgeNode* Node = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (Node == NULL) return NULL;

    Node -> number = num;
    Node -> weight = weight;
    Node -> next = NULL;

    return Node;
}

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

void AddEdge(Graph* graph, int u, int v, int weight){
    graph -> numEdges += 1;

    EdgeNode* edge1 = CreateEdgeNode(v, weight);
    edge1 -> next = graph -> matrix[u];
    graph -> matrix[u] = edge1;

    EdgeNode* edge2 = CreateEdgeNode(u, weight);
    edge2 -> next = graph -> matrix[v];
    graph -> matrix[v] = edge2;
}  

Graph* LoadGraphEdges(const char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    int vertexCount;
    char line[MAX_LINE];

    fscanf(file, "%d\n", &vertexCount);
    
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
    
    int u, v , weight;

    while (fscanf(file, "%d %d %d", &u, &v, &weight) == 3)
        AddEdge(graph, u, v, weight);
    
    fclose(file);
    return graph;
}

Graph* LoadGraphMatrix(const char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    int vertexCount;
    char line[MAX_LINE];

    fscanf(file, "%d", &vertexCount);
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
    
    fprintf(file, "%d\n", numVertices);

    for(int i = 0; i < numVertices; i++)
        fprintf(file, "%s\n", graph -> cityNames[i]);
    
    for(int i = 0; i < numVertices; i++){
        EdgeNode* edge = graph -> matrix[i];
    
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
    

    // Собираем путь в обратном порядке
    int* path = (int*)malloc(graph -> numVertices * sizeof(int));
    if (path == NULL) return NULL;

    int pathLength = 0;
    int current = target;

    while (current != -1) {
        path[pathLength++] = current;
        current = result -> prev[current];
    }
    
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

// Создание пустой очереди
Queue* CreateQueue() {
    Queue* q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;

    q -> head = NULL;
    q -> rear = NULL;

    return q;
}

// Добавление элемента в конец очереди
void Enqueue(Queue* q, int value) {
    if (q == NULL) return;

    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) return;

    newNode->number = value;
    newNode->next = NULL;

    if (q -> rear == NULL) {
        // Очередь была пуста — новый узел и голова, и хвост
        q -> head = newNode;
        q -> rear = newNode;
    } else {
        // Прицепляем к хвосту и обновляем хвост
        q -> rear->next = newNode;
        q -> rear = newNode;
    }
}

// Удаление элемента из начала очереди
int Dequeue(Queue* q) {
    if (q == NULL || q -> head == NULL) return -1;
    

    Node* tmp = q -> head;
    int value = tmp->number;

    q -> head = q -> head -> next;

    if (q -> head == NULL) 
        q -> rear = NULL;  // Очередь стала пустой
    

    free(tmp);
    return value;
}

// Проверка на пустоту (1 — пуста, 0 — не пуста)
int IsEmpty(Queue* q) {
    if (q == NULL) return 1;
    return q -> head == NULL;
}

// Очистка всей очереди
void FreeQueue(Queue* q) {
    if (q == NULL) return;

    while (q -> head != NULL) {
        Node* tmp = q -> head;
        q -> head = q -> head -> next;
        free(tmp);
    }

    free(q);
}

int* BFS(const Graph* graph, int start, int* orderlen) {
    if (graph == NULL || orderlen == NULL) return NULL;

    int V = graph -> numVertices;

    // Массив для результата (порядок обхода)
    int* order = malloc(V * sizeof(int));
    if (order == NULL) return NULL;

    // Массив visited (чтобы не зациклиться)
    int* visited = calloc(V, sizeof(int));

    // Очередь
    Queue* q = CreateQueue();

    // Начинаем со стартовой вершины
    visited[start] = 1;
    Enqueue(q, start);

    int orderIndex = 0;

    while (!IsEmpty(q)) {
        int u = Dequeue(q);
        order[orderIndex++] = u;   // записываем вершину в результат

        // Все соседи u
        EdgeNode* edge = graph->matrix[u];
        while (edge != NULL) {
            int v = edge->number;

            if (!visited[v]) {
                visited[v] = 1;
                Enqueue(q, v);
            }

            edge = edge -> next;
        }
    }

    *orderlen = orderIndex;   // сколько вершин обошли

    free(visited);
    FreeQueue(q);

    return order;
}
