#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_VERTICES 500  

//Вспомогательные функции для проверки ввода 

int read_positive_int(const char* prompt, int min, int max) {
    char buffer[256];
    long val;
    char* endptr;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            // ввод закончился
            printf("Ошибка ввода. Попробуйте снова.\n");
            continue;
        }
        // Убрать перевод строки
        buffer[strcspn(buffer, "\n")] = '\0';

        // Проверка на пустую строку
        if (buffer[0] == '\0') {
            printf("Пустой ввод. Введите целое положительное число.\n");
            continue;
        }

        // Проверка на то, что все символы - цифры (плюс возможный ведущий '+')
        int i = 0;
        if (buffer[0] == '+') i = 1;
        int ok = 1;
        for (; buffer[i]; ++i) {
            if (!isdigit((unsigned char)buffer[i])) { ok = 0; break; }
        }
        if (!ok) {
            printf("Разрешены только целые положительные числа. Попробуйте снова.\n");
            continue;
        }

        val = strtol(buffer, &endptr, 10);
        if (endptr == buffer) {
            printf("Неверный ввод. Попробуйте снова.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Число должно быть в диапазоне %d..%d. Попробуйте снова.\n", min, max);
            continue;
        }
        return (int)val;
    }
}

//Очередь на массиве (простая реализация)

typedef struct {
    int* data;
    int front;
    int rear;
    int capacity;
} ArrayQueue;

void aq_init(ArrayQueue* q, int capacity) {
    q->data = (int*)malloc(sizeof(int) * capacity);
    q->front = 0;
    q->rear = -1;
    q->capacity = capacity;
}

void aq_free(ArrayQueue* q) {
    free(q->data);
}

int aq_is_empty(ArrayQueue* q) {
    return q->rear < q->front;
}

void aq_enqueue(ArrayQueue* q, int v) {
    if (q->rear + 1 >= q->capacity) {
        return;
    }
    q->data[++q->rear] = v;
}

int aq_dequeue(ArrayQueue* q) {
    if (aq_is_empty(q)) return -1;
    return q->data[q->front++];
}

//Очередь на связанном списке (самостоятельная реализация) 

typedef struct Node {
    int val;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} ListQueue;

void lq_init(ListQueue* q) {
    q->front = q->rear = NULL;
}

int lq_is_empty(ListQueue* q) {
    return q->front == NULL;
}

void lq_enqueue(ListQueue* q, int v) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (!p) { perror("malloc"); exit(EXIT_FAILURE); }
    p->val = v; p->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = p;
    }
    else {
        q->rear->next = p;
        q->rear = p;
    }
}

int lq_dequeue(ListQueue* q) {
    if (lq_is_empty(q)) return -1;
    Node* p = q->front;
    int v = p->val;
    q->front = p->next;
    if (q->front == NULL) q->rear = NULL;
    free(p);
    return v;
}

void lq_free_all(ListQueue* q) {
    while (!lq_is_empty(q)) lq_dequeue(q);
}

//Генерация и печать матрицы смежности 

void generate_random_adj_matrix(int n, int** matrix) {
    int density = 40 + rand() % 41;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) matrix[i][j] = 0;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int r = rand() % 100;
            if (r < density) {
                matrix[i][j] = matrix[j][i] = 1;
            }
        }
    }
}

