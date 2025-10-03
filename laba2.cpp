/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

double** create_matrix(int n) {
	double** matrix = (double**)malloc(n * sizeof(double*));
	for (int i = 0; i < n; i++) {
		matrix[i] = (double*)malloc(n * sizeof(double));
		for (int j = 0; j < n; j++) {
			matrix[i][j] = (double)rand() / RAND_MAX;
		}
	}
	return matrix;
}

void free_matrix(double** matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void matrix_multiply(double** A, double** B, double** C, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = 0;
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[j][k];
			}
		}
	}
}

int main() {

	setlocale(LC_ALL, "rus");
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	int sizes[] = { 100, 200, 400, 1000, 2000, 4000, 10000 };
	int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

	srand(time(NULL));


	for (int i = 0; i < num_sizes; i++) {
		int n = sizes[i];

		if (n > 4000) { // Для больших матриц предупреждение
			printf("Размер %d: очень долго, пропускаем...\n", n);
			continue;
		}

		printf("Создание матриц %dx%d...\n", n, n);
		double** A = create_matrix(n);
		double** B = create_matrix(n);
		double** C = create_matrix(n);
		
		printf("Умножение матриц %dx%d...\n", n, n);
		clock_t start = clock();
		matrix_multiply(A, B, C, n);
		clock_t end = clock();

		double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("Размер: %dx%d, Время: %.2f секунд\n\n", n, n, time_taken);

		free_matrix(A, n);
		free_matrix(B, n);
		free_matrix(C, n);
	}
	return 0;
}
*/