#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

/* ��������, ��� ������ ������� ������ �� ���� */
int is_valid_int(const char* s) {
    if (!s || *s == '\0') return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

/* ���������� �������������� ������ ����� � ��������� */
int read_positive_int(const char* prompt) {
    char buffer[128];
    long n;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "������ �����!\n");
            exit(EXIT_FAILURE);
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // ������ \n

        if (!is_valid_int(buffer)) {
            printf("������: ������� ������������� ����� �����!\n");
            continue;
        }

        n = atol(buffer);
        if (n <= 0) {
            printf("������: ����� ������ ���� ������ ����!\n");
            continue;
        }
        if (n > 1000000) { // ������ �� ��������� �������� �����
            printf("������: ������� ������� ����� ������!\n");
            continue;
        }
        return (int)n;
    }
}

/* ��������� n x n ������� (������������), ������� ����� */
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

/* ������ ������� n x n */
void print_matrix(int** a, int n, const char* name) {
    printf("%s (n = %d):\n", name, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* �������������� ������� � ������ ��������� */
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

/* ������ ������ ��������� */
void print_adjlist(int** adj, int* deg, int n, const char* name) {
    printf("%s (������ ���������):\n", name);
    for (int i = 0; i < n; ++i) {
        printf("%d:", i + 1);
        if (deg[i] == 0) printf(" (��� �������)");
        else for (int k = 0; k < deg[i]; ++k) printf(" %d", adj[i][k]);
        printf("\n");
    }
    printf("\n");
}

/* ������������ ������ */
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

/* (�) �������������� ������ (v1 � v2 � 1-based) */
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
                /* ���� ������/������� �������� v1 ��� v2 � ���������� ���� */
                if (i == v1 && j == v1) {
                    /* ����� �� ����������� �������: ���� ���� ����� � v1 ��� v2 */
                    val = (a[v1][v1] || a[v2][v2] || a[v1][v2]) ? 1 : 0;
                }
                else {
                    int orig_j = (j == v1) ? v1 : j;
                    /* ��������� ����� �� ����������� ������� � orig_j */
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

/* (�) ���������� ����� (v1 � v2 � 1-based) */
int** contract_edge(int** a, int* n, int v1, int v2) {
    if (!a) return NULL;
    v1--; v2--;
    if (v1 < 0 || v2 < 0 || v1 >= *n || v2 >= *n) return a;
    if (a[v1][v2] == 0) {
        printf("����� ����� ��������� %d � %d �����������!\n", v1 + 1, v2 + 1);
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

    // ������ ����� �������
    int** b = (int**)malloc(new_n * sizeof(int*));
    if (!b) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < new_n; ++i) {
        b[i] = (int*)calloc(new_n, sizeof(int));
        if (!b[i]) { perror("calloc"); exit(EXIT_FAILURE); }
    }

    // �������� ������ ������� � ������� ����� ����
    for (int i = 0; i < old_n; ++i)
        for (int j = 0; j < old_n; ++j)
            b[i][j] = a[i][j];

    int new_v = new_n - 1; // ������ ����� �������

    // ����� ������� ��������� ��� ����� ��������
    for (int j = 0; j < old_n; ++j) {
        if (a[v1][j]) {
            b[new_v][j] = 1;
            b[j][new_v] = 1;
        }
    }

    // ��������� ����� ����� �������� � ����� ���������
    b[v1][new_v] = 1;
    b[new_v][v1] = 1;

    // ���� � �������� ������� ���� ����� � ��������� � � �����
    if (a[v1][v1]) {
        b[new_v][new_v] = 1;
        b[v1][v1] = 1;
    }

    free_matrix(a, old_n);
    *n = new_n;
    return b;
}

/* ���������� ������� ������� ������ �� new_n x new_n */
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

/* --- ������� ��� �������� ��� ����� ������� --- */
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

/* ��������� ������������ ������ */
int** cartesian_product(int** G1, int n1, int** G2, int n2, int* out_n) {
    int new_n = n1 * n2;
    int** G = (int**)malloc(new_n * sizeof(int*));
    if (!G) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int i = 0; i < new_n; ++i) {
        G[i] = (int*)calloc(new_n, sizeof(int));
        if (!G[i]) { perror("calloc"); exit(EXIT_FAILURE); }
    }

    /* ���������� ���������� ������� ��������� */
    for (int u1 = 0; u1 < n1; ++u1) {
        for (int u2 = 0; u2 < n2; ++u2) {
            int u = u1 * n2 + u2;

            /* ��������� � �������� �� G1 (��������� u2, ������ v1) */
            for (int v1 = 0; v1 < n1; ++v1) {
                if (G1[u1][v1] && u1 != v1) {  // ��������� ��������� 
                    int v = v1 * n2 + u2;
                    G[u][v] = G1[u1][v1];
                    G[v][u] = G1[u1][v1];
                }
            }

            /* ��������� � �������� �� G2 (��������� u1, ������ v2) */
            for (int v2 = 0; v2 < n2; ++v2) {
                if (G2[u2][v2] && u2 != v2) {  // ��������� ��������� 
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

/* �������� ���������: ������������� ����, ����� ������ �������� ���������� */
int main(void) {
    srand((unsigned)time(NULL));
    setlocale(LC_ALL, "rus");

    int n1 = 0, n2 = 0;
    int** M1 = NULL;
    int** M2 = NULL;

    while (1) {
        printf("=== ������� ���� ===\n");
        printf("1 - �������/����������� ����� G1 � G2\n");
        printf("2 - ������ � ����� ������ (��������/��������������/����������/�����������)\n");
        printf("3 - �������� ��� ����� ������� (�����������/�����������/XOR/��������� ������������)\n");
        printf("4 - �������� ������� �������\n");
        printf("5 - �����\n");

        int choice = read_positive_int("��� �����: ");

        if (choice == 1) {
            if (M1) { free_matrix(M1, n1); M1 = NULL; n1 = 0; }
            if (M2) { free_matrix(M2, n2); M2 = NULL; n2 = 0; }
            n1 = read_positive_int("������� ����� ������ ��� G1: ");
            n2 = read_positive_int("������� ����� ������ ��� G2: ");
            M1 = generate_matrix(n1);
            M2 = generate_matrix(n2);
            print_matrix(M1, n1, "������� M1 (G1)");
            print_matrix(M2, n2, "������� M2 (G2)");
        }
        else if (choice == 2) {
            if (!M1 && !M2) {
                printf("����� ��� �� �������. �������� ����� 1 ��� �������� ������.\n");
                continue;
            }
            int g_choice = read_positive_int("� ����� ������ ������ ��������? (1 ��� 2): ");
            if (g_choice != 1 && g_choice != 2) { printf("������������ ����� �����.\n"); continue; }
            int** M = (g_choice == 2) ? M2 : M1;
            int* n = (g_choice == 2) ? &n2 : &n1;
            if (!M) { printf("��������� ���� �� ������.\n"); continue; }

            while (1) {
                printf("\n--- ������ � ������ %d (n = %d) ---\n", g_choice, *n);
                printf("1 - �������� �������\n");
                printf("2 - �������� ������ ���������\n");
                printf("3 - �������������� ������\n");
                printf("4 - ���������� �����\n");
                printf("5 - ����������� �������\n");
                printf("6 - ��������� � ������� ����\n");

                int op = read_positive_int("��� �����: ");
                if (op == 1) {
                    print_matrix(M, *n, "������� �������");
                }
                else if (op == 2) {
                    int* deg = NULL;
                    int** adj = matrix_to_adjlist(M, *n, &deg);
                    print_adjlist(adj, deg, *n, "������ ���������");
                    free_adjlist(adj, deg, *n);
                }
                else if (op == 3) {
                    int v1 = read_positive_int("������� ������ �������: ");
                    int v2 = read_positive_int("������� ������ �������: ");
                    if (v1 == v2 || v1 > *n || v2 > *n) printf("������������ �������!\n");
                    else {
                        M = merge_vertices(M, n, v1, v2);
                        if (g_choice == 1) M1 = M; else M2 = M;
                    }
                }
                else if (op == 4) {
                    int v1 = read_positive_int("������� ������ ������� �����: ");
                    int v2 = read_positive_int("������� ������ ������� �����: ");
                    if (v1 == v2 || v1 > *n || v2 > *n) printf("������������ �������!\n");
                    else {
                        M = contract_edge(M, n, v1, v2);
                        if (g_choice == 1) M1 = M; else M2 = M;
                    }
                }
                else if (op == 5) {
                    int v = read_positive_int("������� ������� ��� �����������: ");
                    if (v > *n) printf("������������ �������!\n");
                    else {
                        M = split_vertex(M, n, v);
                        if (g_choice == 1) M1 = M; else M2 = M;
                    }
                }
                else if (op == 6) {
                    break;
                }
                else {
                    printf("����������� ��������!\n");
                }
            }
        }
        else if (choice == 3) {
            if (!M1 || !M2) { printf("��� ����� ������ ���� ������� (����� 1).\n"); continue; }
            int res_n = 0;
            printf("�������� �������� ��� G1 � G2:\n");
            printf("1 - �����������\n");
            printf("2 - �����������\n");
            printf("3 - ��������� ����� (XOR)\n");
            printf("4 - ��������� ������������\n");
            printf("5 - ������\n");
            int op2 = read_positive_int("��� �����: ");
            int** G = NULL;

            if (op2 == 1) G = union_graph(M1, n1, M2, n2, &res_n);
            else if (op2 == 2) G = intersection_graph(M1, n1, M2, n2, &res_n);
            else if (op2 == 3) G = xor_graph(M1, n1, M2, n2, &res_n);
            else if (op2 == 4) G = cartesian_product(M1, n1, M2, n2, &res_n);
            else { printf("������ ��� ����������� ��������.\n"); }

            if (G) {
                print_matrix(G, res_n, "��������� �������� ��� ����� �������");
                int* deg = NULL;
                int** adj = matrix_to_adjlist(G, res_n, &deg);
                print_adjlist(adj, deg, res_n, "������ ��������� ����������");
                free_adjlist(adj, deg, res_n);
                free_matrix(G, res_n);
            }
        }
        else if (choice == 4) {
            if (M1) print_matrix(M1, n1, "������� M1 (G1)"); else printf("M1 �� �������\n");
            if (M2) print_matrix(M2, n2, "������� M2 (G2)"); else printf("M2 �� �������\n");
        }
        else if (choice == 5) {
            printf("�����...\n");
            break;
        }
        else {
            printf("����������� ����� ������!\n");
        }
    }

    if (M1)
        free_matrix(M1, n1);
    if (M2) free_matrix(M2, n2);
    return 0;
}