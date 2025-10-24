#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

/* Проверка, что строка состоит только из цифр */
int is_valid_int(const char* s) {
    if (!s || *s == '\0') return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

/* Считывание положительного целого числа с проверкой */
int read_positive_int(const char* prompt) {
    char buffer[128];
    long n;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "Ошибка ввода!\n");
            exit(EXIT_FAILURE);
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // убрать \n

        if (!is_valid_int(buffer)) {
            printf("Ошибка: введите положительное целое число!\n");
            continue;
        }

        n = atol(buffer);
        if (n <= 0) {
            printf("Ошибка: число должно быть больше нуля!\n");
            continue;
        }
        if (n > 1000000) { // защита от нереально большого ввода
            printf("Ошибка: слишком большое число вершин!\n");
            continue;
        }
        return (int)n;
    }
}

/* Генерация n x n матрицы (симметричной), включая петли */
int** generate_matrix(int n) {
    int i, j;
    int** a = (int**)malloc(n * sizeof(int*));
    if (!a) { perror("malloc"); exit(EXIT_FAILURE); }

    for (i = 0; i < n; ++i) {
        a[i] = (int*)calloc(n, sizeof(int));
        if (!a[i]) { perror("calloc"); exit(EXIT_FAILURE); }
    }

    for (i = 0; i < n; ++i) {
        for (j = i; j < n; ++j) {
            int edge = rand() % 2;
            a[i][j] = edge;
            a[j][i] = edge;
        }
    }
    return a;
}

