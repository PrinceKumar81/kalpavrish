#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void generateMatrix(int **matrix, int n) {
    srand(time(NULL));
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            *(*(matrix + row) + col) = (rand() % 256);
        }
    }
}
void displayMatrix(int **matrix, int n, const char *title) {
    printf("\n%s:\n", title);
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            printf("%4d ", *(*(matrix + row) + col));
        }
        printf("\n");
    }
}
void transposeMatrix(int **matrix, int n) {
    for (int row = 0; row < n; row++) {
        for (int col = row + 1; col < n; col++) {
            int temp = *(*(matrix + row) + col);
            *(*(matrix + row) + col) = *(*(matrix + col) + row);
            *(*(matrix + col) + row) = temp;
        }
    }
}
void reverseRows(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        int *left = *(matrix + i);
        int *right = *(matrix + i) + (n - 1);
        while (left < right) {
            int temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
}
void rotateMatrix90Clockwise(int **matrix, int n) {
    printf("\nPerforming 90 Clockwise Rotation...");
    printf("\nStep 1: Transpose the matrix");
    transposeMatrix(matrix, n);
    displayMatrix(matrix, n, "After Transpose");
    printf("\nStep 2: Reverse each row");
    reverseRows(matrix, n);
}
void applySmoothingFilter(int **matrix, int n) {
    int **temp = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        temp[i] = (int *)malloc(n * sizeof(int));
    }
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            int sum = 0, count = 0;
            for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                for (int colOffset = -1; colOffset <= 1; colOffset++) {
                    int r = row + rowOffset;
                    int c = col + colOffset;
                    if (r >= 0 && r < n && c >= 0 && c < n) {
                        sum += *(*(matrix + r) + c);
                        count++;
                    }
                }
            }
            *(*(temp + row) + col) = sum / count;
        }
    }
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            *(*(matrix + row) + col) = *(*(temp + row) + col);
        }
    }
    for (int i = 0; i < n; i++) {
        free(temp[i]);
    }
    free(temp);
}
int main() {
    int n;
    printf("  SONAR IMAGE PROCESSING SYSTEM\n");
    printf("======================================\n");
    printf("\nEnter matrix size N (2 <= N <= 10): ");
    scanf("%d", &n);

    if (n < 2 || n > 10) {
        printf("Invalid input! N must be between 2 and 10.\n");
        return 1;
    }
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        *(matrix + i) = (int *)malloc(n * sizeof(int));
    }

    printf("\n1. Generating Random Sonar Image...");
    generateMatrix(matrix, n);
    displayMatrix(matrix, n, "Original Matrix (Sonar Intensity Values)");
    printf("\n2. Rotating Matrix 90 Clockwise...");
    rotateMatrix90Clockwise(matrix, n);
    displayMatrix(matrix, n, "After 90 Clockwise Rotation");
    printf("\n3. Applying 3x3 Smoothing Filter...");
    applySmoothingFilter(matrix, n);
    displayMatrix(matrix, n, "After Smoothing Filter");
    for (int i = 0; i < n; i++) {
        free(*(matrix + i));
    }
    free(matrix);
    return 0;
}
