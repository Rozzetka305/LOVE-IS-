#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <queue>
using namespace std;

/* Проверка, что введено положительное число */
int is_positive_integer(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i]; i++)
        if (!isdigit((unsigned char)str[i]))
            return 0;
    return atoi(str) > 0;
}

int** generateAdjMatrix(int n) {
    int** G = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        G[i] = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int edge = rand() % 2;  
            G[i][j] = edge;
            G[j][i] = edge; 
        }
    }
    return G;
}

/* Печать матрицы */
void printMatrix(int** G, int n) {
    printf("\nМатрица смежности:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", G[i][j]);
        printf("\n");
    }
}

/* BFS по матрице */
void BFS_matrix(int** G, int n, int start, int* DIST) {
    for (int i = 0; i < n; i++) DIST[i] = -1;

    queue<int> Q;
    DIST[start] = 0;
    Q.push(start);

    printf("\nBFS по матрице, порядок обхода: ");
    while (!Q.empty()) {
        int v = Q.front(); Q.pop();
        printf("%d ", v);
        for (int i = 0; i < n; i++) {
            if (G[v][i] == 1 && DIST[i] == -1) {
                DIST[i] = DIST[v] + 1;
                Q.push(i);
            }
        }
    }
    printf("\n");
}

/* DFS по матрице */
void DFS_matrix(int** G, int n, int v, int* DIST, int dist) {
    DIST[v] = dist;
    for (int i = 0; i < n; i++) {
        if (G[v][i] == 1 && DIST[i] == -1) {
            DFS_matrix(G, n, i, DIST, dist + 1);
        }
    }
}

/* --- Структуры для списка смежности --- */
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

/* Построение списков смежности из матрицы смежности */
Node** buildAdjListFromMatrix(int** G, int n) {
    Node** adjList = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) adjList[i] = NULL;

    for (int i = 0; i < n; i++) {
        for (int j = n - 1; j >= 0; j--) { // j с конца — для аккуратного порядка
            if (G[i][j] == 1) {
                Node* node = (Node*)malloc(sizeof(Node));
                node->vertex = j;
                node->next = adjList[i];
                adjList[i] = node;
            }
        }
    }
    return adjList;
}

/* Печать списков смежности */
void printGraph(Node** adjList, int n) {
    printf("\nСписки смежности графа:\n");
    for (int i = 0; i < n; i++) {
        printf("%d:", i);
        Node* curr = adjList[i];
        while (curr) {
            printf(" %d", curr->vertex);
            curr = curr->next;
        }
        printf("\n");
    }
}

/* BFS по спискам смежности */
void BFS_list(Node** adjList, int n, int start, int* DIST) {
    for (int i = 0; i < n; i++) DIST[i] = -1;

    queue<int> Q;
    DIST[start] = 0;
    Q.push(start);

    printf("\nBFS по спискам смежности, порядок обхода: ");
    while (!Q.empty()) {
        int v = Q.front(); Q.pop();
        printf("%d ", v);

        Node* curr = adjList[v];
        while (curr) {
            int u = curr->vertex;
            if (DIST[u] == -1) {
                DIST[u] = DIST[v] + 1;
                Q.push(u);
            }
            curr = curr->next;
        }
    }
    printf("\n");
}

/* DFS по спискам смежности */
void DFS_list(Node** adjList, int v, int* DIST, int dist) {
    DIST[v] = dist;
    Node* curr = adjList[v];
    while (curr) {
        int u = curr->vertex;
        if (DIST[u] == -1) {
            DFS_list(adjList, u, DIST, dist + 1);
        }
        curr = curr->next;
    }
}

/* Освобождение памяти */
void freeGraph(Node** adjList, int n) {
    for (int i = 0; i < n; i++) {
        Node* curr = adjList[i];
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(adjList);
}

int main(void) {

    setlocale(LC_ALL, "rus");
    srand((unsigned)time(NULL));

    char input[100];
    int n;

    /* Ввод количества вершин */
    while (1) {
        printf("Введите количество вершин графа: ");
        if (!fgets(input, sizeof(input), stdin)) { fprintf(stderr, "Ошибка ввода\n"); return 1; }
        input[strcspn(input, "\n")] = '\0';
        if (is_positive_integer(input)) { n = atoi(input); break; }
        printf("Ошибка: нужно положительное число!\n");
    }

    /* Ввод стартовой вершины */
    int start;
    while (1) {
        printf("Введите исходную вершину (0..%d): ", n - 1);
        if (!fgets(input, sizeof(input), stdin)) { fprintf(stderr, "Ошибка ввода\n"); return 1; }
        input[strcspn(input, "\n")] = '\0';
        if (is_positive_integer(input) || strcmp(input, "0") == 0) {
            start = atoi(input);
            if (start >= 0 && start < n) break;
        }
        printf("Ошибка: некорректный номер вершины!\n");
    }

    int* DIST = (int*)malloc(n * sizeof(int));

    /* Матрица смежности */
    int** G = generateAdjMatrix(n);
    printMatrix(G, n);

    /* BFS + время */
    clock_t t1 = clock();
    BFS_matrix(G, n, start, DIST);
    clock_t t2 = clock();
    double timeBFS_matrix = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;

    printf("\nDIST BFS (матрица):\n");
    for (int i = 0; i < n; i++) printf("DIST[%d]=%d\n", i, DIST[i]);
    printf("Время BFS (матрица): %.3f мс\n", timeBFS_matrix);

    /* DFS + время */
    for (int i = 0; i < n; i++) DIST[i] = -1;
    t1 = clock();
    DFS_matrix(G, n, start, DIST, 0);
    t2 = clock();
    double timeDFS_matrix = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;

    printf("\nDIST DFS (матрица):\n");
    for (int i = 0; i < n; i++) printf("DIST[%d]=%d\n", i, DIST[i]);
    printf("Время DFS (матрица): %.3f мс\n", timeDFS_matrix);

    /* --- Списки смежности по матрице --- */
    Node** adjList = buildAdjListFromMatrix(G, n);
    printGraph(adjList, n);

    /* BFS по спискам */
    for (int i = 0; i < n; i++) DIST[i] = -1;
    t1 = clock();
    BFS_list(adjList, n, start, DIST);
    t2 = clock();
    double timeBFS_list = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;

    printf("\nDIST BFS (списки):\n");
    for (int i = 0; i < n; i++) printf("DIST[%d]=%d\n", i, DIST[i]);
    printf("Время BFS (списки): %.3f мс\n", timeBFS_list);

    /* DFS по спискам */
    for (int i = 0; i < n; i++) DIST[i] = -1;
    t1 = clock();
    DFS_list(adjList, start, DIST, 0);
    t2 = clock();
    double timeDFS_list = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;

    printf("\nDIST DFS (списки):\n");
    for (int i = 0; i < n; i++) printf("DIST[%d]=%d\n", i, DIST[i]);
    printf("Время DFS (списки): %.3f мс\n", timeDFS_list);

    /* Освобождение памяти */
    free(DIST);
    freeGraph(adjList, n);
    for (int i = 0; i < n; i++) free(G[i]);
    free(G);

    return 0;
}
