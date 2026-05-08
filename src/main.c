#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Graph* g = NULL;
    int number = Menu();

    switch(number){
        case 1: //Загрузка графа 
        {
            char* filename = AskFilename();
            CheckFile(filename);

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
                printf("Граф успешно загружен!");
                PrintGraphInfo(g -> numVertices, g -> numEdges);
            } else {
                printf("Не удалось загрузить граф.");
            }
            free(filename);
            break;
        }
        case 2: //Дейкстра
        {
            if (!g){
                printf("Cначала загрузите файл (пункт 1)");
                break;
            }
            // Показываем список городов
            printf("\nДоступные города:\n");
            for (int i = 0; i < g->num_vertices; i++) {
                printf("  %d - %s\n", i, g->city_names[i]);
            }

            int start, end;
            printf("Введите стартовую вершину (0..%d): ", g->num_vertices - 1);
            scanf("%d", &start);
            printf("Введите конечную вершину (0..%d): ", g->num_vertices - 1);
            scanf("%d", &end);

            if (start < 0 || start >= g->num_vertices || end < 0 || end >= g->num_vertices) {
                printf("Ошибка: неверный номер вершины\n");
                break;
            
            }
            
            DijkstraResult* res = dijkstra(g, start);

            //Проверяем существует ли путь
            if (res -> dist[end] == INT_MAX) {
                printf("Пути не существует");
            } else {
                int pathLen;
                int *path = getPath(res, end, &pathLen);
                PrintDijkstra(g, path, pathLen, res->dist[end]);
                free(path);
            }

            FreeDijkstraResult(res);
            break;
        } 

        case 3: //Краскал
        {
            if (!g){
                printf("Cначала загрузите файл (пункт 1)");
                break;
            }
            
            MSTResult* res = kruskal(g);
            if (res == NULL) {
                printf("Невозможно построить MST, граф несвязный");
            } else {
                PrintMST(res);
                FreeMSTResult(res);
            }
            break;
        }
        case 4: //Прим
        {
            if (!g){
                printf("Cначала загрузите файл (пункт 1)");
                break;
            }
            
            MSTResult* res = prim(g);
            if (res == NULL) {
                printf("Невозможно построить MST, граф несвязный");
            } else {
                PrintMST(res);
                FreeMSTResult(res);
            }
            break;
        }
        case 5: //BFS
        {
            if (!g){
                printf("Cначала загрузите файл (пункт 1)");
                break;
            }

            // Показываем список городов
            printf("\nДоступные города:\n");
            for (int i = 0; i < g->num_vertices; i++) {
                printf("  %d - %s\n", i, g->city_names[i]);
            }

            int start;
            printf("Введите стартовую вершину (0..%d): ", g->num_vertices - 1);
            scanf("%d", &start);

            if (start < 0 || start >= g->num_vertices) {
                printf("Ошибка: неверный номер вершины\n");
                break;
            
            }

            int orderLen; 
            int* order = BFS(g, start, &orderLen);
        
            //Проверка и вывод результата
            if (order == NULL || orderLen == 0) {
                printf("Ошибка: обход не выполнен");
            } else {
                PrintBFS(g, order, orderLen);
                free(order);
            }
        
            break;
        }
        case 6: //Сравнение алгоритмов BFS
        {
            if (!g){
                printf("Cначала загрузите файл (пункт 1)");
                break;
            }

            Comparison cmp = CompareMST(g);  
            PrintComparison(&cmp);                          
            break;
        }
        default: // Все остальные варианты (не 0-6)
        {
            printf("Неверный выбор! Пожалуйста, выберите 1-6\n");
            break;
        }
    }
}