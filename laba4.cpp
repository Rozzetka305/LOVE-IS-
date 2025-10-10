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
            printf("Ошибка выделения памяти");
            exit(0);
        }
        r->left = NULL;
        r->right = NULL;
        r->data = data;

        if (root == NULL) return r;

        if (data > root->data)
            root->right = r;   // Большие значения -> ВПРАВО
        else
            root->left = r;    // Меньшие значения -> ВЛЕВО

        return r;
    }

    if (data > r->data)
        CreateTree(r, r->right, data);  // Идём в ПРАВОЕ поддерево
    else
        CreateTree(r, r->left, data);   // Идём в ЛЕВОЕ поддерево

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

// Функция поиска элемента в дереве
int search(struct Node* root, int value) {
    if (root == NULL) {
        return 0; // элемент не найден
    }

    if (root->data == value) {
        return 1; // элемент найден
    }

    // Рекурсивный поиск в левом и правом поддеревьях
    if (value > root->data) {
        return search(root->left, value);
    }
    else {
        return search(root->right, value);
    }
}

// Функция подсчета числа вхождений элемента в дерево
int countOccurrences(struct Node* root, int value) {
    if (root == NULL) {
        return 0;
    }

    int count = 0;

    // Если текущий узел содержит искомое значение
    if (root->data == value) {
        count = 1;
    }

    // Рекурсивно подсчитываем вхождения в левом и правом поддеревьях
    // В бинарном дереве поиска одинаковые значения обычно находятся в одном направлении
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

    printf("Вводите числа (слово 'stop' для окончания ввода):\n");

    // Построение дерева
    while (1) {
        printf("Введите число: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "stop") == 0) {
            printf("Построение дерева окончено\n\n");
            break;
        }

        if (sscanf(input, "%d", &D) == 1) {
            root = CreateTree(root, root, D);
        }
        else {
            printf("Ошибка! Введите число или 'стоп'\n");
        }
    }

    // Вывод дерева
    printf("Построенное дерево:\n");
    print_tree(root, 0);
    printf("\n");

    // Поиск элемента
    int searchValue;
    printf("Введите значение для поиска: ");
    scanf("%d", &searchValue);

    if (search(root, searchValue)) {
        printf("Элемент %d найден в дереве.\n", searchValue);
    }
    else {
        printf("Элемент %d не найден в дереве.\n", searchValue);
    }

    // Подсчет вхождений
    int countValue;
    printf("Введите значение для подсчета вхождений: ");
    scanf("%d", &countValue);

    int occurrences = countOccurrences(root, countValue);
    printf("Число вхождений элемента %d в дереве: %d\n", countValue, occurrences);

    return 0;
}

/*struct Node *CreateTree(struct Node *root, struct Node *r, int data)
{
    // ПРОВЕРКА ДУБЛИКАТА ПЕРЕД СОЗДАНИЕМ
    if (search(root, data)) {
        printf("Число %d уже есть в дереве!\n", data);
        return root;
    }
    
    if (r == NULL)
    {
        // Создаём новый узел (дубликатов уже нет)
        r = (struct Node *)malloc(sizeof(struct Node));
        if (r == NULL) {
            printf("Ошибка выделения памяти");
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