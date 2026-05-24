#include "backendGraph.h"

void CreateFile(const char* filename, const char* content){
    FILE* f = fopen(filename, "w");
    fputs(content, f);
    fclose(f);
}

void TestCheckFileToLoad(){
    CreateFile("valid_edges.txt",
        "1\n"
        "3\n"
        "A\n"
        "B\n"
        "C\n"
        "0 1 5\n"
        "1 2 7\n"
    );

    assert(CheckFileToLoad("valid_edges.txt") != NULL);
    remove("valid_edges.txt");

    CreateFile("invalid_edges_extra.txt",
        "1\n"
        "2\n"
        "A\n"
        "B\n"
        "0 1 5 abc\n"
    );

    assert(CheckFileToLoad("invalid_edges_extra.txt") == NULL);
    remove("invalid_edges_extra.txt");

    CreateFile("invalid_edges_few.txt",
        "1\n"
        "2\n"
        "A\n"
        "B\n"
        "0 1\n"
    );

    assert(CheckFileToLoad("invalid_edges_few.txt") == NULL);
    remove("invalid_edges_few.txt");

    CreateFile("invalid_city_count.txt",
        "1\n"
        "3\n"
        "A\n"
        "B\n"
        "0 1 5\n"
    );

    assert(CheckFileToLoad("invalid_city_count.txt") == NULL);
    remove("invalid_city_count.txt");

    CreateFile("valid_matrix.txt",
        "2\n"
        "3\n"
        "A\n"
        "B\n"
        "C\n"
        "0 1 2\n"
        "1 0 3\n"
        "2 3 0\n"
    );

    assert(CheckFileToLoad("valid_matrix.txt") != NULL);
    remove("valid_matrix.txt");

    CreateFile("invalid_matrix_cols.txt",
        "2\n"
        "3\n"
        "A\n"
        "B\n"
        "C\n"
        "0 1\n"
        "1 0 3\n"
        "2 3 0\n"
    );

    assert(CheckFileToLoad("invalid_matrix_cols.txt") == NULL);
    remove("invalid_matrix_cols.txt");

    CreateFile("invalid_matrix_rows.txt",
        "2\n"
        "2\n"
        "A\n"
        "B\n"
        "0 1\n"
        "1 0\n"
        "1 1\n"
    );

    assert(CheckFileToLoad("invalid_matrix_rows.txt") == NULL);
    remove("invalid_matrix_rows.txt");

    CreateFile("invalid_matrix_text.txt",
        "2\n"
        "2\n"
        "A\n"
        "B\n"
        "0 x\n"
        "1 0\n"
    );

    assert(CheckFileToLoad("invalid_matrix_text.txt") == NULL);
    remove("invalid_matrix_text.txt");

    CreateFile("invalid_format.txt",
        "5\n"
        "2\n"
        "A\n"
        "B\n"
    );

    assert(CheckFileToLoad("invalid_format.txt") == NULL);
    remove("invalid_format.txt");

    CreateFile("empty.txt", "");

    assert(CheckFileToLoad("empty.txt") == NULL);
    assert(CheckFileToLoad("no_file.txt") == NULL);

    remove("empty.txt");

    printf("All CheckFileToLoad tests passed!\n");
}

void TestDijkstra(){

    Graph* g = CreateGraph(4);

    AddEdge(g, 0, 1, 1);
    AddEdge(g, 1, 2, 2);
    AddEdge(g, 0, 2, 5);
    AddEdge(g, 2, 3, 1);

    DijkstraResult* r = Dijkstra(g, 0);

    assert(r != NULL);

    assert(r->dist[0] == 0);
    assert(r->dist[1] == 1);
    assert(r->dist[2] == 3);
    assert(r->dist[3] == 4);

    FreeDijkstraResult(r);
    FreeGraph(g);

    printf("Dijkstra tests passed!\n");
}

void TestGetPath(){

    Graph* g = CreateGraph(4);

    AddEdge(g, 0, 1, 1);
    AddEdge(g, 1, 2, 2);
    AddEdge(g, 2, 3, 1);

    DijkstraResult* r = Dijkstra(g, 0);

    int len;

    int* path = GetPath(r, g, 3, &len);

    assert(path != NULL);

    assert(len == 4);

    assert(path[0] == 0);
    assert(path[1] == 1);
    assert(path[2] == 2);
    assert(path[3] == 3);

    free(path);

    FreeDijkstraResult(r);
    FreeGraph(g);

    printf("GetPath tests passed!\n");
}

void TestBFS(){

    Graph* g = CreateGraph(4);

    AddEdge(g, 0, 1, 1);
    AddEdge(g, 0, 2, 1);
    AddEdge(g, 1, 3, 1);

    int len;

    int* order = BFS(g, 0, &len);

    assert(order != NULL);

    assert(len == 4);

    assert(order[0] == 0);

    free(order);

    FreeGraph(g);

    assert(BFS(NULL, 0, &len) == NULL);

    printf("BFS tests passed!\n");
}

int main(){

    TestCheckFileToLoad();

    TestDijkstra();

    TestGetPath();

    TestBFS();

    printf("All tests passed!\n");

    return 0;
}