void print_matrix(int n, int** matrix) {
    printf("Матрица смежности (%d x %d):\n", n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
                printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

//Построение списков смежности из матрицы

int** build_adj_list(int n, int** matrix, int* deg) {
    // deg[i] = степень вершины i
    for (int i = 0; i < n; ++i) {
        deg[i] = 0;
        for (int j = 0; j < n; ++j)
            if (matrix[i][j]) deg[i]++;
    }
    int** adj = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        if (deg[i] > 0)
            adj[i] = (int*)malloc(sizeof(int) * deg[i]);
        else
            adj[i] = NULL;
        int k = 0;
        for (int j = 0; j < n; ++j)
            if (matrix[i][j]) adj[i][k++] = j;
    }
    return adj;
}

void free_adj_list(int n, int** adj) {
    for (int i = 0; i < n; ++i) if (adj[i]) free(adj[i]);
    free(adj);
}

//BFS: матрица + массивная очередь

void BFS_matrix_arrayqueue(int n, int** G, int start, int* visited) {
    ArrayQueue q;
    aq_init(&q, n); 
    aq_enqueue(&q, start);
    visited[start] = 1;

    while (!aq_is_empty(&q)) {
        int v = aq_dequeue(&q);
        printf("%d ", v + 1); // выводим вершины с 1
        for (int i = 0; i < n; ++i) {
            if (G[v][i] == 1 && visited[i] == 0) {
                aq_enqueue(&q, i);
                visited[i] = 1; // пометить i как посещенную
            }
        }
    }

    aq_free(&q);
}

// BFS: матрица + очередь на списке

void BFS_matrix_listqueue(int n, int** G, int start, int* visited) {
    ListQueue q;
    lq_init(&q);
    lq_enqueue(&q, start);
    visited[start] = 1;

    while (!lq_is_empty(&q)) {
        int v = lq_dequeue(&q);
        printf("%d ", v + 1);
        for (int i = 0; i < n; ++i) {
            if (G[v][i] == 1 && visited[i] == 0) {
                lq_enqueue(&q, i);
                visited[i] = 1;
            }
        }
    }

    lq_free_all(&q);
}

//BFS: списки смежности + массивная очередь 

void BFS_adjlist_arrayqueue(int n, int** adj, int* deg, int start, int* visited) {
    ArrayQueue q;
    aq_init(&q, n);
    aq_enqueue(&q, start);
    visited[start] = 1;

    while (!aq_is_empty(&q)) {
        int v = aq_dequeue(&q);
        printf("%d ", v + 1);
        for (int k = 0; k < deg[v]; ++k) {
            int to = adj[v][k];
            if (!visited[to]) {
                aq_enqueue(&q, to);
                visited[to] = 1;
            }
        }
    }
    aq_free(&q);
}

// Алгоритм ПОШ: обойти все компоненты, вызывая BFS 

void POSH_matrix_arrayqueue_all(int n, int** G) {
    int* visited = (int*)calloc(n, sizeof(int));
    for (int v = 0; v < n; ++v) {
        if (!visited[v]) {
            BFS_matrix_arrayqueue(n, G, v, visited);
        }
    }
    free(visited);
}

void POSH_matrix_listqueue_all(int n, int** G) {
    int* visited = (int*)calloc(n, sizeof(int));
    for (int v = 0; v < n; ++v) {
        if (!visited[v]) {
            BFS_matrix_listqueue(n, G, v, visited);
        }
    }
    free(visited);
}

void POSH_adjlist_arrayqueue_all(int n, int** adj, int* deg) {
    int* visited = (int*)calloc(n, sizeof(int));
    for (int v = 0; v < n; ++v) {
        if (!visited[v]) {
            BFS_adjlist_arrayqueue(n, adj, deg, v, visited);
        }
    }
    free(visited);
}

int main(void) {

    setlocale(LC_ALL, "rus");

    srand((unsigned)time(NULL));

    int n = read_positive_int("Введите количество вершин графа (1..500): ", 1, MAX_VERTICES);

    // Выделим матрицу как массив указателей на строки
    int** G = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        G[i] = (int*)malloc(sizeof(int) * n);
    }

    generate_random_adj_matrix(n, G);
    print_matrix(n, G);

    // Построим списки смежности
    int* deg = (int*)malloc(sizeof(int) * n);
    int** adj = build_adj_list(n, G, deg);

    printf("\nBFS (матрица + массивная очередь) — порядок обхода вершин:\n");
    clock_t t1 = clock();
    POSH_matrix_arrayqueue_all(n, G);
    clock_t t2 = clock();
    double time_array_queue = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("\nВремя: %.6f сек\n", time_array_queue);

    printf("\nBFS (матрица + очередь на списке) — порядок обхода вершин:\n");
    t1 = clock();
    POSH_matrix_listqueue_all(n, G);
    t2 = clock();
    double time_list_queue = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("\nВремя: %.6f сек\n", time_list_queue);

    printf("\nBFS (списки смежности + массивная очередь) — порядок обхода вершин:\n");
    t1 = clock();
    POSH_adjlist_arrayqueue_all(n, adj, deg);
    t2 = clock();
    double time_adjlist = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("\nВремя: %.6f сек\n", time_adjlist);

    printf("\nСравнение времени (матрица): массивная очередь = %.6f с, очередь на списке = %.6f с\n",
        time_array_queue, time_list_queue);

    // Очистка
    for (int i = 0; i < n; ++i) free(G[i]);
    free(G);
    free_adj_list(n, adj);
    free(deg);

    return 0;
}