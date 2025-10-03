#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    char inf[256]; // полезная информация
    int priority;  // приоритет элемента (чем больше число, тем выше приоритет)
    struct node* next; // ссылка на следующий элемент
};

// указатели на первый и последний элементы списка
struct node* head = NULL, * last = NULL;
int dlinna = 0;
char find_el[256];

// Функции
struct node* get_struct(void);
void spstore(void);
void review(void);
void del(char* name);
struct node* find(char* name);

// Функция создания элемента с приоритетом
struct node* get_struct(void)
{
    struct node* p = NULL;
    char s[256];
    int priority;

    if ((p = (struct node*)malloc(sizeof(struct node))) == NULL)
    {
        printf("Ошибка при распределении памяти\n");
        exit(1);
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Введите название объекта: ");
    fgets(s, sizeof(s), stdin);

    s[strcspn(s, "\n")] = 0;

    printf("Введите приоритет объекта (целое число): ");
    scanf("%d", &priority);

    if (strlen(s) == 0)
    {
        printf("Запись не была произведена\n");
        free(p);
        return NULL;
    }

    strcpy(p->inf, s);
    p->priority = priority;
    p->next = NULL;

    return p;
}

/* Добавление элемента */
void spstore(void)
{
    struct node* p = NULL;

    p = get_struct();
    if (p == NULL)
    {
        return;
    }

    // Если список пуст
    if (head == NULL)
    {
        head = p;
        last = p;
        return;
    }

    else
    {
        last->next = p;
        last = p;
    }
    printf("Элемент добавлен в конец очереди: %s (приоритет %d)\n", p->inf, p->priority);
}

void spstec(void)
{
    struct node* p = NULL;

    p = get_struct();
    if (p == NULL)
    {
        return;
    }

    // Если список пуст
    if (head == NULL)
    {
        head = p;
        last = p;
        return;
    }

    else
    {
        p->next = head;
        head = p;
    }
    printf("Элемент добавлен в cтек: %s (приоритет %d)\n", p->inf, p->priority);
}

/* Просмотр содержимого списка с приоритетами */
void review(void)
{
    struct node* struc = head;
    if (head == NULL)
    {
        printf("Список пуст\n");
        return;
    }

    printf("Содержимое списка:\n");
    while (struc)
    {
        printf("Имя - %s, Приоритет - %d\n", struc->inf, struc->priority);
        struc = struc->next;
    }
}

/* Поиск элемента по содержимому */
struct node* find(char* name)
{
    struct node* struc = head;
    if (head == NULL)
    {
        printf("Список пуст\n");
        return NULL;
    }

    while (struc)
    {
        if (strcmp(name, struc->inf) == 0)
        {
            return struc;
        }
        struc = struc->next;
    }

    printf("Элемент не найден\n");
    return NULL;
}

/* Удаление элемента по содержимому */
void del(char* name)
{
    struct node* struc = head;
    struct node* prev = NULL;
    int flag = 0;

    if (head == NULL)
    {
        printf("Список пуст\n");
        return;
    }

    if (strcmp(name, struc->inf) == 0)
    {
        flag = 1;
        head = struc->next;
        free(struc);
        struc = head;
    }
    else
    {
        prev = struc;
        struc = struc->next;
    }

    while (struc)
    {
        if (strcmp(name, struc->inf) == 0)
        {
            flag = 1;
            if (struc->next)
            {
                prev->next = struc->next;
                free(struc);
                struc = prev->next;
            }
            else
            {
                prev->next = NULL;
                last = prev;
                free(struc);
                return;
            }
        }
        else
        {
            prev = struc;
            struc = struc->next;
        }
    }

    if (flag == 0)
    {
        printf("Элемент не найден\n");
    }
}

// Дополнительная функция для получения элемента с наивысшим приоритетом (без удаления)
struct node* peek(void)
{
    return head; // Голова списка всегда имеет наивысший приоритет
}

// Дополнительная функция для извлечения элемента с наивысшим приоритетом (с удалением)
struct node* dequeue(void)
{
    struct node* temp = head;

    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return NULL;
    }

    head = head->next;
    if (head == NULL)
    {
        last = NULL;
    }

    temp->next = NULL;
    return temp;
}
void del_head(void) {

    if (head == NULL) {
        printf("Очередь пуста\n");
        return;
    }
    struct node* temp = head;
    head = head->next;

    if (head == NULL) {
        last = NULL;
    }
    printf("Удален элемент: %s (приоритет %d)\n", temp->inf, temp->priority);
    free(temp);
}


int main() {

    setlocale(LC_ALL, "rus");

    int choice;
    char name[256];
    struct node* temp;

    while (1)
    {
        printf("\n1. Добавить элемент в конец очереди\n");
        printf("2. Добавить элемент в начало очереди(стек)\n");
        printf("3. Просмотреть очередь\n");
        printf("4. Найти элемент\n");
        printf("5. Удалить элемент\n");
        printf("6. Показать элемент с наивысшим приоритетом\n");
        printf("7. Извлечь элемент с наивысшим приоритетом\n");
        printf("8. Удалить первый элемент в очереди\n");
        printf("9. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            spstore();
            break;
        case 2:
            spstec();
            break;
        case 3:
            review();
            break;
        case 4:
            printf("Введите имя для поиска: ");
            scanf("%s", name);
            temp = find(name);
            if (temp != NULL)
            {
                printf("Найден: %s с приоритетом %d\n", temp->inf, temp->priority);
            }
            break;
        case 5:
            printf("Введите имя для удаления: ");
            scanf("%s", name);
            del(name);
            break;
        case 6:
            temp = peek();
            if (temp != NULL)
            {
                printf("Элемент с наивысшим приоритетом: %s (приоритет: %d)\n",
                    temp->inf, temp->priority);
            }
            break;
        case 7:
            temp = dequeue();
            if (temp != NULL)
            {
                printf("Извлечен элемент: %s (приоритет: %d)\n",
                    temp->inf, temp->priority);
                free(temp);
            }
            break;
        case 8:
            del_head();
            break;
        case 9:
            // Очистка памяти перед выходом
            while (head != NULL)
            {
                temp = head;
                head = head->next;
                free(temp);
            }
            exit(0);
        default:
            printf("Неверный выбор\n");
        }
    }

    return 0;

}