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

//��������: ������������� ����� ����� 
int is_positive_integer(const char* str) {
    if (str == NULL || *str == '\0')
        return 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (!isdigit(str[i])) return 0;
    int num = atoi(str);
    return num > 0;
}

//���������� ���� ���������� ������ 
int get_vertex_count() {
    char input[100];
    int num;
    while (1) {
        printf("\n������� ���������� ������ �����: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("������ ������ �����.\n");
            continue;
        }
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == '\n') {
                input[i] = '\0';
                break;
            }
        }
        if (!is_positive_integer(input)) {
            printf("������: ������� ������������� ����� �����!\n");
            continue;
        }
        num = atoi(input);
        if (num > MAX_VERTICES) {
            printf("������: �������� %d ������!\n", MAX_VERTICES);
            continue;
        }
        break;
    }
    return num;
}

//��������� ������������������ �����
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

//����� ������� ��������� 
void printAdjacencyMatrix() {
    printf("\n������� ��������� (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", adjMatrix[i][j]);
        printf("\n");
    }
}

//���������� ������� ���������
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

//����� ������� ��������� 
void printAdjacencyLists() {
    printf("\n������ ���������:\n");
    for (int i = 0; i < n; i++) {
        printf("%d: ", i);
        for (int j = 0; j < listSize[i]; j++)
            printf("%d ", adjList[i][j]);
        printf("\n");
    }
}

//����������� DFS �� ������� 
void DFS_matrix_recursive(int v) {
    visited[v] = 1;
    printf("%d ", v);

    for (int i = 0; i < n; i++) {
        if (adjMatrix[v][i] == 1 && !visited[i])
            DFS_matrix_recursive(i);
    }
}

//������������� DFS �� ������� 
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

//����� � ������� �� ������� (�����������) 
void POG_matrix_recursive() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("����������� ����� (������� ���������): ");
    for (int v = 0; v < n; v++)
        if (!visited[v])
            DFS_matrix_recursive(v);
    printf("\n");
}

//����� � ������� �� ������� (�������������) 
void POG_matrix_iterative() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("������������� ����� (������� ���������): ");
    for (int v = 0; v < n; v++)
        if (!visited[v])
            DFS_matrix_iterative(v);
    printf("\n");
}

//����������� DFS �� ������� ��������� 
void DFS_list_recursive(int v) {
    visited[v] = 1;
    printf("%d ", v);

    for (int i = 0; i < listSize[v]; i++) {
        int neighbor = adjList[v][i];
        if (!visited[neighbor])
            DFS_list_recursive(neighbor);
    }
}

//����� � ������� �� ������� ��������� 
void POG_list_recursive() {
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("����������� ����� (������ ���������): ");
    for (int v = 0; v < n; v++)
        if (!visited[v])
            DFS_list_recursive(v);
    printf("\n");
}

//���� 
int menu() {
    int choice;
    printf("\n===== ���� =====\n");
    printf("1. ������������� ����\n");
    printf("2. �������� ������� ���������\n");
    printf("3. ��������� � �������� ������ ���������\n");
    printf("4. ����� � ������� (�������, ����������)\n");
    printf("5. ����� � ������� (�������, ������������)\n");
    printf("6. ����� � ������� (������ ���������)\n");
    printf("7. �����\n");
    printf("�������� �����: ");
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
            printf("���� � %d ��������� ������� ������������!\n", n);
            break;

        case 2:
            if (!generated)
                printf("������� ������������ ����!\n");
            else
                printAdjacencyMatrix();
            break;

        case 3:
            if (!generated)
                printf("������� ������������ ����!\n");
            else {
                buildAdjacencyLists();
                printAdjacencyLists();
            }
            break;

        case 4:
            if (!generated)
                printf("������� ������������ ����!\n");
            else
                POG_matrix_recursive();
            break;

        case 5:
            if (!generated)
                printf("������� ������������ ����!\n");
            else
                POG_matrix_iterative();
            break;

        case 6:
            if (!generated)
                printf("������� ������������ ����!\n");
            else {
                buildAdjacencyLists();
                POG_list_recursive();
            }
            break;

        case 7:
            printf("����� �� ���������...\n");
            return 0;

        default:
            printf("������: �������� ����� �� 1 �� 7.\n");
        }
    }

    return 0;
}