/* Печать матрицы n x n */
void print_matrix(int** a, int n, const char* name) {
    printf("%s (n = %d):\n", name, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* Преобразование матрицы в список смежности */
int** matrix_to_adjlist(int** a, int n, int** out_deg) {
    int* deg = (int*)calloc(n, sizeof(int));
    if (!deg) { perror("calloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (a[i][j]) deg[i]++;

    int** adj = (int**)malloc(n * sizeof(int*));
    if (!adj) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < n; ++i) {
        adj[i] = deg[i] ? (int*)malloc(deg[i] * sizeof(int)) : NULL;
        int pos = 0;
        for (int j = 0; j < n; ++j)
            if (a[i][j]) adj[i][pos++] = j + 1;
    }

    *out_deg = deg;
    return adj;
}

/* Печать списка смежности */
void print_adjlist(int** adj, int* deg, int n, const char* name) {
    printf("%s (список смежности):\n", name);
    for (int i = 0; i < n; ++i) {
        printf("%d:", i + 1);
        if (deg[i] == 0) printf(" (нет соседей)");
        else for (int k = 0; k < deg[i]; ++k) printf(" %d", adj[i][k]);
        printf("\n");
    }
    printf("\n");
}

/* Освобождение памяти */
void free_matrix(int** a, int n) {
    if (!a) return;
    for (int i = 0; i < n; ++i) {
        if (a[i]) free(a[i]);
    }
    free(a);
}

void free_adjlist(int** adj, int* deg, int n) {
    if (!adj) return;
    for (int i = 0; i < n; ++i) if (adj[i]) free(adj[i]);
    free(adj);
    free(deg);
}

/* (а) Отождествление вершин (v1 и v2 — 1-based) */
int** merge_vertices(int** a, int* n, int v1, int v2) {
    if (!a) return NULL;
    v1--; v2--;
    if (v1 < 0 || v2 < 0 || v1 >= *n || v2 >= *n) return a;
    if (v1 == v2) return a;

    int new_n = *n - 1;
    int** b = (int**)malloc(new_n * sizeof(int*));
    if (!b) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < new_n; ++i) {
        b[i] = (int*)calloc(new_n, sizeof(int));
        if (!b[i]) { perror("calloc"); exit(EXIT_FAILURE); }
    }

    for (int i = 0, bi = 0; i < *n; ++i) {
        if (i == v2) continue;
        for (int j = 0, bj = 0; j < *n; ++j) {
            if (j == v2) continue;
            if (i == v1 || j == v1) {
                int val;
                /* Если строка/столбец содержит v1 или v2 — объединяем рёбра */
                if (i == v1 && j == v1) {
                    /* петля на объединённой вершине: если была петля у v1 или v2 */
                    val = (a[v1][v1] || a[v2][v2] || a[v1][v2]) ? 1 : 0;
                }
                else {
                    int orig_j = (j == v1) ? v1 : j;
                    /* Проверяем связи от объединённой вершины к orig_j */
                    val = (a[v1][j] || a[v2][j]) ? 1 : 0;
                }
                b[bi][bj] = val;
            }
            else {
                b[bi][bj] = a[i][j];
            }
            bj++;
        }
        bi++;
    }

    free_matrix(a, *n);
    *n = new_n;
    return b;
}

/* (б) Стягивание ребра (v1 и v2 — 1-based) */
int** contract_edge(int** a, int* n, int v1, int v2) {
    if (!a) return NULL;
    v1--; v2--;
    if (v1 < 0 || v2 < 0 || v1 >= *n || v2 >= *n) return a;
    if (a[v1][v2] == 0) {
        printf("Ребро между вершинами %d и %d отсутствует!\n", v1 + 1, v2 + 1);
        return a;
    }
    return merge_vertices(a, n, v1 + 1, v2 + 1);
}

int** split_vertex(int** a, int* n, int v1) {
    if (!a) return NULL;
    v1--;
    if (v1 < 0 || v1 >= *n) return a;

    int old_n = *n;
    int new_n = old_n + 1;

    // создаём новую матрицу
    int** b = (int**)malloc(new_n * sizeof(int*));
    if (!b) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < new_n; ++i) {
        b[i] = (int*)calloc(new_n, sizeof(int));
        if (!b[i]) { perror("calloc"); exit(EXIT_FAILURE); }
    }

    // копируем старую матрицу в верхний левый угол
    for (int i = 0; i < old_n; ++i)
        for (int j = 0; j < old_n; ++j)
            b[i][j] = a[i][j];

    int new_v = new_n - 1; // индекс новой вершины

    // новая вершина наследует все связи исходной
    for (int j = 0; j < old_n; ++j) {
        if (a[v1][j]) {
            b[new_v][j] = 1;
            b[j][new_v] = 1;
        }
    }

    // добавляем связь между исходной и новой вершинами
    b[v1][new_v] = 1;
    b[new_v][v1] = 1;

    // если у исходной вершины была петля — дублируем и у новой
    if (a[v1][v1]) {
        b[new_v][new_v] = 1;
        b[v1][v1] = 1;
    }

    free_matrix(a, old_n);
    *n = new_n;
    return b;
}

/* Дополнение меньшей матрицы нулями до new_n x new_n */
int** resize_matrix(int** A, int old_n, int new_n) {
    int** B = (int**)malloc(new_n * sizeof(int*));
    if (!B) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < new_n; ++i) {
        B[i] = (int*)calloc(new_n, sizeof(int));
        if (!B[i]) { perror("calloc"); exit(EXIT_FAILURE); }
        if (i < old_n) {
            for (int j = 0; j < old_n; ++j)
                B[i][j] = A[i][j];
        }
    }
    return B;
}

/* --- Функции для операций над двумя графами --- */
int** union_graph(int** G1, int n1, int** G2, int n2, int* out_n) {
    int new_n = (n1 > n2) ? n1 : n2;
    int** A = resize_matrix(G1, n1, new_n);
    int** B = resize_matrix(G2, n2, new_n);
    int** G = (int**)malloc(new_n * sizeof(int*));
    if (!G) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < new_n; ++i) {
        G[i] = (int*)malloc(new_n * sizeof(int));
        if (!G[i]) { perror("malloc"); exit(EXIT_FAILURE); }
        for (int j = 0; j < new_n; ++j)
            G[i][j] = (A[i][j] || B[i][j]) ? 1 : 0;
    }

    free_matrix(A, new_n);
    free_matrix(B, new_n);
    *out_n = new_n;
    return G;
}

int** intersection_graph(int** G1, int n1, int** G2, int n2, int* out_n) {
    int new_n = (n1 > n2) ? n1 : n2;
    int** A = resize_matrix(G1, n1, new_n);
    int** B = resize_matrix(G2, n2, new_n);
    int** G = (int**)malloc(new_n * sizeof(int*));
    if (!G) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < new_n; ++i) {
        G[i] = (int*)malloc(new_n * sizeof(int));
        if (!G[i]) { perror("malloc"); exit(EXIT_FAILURE); }
        for (int j = 0; j < new_n; ++j)
            G[i][j] = (A[i][j] && B[i][j]) ? 1 : 0;
    }

    free_matrix(A, new_n);
    free_matrix(B, new_n);
    *out_n = new_n;
    return G;
}

