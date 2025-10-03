#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    char inf[256]; // �������� ����������
    int priority;  // ��������� �������� (��� ������ �����, ��� ���� ���������)
    struct node* next; // ������ �� ��������� �������
};

// ��������� �� ������ � ��������� �������� ������
struct node* head = NULL, * last = NULL;
int dlinna = 0;
char find_el[256];

// �������
struct node* get_struct(void);
void spstore(void);
void review(void);
void del(char* name);
struct node* find(char* name);

// ������� �������� �������� � �����������
struct node* get_struct(void)
{
    struct node* p = NULL;
    char s[256];
    int priority;

    if ((p = (struct node*)malloc(sizeof(struct node))) == NULL)
    {
        printf("������ ��� ������������� ������\n");
        exit(1);
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("������� �������� �������: ");
    fgets(s, sizeof(s), stdin);

    s[strcspn(s, "\n")] = 0;

    printf("������� ��������� ������� (����� �����): ");
    scanf("%d", &priority);

    if (strlen(s) == 0)
    {
        printf("������ �� ���� �����������\n");
        free(p);
        return NULL;
    }

    strcpy(p->inf, s);
    p->priority = priority;
    p->next = NULL;

    return p;
}

/* ���������� �������� */
void spstore(void)
{
    struct node* p = NULL;

    p = get_struct();
    if (p == NULL)
    {
        return;
    }

    // ���� ������ ����
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
    printf("������� �������� � ����� �������: %s (��������� %d)\n", p->inf, p->priority);
}

void spstec(void)
{
    struct node* p = NULL;

    p = get_struct();
    if (p == NULL)
    {
        return;
    }

    // ���� ������ ����
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
    printf("������� �������� � c���: %s (��������� %d)\n", p->inf, p->priority);
}

/* �������� ����������� ������ � ������������ */
void review(void)
{
    struct node* struc = head;
    if (head == NULL)
    {
        printf("������ ����\n");
        return;
    }

    printf("���������� ������:\n");
    while (struc)
    {
        printf("��� - %s, ��������� - %d\n", struc->inf, struc->priority);
        struc = struc->next;
    }
}

/* ����� �������� �� ����������� */
struct node* find(char* name)
{
    struct node* struc = head;
    if (head == NULL)
    {
        printf("������ ����\n");
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

    printf("������� �� ������\n");
    return NULL;
}

/* �������� �������� �� ����������� */
void del(char* name)
{
    struct node* struc = head;
    struct node* prev = NULL;
    int flag = 0;

    if (head == NULL)
    {
        printf("������ ����\n");
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
        printf("������� �� ������\n");
    }
}

// �������������� ������� ��� ��������� �������� � ��������� ����������� (��� ��������)
struct node* peek(void)
{
    return head; // ������ ������ ������ ����� ��������� ���������
}

// �������������� ������� ��� ���������� �������� � ��������� ����������� (� ���������)
struct node* dequeue(void)
{
    struct node* temp = head;

    if (head == NULL)
    {
        printf("������� �����\n");
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
        printf("������� �����\n");
        return;
    }
    struct node* temp = head;
    head = head->next;

    if (head == NULL) {
        last = NULL;
    }
    printf("������ �������: %s (��������� %d)\n", temp->inf, temp->priority);
    free(temp);
}


int main() {

    setlocale(LC_ALL, "rus");

    int choice;
    char name[256];
    struct node* temp;

    while (1)
    {
        printf("\n1. �������� ������� � ����� �������\n");
        printf("2. �������� ������� � ������ �������(����)\n");
        printf("3. ����������� �������\n");
        printf("4. ����� �������\n");
        printf("5. ������� �������\n");
        printf("6. �������� ������� � ��������� �����������\n");
        printf("7. ������� ������� � ��������� �����������\n");
        printf("8. ������� ������ ������� � �������\n");
        printf("9. �����\n");
        printf("�������� ��������: ");
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
            printf("������� ��� ��� ������: ");
            scanf("%s", name);
            temp = find(name);
            if (temp != NULL)
            {
                printf("������: %s � ����������� %d\n", temp->inf, temp->priority);
            }
            break;
        case 5:
            printf("������� ��� ��� ��������: ");
            scanf("%s", name);
            del(name);
            break;
        case 6:
            temp = peek();
            if (temp != NULL)
            {
                printf("������� � ��������� �����������: %s (���������: %d)\n",
                    temp->inf, temp->priority);
            }
            break;
        case 7:
            temp = dequeue();
            if (temp != NULL)
            {
                printf("�������� �������: %s (���������: %d)\n",
                    temp->inf, temp->priority);
                free(temp);
            }
            break;
        case 8:
            del_head();
            break;
        case 9:
            // ������� ������ ����� �������
            while (head != NULL)
            {
                temp = head;
                head = head->next;
                free(temp);
            }
            exit(0);
        default:
            printf("�������� �����\n");
        }
    }

    return 0;

}