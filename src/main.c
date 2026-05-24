#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  
#include "front.h"
#include "mst_algorithms.h" 
#include "mst_compare.h"
#include "backendGraph.h"  
#include "front.h"         
#include "common.h" 
#include "dsu.h"

int GetFormat(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;
    
    int format;
    fscanf(f, "%d", &format);
    fclose(f);
    
    if (format == 1 || format == 2) {
        return format;
    }
    return 0; 
}

int AskToContinue() {
    char answer;
    printf("\nDo you want to return to the main menu? (y/n): ");
    scanf(" %c", &answer);
    { int c; while ((c = getchar()) != '\n' && c != EOF); } 
    
    if (answer == 'y' || answer == 'Y') {
        return 1;  
    } else {
        return 0;  
    }
}
int main() {
    Graph* g = NULL;
    int number;                     
    
    do {
        number = Menu();           
        
        switch(number) {
            case 1: // Graph Loading
            {
                char* filename = AskFilename();
                int format = GetFormat(filename);  
                
                if (g) {
                    FreeGraph(g);
                    g = NULL;
                }

                if (format == 1) {
                    g = LoadGraphEdges(filename);
                } else if (format == 2) {
                    g = LoadGraphMatrix(filename);
                } else {
                    printf("Unknown file format\n");
                }
        

                // If it didn't load, then we take the default one.
                if (!g) {
                    printf("The file could not be uploaded. I use the default graph.\n");
                    g = LoadGraphEdges("tests/Graphs/default1.txt");
                    if (!g) {
                        g = LoadGraphMatrix("tests/Graphs/default2.txt");
                    }
                }

                if (g) {
                    printf("The graph has been uploaded successfully!\n");
                    PrintGraphInfo(g->numVertices, g->numEdges);
                }
                free(filename);
                
                if (!AskToContinue()) {
                    number = 0;  
                }
                break;
                
            }
            
            case 2: // Dijkstra
            {
                if (!g) {
                    printf("Download the file first (step 1)\n");
                    break;
                }
                // Showing a list of cities
                printf("\nAvailable cities:\n");
                for (int i = 0; i < g->numVertices; i++) {   
                    printf("  %d - %s\n", i, g->cityNames[i]);
                }
 
                int start, end;
                printf("Enter the starting vertex (0..%d): ", g->numVertices - 1);
                scanf("%d", &start);
                printf("Enter the end vertex (0..%d): ", g->numVertices - 1);
                scanf("%d", &end);
 
                if (start < 0 || start >= g->numVertices || end < 0 || end >= g->numVertices) {
                    printf("Error: incorrect vertex number\n");
                    break;
                }
                
                double startTime = GetTime();
                DijkstraResult* res = Dijkstra(g, start);
                double endTime = GetTime();
                double timeMS = (endTime - startTime) * 1000.0;
 
                if (!res) {
                    printf("Dijkstra algorithm execution error\n");
                    break;
                }
 
 
                // Checking if there is a path
                if (res->dist[end] == INT_MAX) {
                    printf("There is no way\n");
                } else {
                    int pathLen = 0;
                    int* path = GetPath(res, g, end, &pathLen); 
        
                    if (path == NULL || pathLen <= 0) {
                        printf("Mistake: couldn't restore the path\n");
                    } else {
                        PrintDijkstra(g, path, pathLen, res->dist[end], timeMS);
                        free(path);
                    }
                }

 
                FreeDijkstraResult(res);
                if (!AskToContinue()) {
                    number = 0;  
                }
                break;
            }

            case 3: // Kruskal
            {
                if (!g) {
                    printf("Download the file first (step 1)\n");
                    break;
                }
                
                MSTResult* res = kruskal(g);
                if (res == NULL) {
                    printf("It is impossible to build an MST, the graph is disconnected\n");
                } else {
                    PrintMST(res, g);
                    FreeMSTResult(res);
                }
                if (!AskToContinue()) {
                    number = 0;  
                }
                break;
            }
            
            case 4: // Prim
            {
                if (!g) {
                    printf("Download the file first (step 1)\n");
                    break;
                }
                
                MSTResult* res = prim(g);
                if (res == NULL) {
                    printf("It is impossible to build an MST, the graph is disconnected\n");
                } else {
                    PrintMST(res, g);
                    FreeMSTResult(res);
                }
                if (!AskToContinue()) {
                    number = 0;  
                }
                break;
            }
            
            case 5: // BFS
            {
                if (!g) {
                    printf("Download the file first (step 1)\n");
                    break;
                }

                printf("\nAvailable cities:\n");
                for (int i = 0; i < g->numVertices; i++) {
                    printf("  %d - %s\n", i, g->cityNames[i]);
                }

                int start;
                printf("Enter the starting vertex (0..%d): ", g->numVertices - 1);
                scanf("%d", &start);
                { int c; while ((c = getchar()) != '\n' && c != EOF); }

                if (start < 0 || start >= g->numVertices) {
                    printf("Error: incorrect vertex number\n");
                    break;
                }

                int orderLen; 
                
                
                double startT = GetTime();
                int* order = BFS(g, start, &orderLen);   
                double endT = GetTime();
                double time2MS = (endT - startT) * 1000.0;

                if (order == NULL || orderLen == 0) {
                    printf("Error: Bypass failed\n");
                } else {
                    PrintBFS(g, order, orderLen, time2MS);
                    free(order);
                }
                if (!AskToContinue()) {
                    number = 0;  
                }
                break;
            }
            
            case 6: // Comparison of MST algorithms
            {
                if (!g) {
                    printf("Download the file first (step 1)\n");
                    break;
                }

                Comparison cmp = CompareMST(g);  
                PrintComparison(&cmp);     

                if (!AskToContinue()) {
                    number = 0;  
                }                  
                break;
            }
            
            case 0: // Exit
            {
                printf("Goodbye! The program is completed.\n");
                break;
            }
            
        }
    } while (number != 0);   
    
    if (g) {
        FreeGraph(g);
    }
    
    return 0;
} 