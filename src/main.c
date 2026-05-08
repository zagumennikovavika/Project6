#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  

int main() {
    Graph* g = NULL;
    int number;                     
    
    do {
        number = Menu();           
        
        switch(number) {
            case 1: // Загрузка графа 
            {
                char* filename = AskFilename();
                CheckFile(filename);

                // Освобождаем старый граф, если был
                if (g) {
                    FreeGraph(g);
                    g = NULL;
                }

                g = loadGraphEdges(filename);
                if (!g) {
                    g = loadGraphMatrix(filename);
                }
                // Если не загрузился, то берём дефолтный
                if (!g) {
                    printf("Не удалось загрузить файл. Использую граф по умолчанию.\n");
                    g = loadGraphEdges("tests/default.txt");
                    if (!g) {
                        g = loadGraphMatrix("tests/default.txt");
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
                    printf("  %d - %s\n", i, g->city_names[i]);
                }

                int start, end;
                printf("Введите стартовую вершину (0..%d): ", g->numVertices - 1);
                scanf("%d", &start);
                printf("Введите конечную вершину (0..%d): ", g->numVertices - 1);
                scanf("%d", &end);

                if (start < 0 || start >= g->numVertices || end < 0 || end >= g->numVertices) {
                    printf("Ошибка: неверный номер вершины\n");
                    break;
                }
                
                DijkstraResult* res = dijkstra(g, start);
                if (!res) {
                    printf("Ошибка выполнения алгоритма Дейкстры\n");
                    break;
                }

                // Проверяем существует ли путь
                if (res->dist[end] == INT_MAX) {
                    printf("Пути не существует\n");
                } else {
                    int pathLen;
                    int* path = getPath(res, end, &pathLen);
                    PrintDijkstra(g, path, pathLen, res->dist[end]);
                    free(path);
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
                    PrintMST(res);
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
                    PrintMST(res);
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
                    printf("  %d - %s\n", i, g->city_names[i]);
                }

                int start;
                printf("Введите стартовую вершину (0..%d): ", g->numVertices - 1);
                scanf("%d", &start);

                if (start < 0 || start >= g->numVertices) {
                    printf("Ошибка: неверный номер вершины\n");
                    break;
                }

                int orderLen; 
                int* order = BFS(g, start, &orderLen);
            
                if (order == NULL || orderLen == 0) {
                    printf("Ошибка: обход не выполнен\n");
                } else {
                    PrintBFS(g, order, orderLen);
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
                printf("Выход из программы.\n");
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