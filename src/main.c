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

int main() {
    Graph* g = NULL;
    int number;                     
    
    do {
        number = Menu();           
        
        switch(number) {
            case 1: // Загрузка графа 
            {
                char* filename = AskFilename();

                // Освобождаем старый граф, если был
                if (g) {
                    FreeGraph(g);
                    g = NULL;
                }

                g = LoadGraphEdges(filename);
                if (!g) {
                    g = LoadGraphMatrix(filename);
                }
                // Если не загрузился, то берём дефолтный
                if (!g) {
                    printf("Не удалось загрузить файл. Использую граф по умолчанию.\n");
                    g = LoadGraphEdges("tests/default.txt");
                    if (!g) {
                        g = LoadGraphMatrix("tests/default.txt");
                    }
                }

                if (g) {
                    printf("Граф успешно загружен!\n");
                    PrintGraphInfo(g->numVertices, g->numEdges);
                } else {
                    printf("Не удалось загрузить граф.\n");
                }
                free(filename);
                
                break;
                
            }
            
            case 2: // Дейкстра
            {
                if (!g) {
                    printf("Сначала загрузите файл (пункт 1)\n");
                    break;
                }
                // Показываем список городов
                printf("\nДоступные города:\n");
                for (int i = 0; i < g->numVertices; i++) {   
                    printf("  %d - %s\n", i, g->cityNames[i]);
                }
 
                int start, end;
                printf("Введите стартовую вершину (0..%d): ", g->numVertices - 1);
                scanf("%d", &start);
                { int c; while ((c = getchar()) != '\n' && c != EOF); }
                printf("Введите конечную вершину (0..%d): ", g->numVertices - 1);
                scanf("%d", &end);
                { int c; while ((c = getchar()) != '\n' && c != EOF); }
 
                if (start < 0 || start >= g->numVertices || end < 0 || end >= g->numVertices) {
                    printf("Ошибка: неверный номер вершины\n");
                    break;
                }
                
                //Замер времени
                double startTime = GetTime();
                DijkstraResult* res = Dijkstra(g, start);
                double endTime = GetTime();
                double timeMS = (endTime - startTime) * 1000.0;
 
                if (!res) {
                    printf("Ошибка выполнения алгоритма Дейкстры\n");
                    break;
                }
 
 
                // Проверяем существует ли путь
                if (res->dist[end] == INT_MAX) {
                    printf("Пути не существует\n");
                } else {
                    int pathLen = 0;
                    int* path = GetPath(res, g, end, &pathLen); 
        
                    if (path == NULL || pathLen <= 0) {
                        printf("Ошибка: не удалось восстановить путь\n");
                    } else {
                        PrintDijkstra(g, path, pathLen, res->dist[end], timeMS);
                        free(path);
                    }
                }

 
                FreeDijkstraResult(res);
                break;
            }

            case 3: // Краскал
            {
                if (!g) {
                    printf("Сначала загрузите файл (пункт 1)\n");
                    break;
                }
                
                MSTResult* res = kruskal(g);
                if (res == NULL) {
                    printf("Невозможно построить MST, граф несвязный\n");
                } else {
                    PrintMST(res, g);
                    FreeMSTResult(res);
                }
                break;
            }
            
            case 4: // Прим
            {
                if (!g) {
                    printf("Сначала загрузите файл (пункт 1)\n");
                    break;
                }
                
                MSTResult* res = prim(g);
                if (res == NULL) {
                    printf("Невозможно построить MST, граф несвязный\n");
                } else {
                    PrintMST(res, g);
                    FreeMSTResult(res);
                }
                break;
            }
            
            case 5: // BFS
            {
                if (!g) {
                    printf("Сначала загрузите файл (пункт 1)\n");
                    break;
                }

                // Показываем список городов
                printf("\nДоступные города:\n");
                for (int i = 0; i < g->numVertices; i++) {
                    printf("  %d - %s\n", i, g->cityNames[i]);
                }

                int start;
                printf("Введите стартовую вершину (0..%d): ", g->numVertices - 1);
                scanf("%d", &start);
                { int c; while ((c = getchar()) != '\n' && c != EOF); }

                if (start < 0 || start >= g->numVertices) {
                    printf("Ошибка: неверный номер вершины\n");
                    break;
                }

                int orderLen; 
                
                //Замер времени
                double startT = GetTime();
                int* order = BFS(g, start, &orderLen);   
                double endT = GetTime();
                double time2MS = (endT - startT) * 1000.0;

                if (order == NULL || orderLen == 0) {
                    printf("Ошибка: обход не выполнен\n");
                } else {
                    PrintBFS(g, order, orderLen, time2MS);
                    free(order);
                }
                break;
            }
            
            case 6: // Сравнение алгоритмов MST 
            {
                if (!g) {
                    printf("Сначала загрузите файл (пункт 1)\n");
                    break;
                }

                Comparison cmp = CompareMST(g);  
                PrintComparison(&cmp);                          
                break;
            }
            
            case 0: // Выход
            {
                printf("Goodbye! The program is completed.\n");
                break;
            }
            
            default:
            {
                printf("Неверный выбор! Пожалуйста, выберите 0-6\n");
                break;
            }
        }
    } while (number != 0);   
    
    if (g) {
        FreeGraph(g);
    }
    
    return 0;
} 