int** xor_graph(int** G1, int n1, int** G2, int n2, int* out_n) {
    int new_n = (n1 > n2) ? n1 : n2;
    int** A = resize_matrix(G1, n1, new_n);
    int** B = resize_matrix(G2, n2, new_n);
    int** G = (int**)malloc(new_n * sizeof(int*));
    if (!G) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < new_n; ++i) {
        G[i] = (int*)malloc(new_n * sizeof(int));
        if (!G[i]) { perror("malloc"); exit(EXIT_FAILURE); }
        for (int j = 0; j < new_n; ++j)
            G[i][j] = (A[i][j] != B[i][j]) ? 1 : 0;
    }

    free_matrix(A, new_n);
    free_matrix(B, new_n);
    *out_n = new_n;
    return G;
}

/* Декартово произведение графов */
int** cartesian_product(int** G1, int n1, int** G2, int n2, int* out_n) {
    int new_n = n1 * n2;
    int** G = (int**)malloc(new_n * sizeof(int*));
    if (!G) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < new_n; ++i) {
        G[i] = (int*)calloc(new_n, sizeof(int));
        if (!G[i]) { perror("calloc"); exit(EXIT_FAILURE); }
    }

    /* Правильное заполнение матрицы смежности */
    for (int u1 = 0; u1 < n1; ++u1) {
        for (int u2 = 0; u2 < n2; ++u2) {
            int u = u1 * n2 + u2;

            /* Соединяем с соседями по G1 (фиксируем u2, меняем v1) */
            for (int v1 = 0; v1 < n1; ++v1) {
                if (G1[u1][v1] && u1 != v1) {  // проверяем смежность 
                    int v = v1 * n2 + u2;
                    G[u][v] = G1[u1][v1];
                    G[v][u] = G1[u1][v1];
                }
            }

            /* Соединяем с соседями по G2 (фиксируем u1, меняем v2) */
            for (int v2 = 0; v2 < n2; ++v2) {
                if (G2[u2][v2] && u2 != v2) {  // проверяем смежность 
                    int v = u1 * n2 + v2;
                    G[u][v] = G2[u2][v2];
                    G[v][u] = G2[u2][v2];
                }
            }
        }
    }

    *out_n = new_n;
    return G;
}

