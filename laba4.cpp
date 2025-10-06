#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* root = NULL;

struct Node* CreateTree(struct Node* root, struct Node* r, int data)
{
    if (r == NULL)
    {
        r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL) {
            printf("������ ��������� ������");
            exit(0);
        }
        r->left = NULL;
        r->right = NULL;
        r->data = data;

        if (root == NULL) return r;

        if (data > root->data)
            root->right = r;   // ������� �������� -> ������
        else
            root->left = r;    // ������� �������� -> �����

        return r;
    }

    if (data > r->data)
        CreateTree(r, r->right, data);  // ��� � ������ ���������
    else
        CreateTree(r, r->left, data);   // ��� � ����� ���������

    return root;
}

void print_tree(struct Node* r, int l) {
    if (r == NULL) {
        return;
    }
    print_tree(r->right, l + 1);
    for (int i = 0; i < l; i++) {
        printf(" ");
    }
    printf("%d\n", r->data);
    print_tree(r->left, l + 1);
}

// ������� ������ �������� � ������
int search(struct Node* root, int value) {
    if (root == NULL) {
        return 0; // ������� �� ������
    }

    if (root->data == value) {
        return 1; // ������� ������
    }

    // ����������� ����� � ����� � ������ �����������
    if (value > root->data) {
        return search(root->left, value);
    }
    else {
        return search(root->right, value);
    }
}

// ������� �������� ����� ��������� �������� � ������
int countOccurrences(struct Node* root, int value) {
    if (root == NULL) {
        return 0;
    }

    int count = 0;

    // ���� ������� ���� �������� ������� ��������
    if (root->data == value) {
        count = 1;
    }

    // ���������� ������������ ��������� � ����� � ������ �����������
    // � �������� ������ ������ ���������� �������� ������ ��������� � ����� �����������
    if (value >= root->data) {
        count += countOccurrences(root->left, value);
    }
    if (value <= root->data) {
        count += countOccurrences(root->right, value);
    }

    return count;
}

int main() {
    setlocale(LC_ALL, "");
    char input[100];
    int D, start = 1;
    root = NULL;

    printf("������� ����� (����� 'stop' ��� ��������� �����):\n");

    // ���������� ������
    while (1) {
        printf("������� �����: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "stop") == 0) {
            printf("���������� ������ ��������\n\n");
            break;
        }

        if (sscanf(input, "%d", &D) == 1) {
            root = CreateTree(root, root, D);
        }
        else {
            printf("������! ������� ����� ��� '����'\n");
        }
    }

    // ����� ������
    printf("����������� ������:\n");
    print_tree(root, 0);
    printf("\n");

    // ����� ��������
    int searchValue;
    printf("������� �������� ��� ������: ");
    scanf("%d", &searchValue);

    if (search(root, searchValue)) {
        printf("������� %d ������ � ������.\n", searchValue);
    }
    else {
        printf("������� %d �� ������ � ������.\n", searchValue);
    }

    // ������� ���������
    int countValue;
    printf("������� �������� ��� �������� ���������: ");
    scanf("%d", &countValue);

    int occurrences = countOccurrences(root, countValue);
    printf("����� ��������� �������� %d � ������: %d\n", countValue, occurrences);

    return 0;
}

/*struct Node *CreateTree(struct Node *root, struct Node *r, int data)
{
    // �������� ��������� ����� ���������
    if (search(root, data)) {
        printf("����� %d ��� ���� � ������!\n", data);
        return root;
    }
    
    if (r == NULL)
    {
        // ������ ����� ���� (���������� ��� ���)
        r = (struct Node *)malloc(sizeof(struct Node));
        if (r == NULL) {
            printf("������ ��������� ������");
            exit(0);
        }
        r->left = NULL;
        r->right = NULL;
        r->data = data;
        
        if (root == NULL) return r;
        if (data > root->data)
            root->left = r;
        else 
            root->right = r;
        return r;
    }
    
    if (data > r->data)
        CreateTree(r, r->left, data);
    else
        CreateTree(r, r->right, data);
    
    return root;
}*/