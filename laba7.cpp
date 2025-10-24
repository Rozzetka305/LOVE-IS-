#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

#define MAX_VERTICES 100

int adjMatrix[MAX_VERTICES][MAX_VERTICES];
int adjList[MAX_VERTICES][MAX_VERTICES];
int listSize[MAX_VERTICES];
int visited[MAX_VERTICES];
int n = 0;
int generated = 0;

//Проверка: положительное целое число 
int is_positive_integer(const char* str) {
    if (str == NULL || *str == '\0')
        return 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (!isdigit(str[i])) return 0;
    int num = atoi(str);
    return num > 0;
}

//Безопасный ввод количества вершин 
int get_vertex_count() {
    char input[100];
    int num;
    while (1) {
        printf("\nВведите количество вершин графа: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка чтения ввода.\n");
            continue;
        }
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == '\n') {
                input[i] = '\0';
                break;
            }
        }
        if (!is_positive_integer(input)) {
            printf("Ошибка: введите положительное целое число!\n");
            continue;
        }
        num = atoi(input);
        if (num > MAX_VERTICES) {
            printf("Ошибка: максимум %d вершин!\n", MAX_VERTICES);
            continue;
        }
        break;
    }
    return num;
}

//Генерация неориентированного графа
void generateAdjacencyMatrix(int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adjMatrix[i][j] = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int edge = rand() % 2;
            adjMatrix[i][j] = edge;
            adjMatrix[j][i] = edge;
        }
    }
}

//Вывод матрицы смежности 
void printAdjacencyMatrix() {
    printf("\nМатрица смежности (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", adjMatrix[i][j]);
        printf("\n");
    }
}

//Построение списков смежности
void buildAdjacencyLists() {
    for (int i = 0; i < n; i++) {
        listSize[i] = 0;
        for (int j = 0; j < n; j++) {
            if (adjMatrix[i][j] == 1) {
                adjList[i][listSize[i]++] = j;
            }
        }
    }
}

//Вывод списков смежности 
void printAdjacencyLists() {
    printf("\nСписки смежности:\n");
    for (int i = 0; i < n; i++) {
        printf("%d: ", i);
        for (int j = 0; j < listSize[i]; j++)
            printf("%d ", adjList[i][j]);
        printf("\n");
    }
}

//Рекурсивный DFS по матрице 
void DFS_matrix_recursive(int v) {
    visited[v] = 1;
    printf("%d ", v);

    for (int i = 0; i < n; i++) {
        if (adjMatrix[v][i] == 1 && !visited[i])
            DFS_matrix_recursive(i);
    }
}

//Нерекурсивный DFS по матрице 
void DFS_matrix_iterative(int start) {
    int stack[MAX_VERTICES];
    int top = -1;

    visited[start] = 1;
    stack[++top] = start;

    while (top >= 0) {
        int v = stack[top--];
        printf("%d ", v);

        for (int i = n - 1; i >= 0; i--) {
            if (adjMatrix[v][i] == 1 && !visited[i]) {
                visited[i] = 1;
                stack[++top] = i;
            }
        }
    }
}

//Обход в глубину по матрице (рекурсивный) 
void POG_matrix_recursive() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("Рекурсивный обход (матрица смежности): ");
    for (int v = 0; v < n; v++)
        if (!visited[v])
            DFS_matrix_recursive(v);
    printf("\n");
}

//Обход в глубину по матрице (нерекурсивный) 
void POG_matrix_iterative() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("Нерекурсивный обход (матрица смежности): ");
    for (int v = 0; v < n; v++)
        if (!visited[v])
            DFS_matrix_iterative(v);
    printf("\n");
}

//Рекурсивный DFS по спискам смежности 
void DFS_list_recursive(int v) {
    visited[v] = 1;
    printf("%d ", v);

    for (int i = 0; i < listSize[v]; i++) {
        int neighbor = adjList[v][i];
        if (!visited[neighbor])
            DFS_list_recursive(neighbor);
    }
}

//Обход в глубину по спискам смежности 
void POG_list_recursive() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("Рекурсивный обход (списки смежности): ");
    for (int v = 0; v < n; v++)
        if (!visited[v])
            DFS_list_recursive(v);
    printf("\n");
}

//Меню 
int menu() {
    int choice;
    printf("\n===== МЕНЮ =====\n");
    printf("1. Сгенерировать граф\n");
    printf("2. Показать матрицу смежности\n");
    printf("3. Построить и показать списки смежности\n");
    printf("4. Обход в глубину (матрица, рекурсивно)\n");
    printf("5. Обход в глубину (матрица, нерекурсивно)\n");
    printf("6. Обход в глубину (списки смежности)\n");
    printf("7. Выход\n");
    printf("Выберите пункт: ");
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');
    return choice;
}

int main() {
    setlocale(LC_ALL, "rus");

    while (1) {
        int choice = menu();

        switch (choice) {
        case 1:
            n = get_vertex_count();
            generateAdjacencyMatrix(n);
            generated = 1;
            printf("Граф с %d вершинами успешно сгенерирован!\n", n);
            break;

        case 2:
            if (!generated)
                printf("Сначала сгенерируйте граф!\n");
            else
                printAdjacencyMatrix();
            break;

        case 3:
            if (!generated)
                printf("Сначала сгенерируйте граф!\n");
            else {
                buildAdjacencyLists();
                printAdjacencyLists();
            }
            break;

        case 4:
            if (!generated)
                printf("Сначала сгенерируйте граф!\n");
            else
                POG_matrix_recursive();
            break;

        case 5:
            if (!generated)
                printf("Сначала сгенерируйте граф!\n");
            else
                POG_matrix_iterative();
            break;

        case 6:
            if (!generated)
                printf("Сначала сгенерируйте граф!\n");
            else {
                buildAdjacencyLists();
                POG_list_recursive();
            }
            break;

        case 7:
            printf("Выход из программы...\n");
            return 0;

        default:
            printf("Ошибка: выберите пункт от 1 до 7.\n");
        }
    }

    return 0;
}