/* Основная программа: интерактивное меню, можно делать операции бесконечно */
int main(void) {
    srand((unsigned)time(NULL));
    setlocale(LC_ALL, "rus");

    int n1 = 0, n2 = 0;
    int** M1 = NULL;
    int** M2 = NULL;

    while (1) {
        printf("=== Главное меню ===\n");
        printf("1 - Создать/пересоздать графы G1 и G2\n");
        printf("2 - Работа с одним графом (просмотр/отождествление/стягивание/расщепление)\n");
        printf("3 - Операции над двумя графами (объединение/пересечение/XOR/декартово произведение)\n");
        printf("4 - Показать текущие матрицы\n");
        printf("5 - Выход\n");

        int choice = read_positive_int("Ваш выбор: ");

        if (choice == 1) {
            if (M1) { free_matrix(M1, n1); M1 = NULL; n1 = 0; }
            if (M2) { free_matrix(M2, n2); M2 = NULL; n2 = 0; }
            n1 = read_positive_int("Введите число вершин для G1: ");
            n2 = read_positive_int("Введите число вершин для G2: ");
            M1 = generate_matrix(n1);
            M2 = generate_matrix(n2);
            print_matrix(M1, n1, "Матрица M1 (G1)");
            print_matrix(M2, n2, "Матрица M2 (G2)");
        }
        else if (choice == 2) {
            if (!M1 && !M2) {
                printf("Графы ещё не созданы. Выберите пункт 1 для создания графов.\n");
                continue;
            }
            int g_choice = read_positive_int("С каким графом хотите работать? (1 или 2): ");
            if (g_choice != 1 && g_choice != 2) { printf("Некорректный выбор графа.\n"); continue; }
            int** M = (g_choice == 2) ? M2 : M1;
            int* n = (g_choice == 2) ? &n2 : &n1;
            if (!M) { printf("Выбранный граф не создан.\n"); continue; }

            while (1) {
                printf("\n--- Работа с графом %d (n = %d) ---\n", g_choice, *n);
                printf("1 - Показать матрицу\n");
                printf("2 - Показать список смежности\n");
                printf("3 - Отождествление вершин\n");
                printf("4 - Стягивание ребра\n");
                printf("5 - Расщепление вершины\n");
                printf("6 - Вернуться в главное меню\n");

                int op = read_positive_int("Ваш выбор: ");
                if (op == 1) {
                    print_matrix(M, *n, "Текущая матрица");
                }
                else if (op == 2) {
                    int* deg = NULL;
                    int** adj = matrix_to_adjlist(M, *n, &deg);
                    print_adjlist(adj, deg, *n, "Список смежности");
                    free_adjlist(adj, deg, *n);
                }
                else if (op == 3) {
                    int v1 = read_positive_int("Введите первую вершину: ");
                    int v2 = read_positive_int("Введите вторую вершину: ");
                    if (v1 == v2 || v1 > *n || v2 > *n) printf("Некорректные вершины!\n");
                    else {
                        M = merge_vertices(M, n, v1, v2);
                        if (g_choice == 1) M1 = M; else M2 = M;
                    }
                }
                else if (op == 4) {
                    int v1 = read_positive_int("Введите первую вершину ребра: ");
                    int v2 = read_positive_int("Введите вторую вершину ребра: ");
                    if (v1 == v2 || v1 > *n || v2 > *n) printf("Некорректные вершины!\n");
                    else {
                        M = contract_edge(M, n, v1, v2);
                        if (g_choice == 1) M1 = M; else M2 = M;
                    }
                }
                else if (op == 5) {
                    int v = read_positive_int("Введите вершину для расщепления: ");
                    if (v > *n) printf("Некорректная вершина!\n");
                    else {
                        M = split_vertex(M, n, v);
                        if (g_choice == 1) M1 = M; else M2 = M;
                    }
                }
                else if (op == 6) {
                    break;
                }
                else {
                    printf("Неизвестная операция!\n");
                }
            }
        }
        else if (choice == 3) {
            if (!M1 || !M2) { printf("Оба графа должны быть созданы (пункт 1).\n"); continue; }
            int res_n = 0;
            printf("Выберите операцию над G1 и G2:\n");
            printf("1 - Объединение\n");
            printf("2 - Пересечение\n");
            printf("3 - Кольцевая сумма (XOR)\n");
            printf("4 - Декартово произведение\n");
            printf("5 - Отмена\n");
            int op2 = read_positive_int("Ваш выбор: ");
            int** G = NULL;

            if (op2 == 1) G = union_graph(M1, n1, M2, n2, &res_n);
            else if (op2 == 2) G = intersection_graph(M1, n1, M2, n2, &res_n);
            else if (op2 == 3) G = xor_graph(M1, n1, M2, n2, &res_n);
            else if (op2 == 4) G = cartesian_product(M1, n1, M2, n2, &res_n);
            else { printf("Отмена или неизвестная операция.\n"); }

            if (G) {
                print_matrix(G, res_n, "Результат операции над двумя графами");
                int* deg = NULL;
                int** adj = matrix_to_adjlist(G, res_n, &deg);
                print_adjlist(adj, deg, res_n, "Список смежности результата");
                free_adjlist(adj, deg, res_n);
                free_matrix(G, res_n);
            }
        }
        else if (choice == 4) {
            if (M1) print_matrix(M1, n1, "Матрица M1 (G1)"); else printf("M1 не создана\n");
            if (M2) print_matrix(M2, n2, "Матрица M2 (G2)"); else printf("M2 не создана\n");
        }
        else if (choice == 5) {
            printf("Выход...\n");
            break;
        }
        else {
            printf("Неизвестный режим работы!\n");
        }
    }

    if (M1)
        free_matrix(M1, n1);
    if (M2) free_matrix(M2, n2);
    return 0;
}