#include "front.h"

// Function to request file path from user
char* AskFilename() {

    // Allocate memory for pointer to file path string
    char *path = (char*)malloc(MAX_PATH * sizeof(char));

    if (path == NULL) {
        printf("Memory error\n");
        return NULL;
    }

    printf("Enter the path to the file with the graph\n");

    // Read string from console
    if (fgets(path, MAX_PATH, stdin) == NULL) {
        printf("Input error\n");
        free(path);
        return NULL;
    }

    // Remove newline character if present,
    // and replace with \0 so fopen can open the file
    size_t len = strlen(path);
    if (len > 0 && *(path + len -1) == '\n') {
        *(path + len - 1) = '\0';
    }

    // Check if file exists and is accessible
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        free(path);
        return NULL;
    }

    fclose(file);
    // Return pointer to file path string
    return path;
}

// Function to display main menu and get user selection
int Menu() {

    // Selected menu item number
    int number = 0;

    // Input success flag
    int flag = 0;
    printf("==============================================================\n");
    printf("GraphMaster: Route planner and network analyzer\n\n");
    printf("==============================================================\n");

    printf("Main menu:\n");
    printf("1) Load graph from file\n");
    printf("2) Find the shortest path between points"
           " (Dijkstra's Algorithm)\n");
    printf("3) Build an MST (Kruskal's Algorithm)\n");
    printf("4) Build an MST (Prim's Algorithm)\n");
    printf("5) Breadth-first search (BFS)\n");
    printf("6) Comparison of MST construction algorithms\n");
    printf("0) Exiting the program\n");
    printf("Enter the number of the menu item you selected:\n");
    do {

        // Buffer for input string
        char buffer[100];

        // Read string from console
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            return ERROR;
        }

        // Validate input:
        // - length should be 2 (digit + \n)
        // - sscanf should successfully read the number
        // - number should be from 1 to 6

        if ((strlen(buffer) != 2) || (sscanf(buffer, "%d",
            &number) != 1)  ||
            (number != 0 && number != 1 && number != 2 && number != 3 &&
                number != 4 && number != 5 && number != 6)) {

            printf("Input error\n");
            printf("Enter one integer from 1 to 6 without spaces"
                   " or other symbols:\n\n");
                }

        // Input is correct, exit loop
        else
            flag = 1;


    }
    while (flag == 0);
    return number;

} 


// Function to display algorithm comparison table
void PrintComparison(const Comparison* cmp) {

    printf("==============================================================\n");

    printf("                    ALGORITHM COMPARISON                     \n");

    printf("==============================================================\n");



    printf("| %-20s | %-15s | %-15s |\n", "Parameter", "Kruskal", "Prim");
    printf("------------------------------------------------------------\n");


    printf("| %-20s | %-15.6f | %-15.6f |\n",
           "Time (seconds)", cmp->timeKruskal, cmp->timePrim);


    printf("| %-20s | %-15d | %-15d |\n",
           "MST Weight", cmp->KruskalWeight, cmp->PrimWeight);


    printf("| %-20s | %-15d | %-15d |\n",
           "Number of edges", cmp->numEdges, cmp->numEdges);

    printf("------------------------------------------------------------\n");
    
}

// Function to display basic graph information
void PrintGraphInfo(int numVertices, int numEdges) {
    printf("You have uploaded the graph.\n");
    printf("---------------------\n");
    printf("| Vertces | Egges   |\n");
    printf("---------------------\n");
    printf("| %-7d | %-7d |\n", numVertices, numEdges);
    printf("---------------------\n");

} 



// Function to display Breadth-First Search (BFS) traversal result
void PrintBFS(Graph *g, int *order, int orderLen, double timeMS) {

    printf("\n========================================================\n");
    printf("\nVertex traversal order (BFS):\n");
    printf("\n========================================================\n");


    // Display vertex traversal order
    for (int i = 0; i < orderLen; i++) {
        int vertexIndex = order[i];

        // Check index validity and presence of city name
        if (vertexIndex >= 0 && vertexIndex < g->numVertices && 
            g->cityNames[vertexIndex] != NULL) {
            printf("%s [%d]", g->cityNames[vertexIndex], vertexIndex);
        } else {
            printf("[vertex %d]", vertexIndex);
        }


        if (i < orderLen - 1) {
            printf(" -> ");
        }
    }

    printf("\n\nCities visited: %d\n", orderLen);

    // Display execution time
    printf("Time: %.3f seconds\n", timeMS);

    
}

// Function to display Minimum Spanning Tree (MST)
void PrintMST(const MSTResult *res, Graph *g) {


    printf("\n========================================================\n");
    printf("Minimum spanning tree (MST):\n");
    printf("\n========================================================\n");
    printf("Edges of MST:\n");


    // Display all MST edges
    for (int i = 0; i < res->numEdges; i++) {

        // Check if city names exist
        if (g != NULL && g->cityNames != NULL &&
            res->u[i] < g->numVertices && res->v[i] < g->numVertices &&
            g->cityNames[res->u[i]] != NULL && g->cityNames[res->v[i]] != NULL) {

            printf("%s (%d) - %s (%d)\t %d units\n",
                   g->cityNames[res->u[i]], res->u[i],
                   g->cityNames[res->v[i]], res->v[i],
                   res->weights[i]);
            } else {
                // If no names available, display only vertex numbers
                printf("Vertex %d - Vertex %d\t %d units\n",
                       res->u[i], res->v[i], res->weights[i]);
            }
    }

    printf("--------------------------------------------------------\n");
    printf("Total weight of the tree: %d\n", res->totalWeight);
    printf("Number of edges in MST: %d\n", res->numEdges);
    
}

// Function to display Dijkstra's algorithm result
// (shortest path)
void PrintDijkstra(Graph *g, const int *path, int pathLen,
    int totalWeight, double timeMS) {
    if (g == NULL || path == NULL || pathLen <= 0) {
        printf("Error: Invalid path data\n");
        return;
    }
    printf("\n========================================================\n");
    printf("\nShortest path:\n");
    printf("\n========================================================\n");

    // Display path in "City -> City -> ..." format
    for (int i = 0; i < pathLen; i++) {
        int vertexIndex = path[i];

        // Check index validity and presence of city name
        if (vertexIndex >= 0 && vertexIndex < g->numVertices &&
            g->cityNames[vertexIndex] != NULL) {
            printf("%s", g->cityNames[vertexIndex]);
            } else {
                printf("[Vertex %d]", vertexIndex);
            }


        if (i < pathLen - 1) {
            printf(" -> ");
        }
    }

    printf("\n\nPath length: %d units\n", totalWeight);

    // Display execution time
    printf("Execution time: %.3f seconds\n", timeMS);
    
}

void WaitForUser() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');  
    getchar();  
}