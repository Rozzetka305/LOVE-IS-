#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
//������� 5

struct student {
	char famil[50];
	char name[50];
	char facult[50];
	int Nomzach;
};

int main() {

	setlocale(LC_ALL, "rus");
	//������� 1
	/*
	int arr[15];
	srand(time(NULL));
	for (int i = 0; i < 15; i++) {
		arr[i] = rand() % 101 - 50;
	}
	printf("������ ");
	for (int i = 0; i < 15; i++) {
		printf("%d ", arr[i]);
	}
	int size = sizeof(arr) / sizeof(arr[0]);
	int max = arr[0];
	int min = arr[0];
	for (int i = 1; i < size; i++) {
		if (arr[i] < min) {
			min = arr[i];
		}
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	
	printf("\n����������� �������: %d", min);
	printf("\n������������ �������: %d", max);
	printf("\n�������: %d - %d = %d\n", max, min, max - min);
	*/

	//������� 2
	/*
	float arr[10], i;
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		int integer_part = (rand() % 10) - 5;
		double franctional_part = (double)rand() / RAND_MAX;
		arr[i] = integer_part + franctional_part;
	}
	printf("������ ");
	for (int i = 0; i < 10; i++) {
		printf("   %.1f ", arr[i]);
	}
	printf("\n");
	*/

	//������� 3
	/*
	int size;
	printf("������� ������ �������: ");
	while (scanf("%d", &size) != 1 or size <= 0) {
		while (getchar() != '\n');
		printf("������! ������� ����� ����� ������ 0: ");
	}
	if (size > 1000000) {
		printf("������� ������� ������! ��������: 1000000\n");
		return 1;
	}
	int* arr = (int*)malloc(size * sizeof(int));
	if (arr == NULL) {
		printf("������ ��������� ������!\n");
		return 1;
	}
	printf("������� %d ��������� �������:\n", size);
	for (int i = 0; i < size; i++) {
		printf("������� %d: ", i + 1);
		while (scanf("%d", &arr[i]) != 1 or size != '-0') {
			while (getchar() != '\n');
			printf("������! ������� ����� �����: ");
			printf("������� %d: ", i + 1);
		}
	}
	printf("\n��������� ������:\n");
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	free(arr);
	*/
	//������� 4
	/*
	int rows, cols;
	printf("������� ���������� �����: ");
	while (scanf("%d", &rows) != 1 or rows <= 0) {
		while (getchar() != '\n');
		printf("������! ������� ����� ����� ������ 0: ");
	}
	printf("������� ���������� ��������: ");
	while (scanf("%d", &cols) != 1 or cols <= 0) {
		while (getchar() != '\n');
		printf("������! ������� ����� ����� ������ 0: ");
	}
	int** arr = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		arr[i] = (int*)malloc(cols * sizeof(int));
	}
	srand(time(NULL));
	printf("\n��������������� ������ %dx%d:\n", rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			arr[i][j] = rand() % 100;
			printf("%4d", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n����� �� �������:\n");
	for (int i = 0; i < rows; i++) {
		int sum_row = 0;
		for (int j = 0; j < cols; j++) {
			sum_row += arr[i][j];
		}
		printf("������ %d: %d\n", i + 1, sum_row);
	}
	printf("\n����� �� �������� :\n");
	for (int j = 0; j < cols; j++) {
		int sum_col = 0;
		for (int i = 0; i < rows; i++) {
			sum_col += arr[i][j];
		}
		printf("������� %d: %d\n", j + 1, sum_col);
	}
	free(arr);
	*/

	//������� 5
	
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	int i;
	struct student stud[3];
	char search_famil[50];
	printf("������� ������ 3 ���������:\n");
	for (i = 0; i < 3; i++)
	{
		printf("\n������� %d:\n", i + 1);
		printf("�������: ");
		scanf("%49s", stud[i].famil);
		while (getchar() != '\n');
		printf("���: ");
		scanf("%49s", stud[i].name);
		while (getchar() != '\n');
		printf("���������: ");
		scanf("%49[^\n]", stud[i].facult);
		while (getchar() != '\n');
		printf("����� �������: ");
		while (scanf("%d", &stud[i].Nomzach) != 1) {
			while (getchar() != '\n');
			printf("������! ������� ����� �����: ");
		}
	}
	printf("\n������� ������� ��� ������: ");
	scanf("%49s", search_famil);
	int found = 0;
	printf("\n���������� ������:\n");
	for(i = 0; i < 3; i++)
	{
		if(strcmp(stud[i].famil, search_famil) == 0)
		{
			printf("������: %s %s, %s, ������� �%d\n",
				   stud[i].famil, stud[i].name, stud[i].facult, stud[i].Nomzach);
			found = 1;
		}
	}
	if(!found)
	{
		printf("������� � �������� '%s' �� ������\n", search_famil);
	}
	
	return 0;
}
