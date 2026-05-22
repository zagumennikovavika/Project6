#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE 256
#define ERROR (-1)


// Structure for adjacency list node
typedef struct {
    int number;
    int weight;
    struct EdgeNode *next;
}EdgeNode;

// Graph structure
typedef struct {
    int numVertices;
    int numEdges;
    EdgeNode**matrix;
    char **cityNames;
}Graph;

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
    long pos = ftell(file);  // Запоминаем позицию ПЕРЕД чтением строки

    fgets(line, MAX_LINE, file);

    int score = 0;

    while (isalpha(line[0]) != 0){
        score++;
        pos = ftell(file);  // Обновляем позицию перед чтением следующей
        
        if (fgets(line, MAX_LINE, file) == NULL) 
            break;
    }

    if (score != count){
        fclose(file);
        return NULL;
    }

    // Теперь нужно вернуться на строку с цифрами
    fseek(file, pos, SEEK_SET);

    if (format == 1){
        int u, v, weight, charN;
           
        while (fgets(line, MAX_LINE, file) != NULL) {
            char extra;
            line[strcspn(line, "\n")] = 0;

            if (sscanf(line, "%d %d %d %n", &u, &v, &weight, &charN) != 3) {
                fclose(file);
                return NULL;
            }

            // После трех чисел ничего быть не должно
            if (sscanf(line, "%d %d %d %c", &u, &v, &weight, &extra) == 4) {
                // Есть лишний символ
                fclose(file);
                return NULL;
            }

            printf("Прочитано: %d %d %d\n", u, v, weight);
        }
    }
    else if (format == 2){

        char line[MAX_LINE];
        int rowCount = 0;

        while (fgets(line, MAX_LINE, file) != NULL) {

            int colCount = 0;

            char* ptr = line;
            char* end;

            while (1) {

                // пропускаем пробелы
                while (isspace(*ptr))
                    ptr++;

                // конец строки
                if (*ptr == '\0' || *ptr == '\n')
                    break;

                // читаем число
                strtol(ptr, &end, 10);

                // если число не найдено
                if (ptr == end) {
                    fclose(file);
                    return NULL;
                }

                colCount++;

                ptr = end;
            }

            // проверка количества чисел
            if (colCount != count) {
                fclose(file);
                return NULL;
            }

            rowCount++;
        }

        // проверка количества строк
        if (rowCount != count) {
            fclose(file);
            return NULL;
        }

        fclose(file);
        return;
    }
    return NULL;
}


Graph* LoadGraphEdges(const char* filename) {
    if (CheckFileToLoad(filename) == NULL) return NULL;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    int vertexCount;
    
    fscanf(file, "%d", &vertexCount);
    fgetc(file);  
    fscanf(file, "%d", &vertexCount);
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

Graph* LoadGraphMatrix(const char* filename){
    if (CheckFileToLoad(filename) == NULL) return NULL;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    int vertexCount;
    char line[MAX_LINE];

    //skip the first character of the file type
    fscanf(file, "%d", &vertexCount);
    fgetc(file);

    //reading the number of nodes
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
    int format = 1;

    fprintf(file, "%d\n%d\n", format, numVertices);

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

int main(){
    Graph* g = LoadGraphEdges("g1.txt");

    SaveGraphEdges(g, "t.txt");

    return 0;
}