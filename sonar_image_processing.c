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
    printf("\n Performing 90 Clockwise Rotation\n");
    printf("Transpose the matrix\n");
    transposeMatrix(matrix, n);
    displayMatrix(matrix, n, "After Transpose");
    printf("Step 2: Reverse each row\n");
    reverseRows(matrix, n);
}
void applySmoothingFilter(int **matrix, int n) {
    int *tempRow = (int *)malloc(n*sizeof(int));
    for(int row=0;row<n;row++)
    {
        for(int col=0;col<n;col++)
        {
            int sum=0;
            int count=0;
            for(int rowoffset=-1;rowoffset<=1;rowoffset++)
            {
                for(int coloffset=-1;coloffset<=1;coloffset++)
                {
                    int neighborRow =row+rowoffset;
                    int neighborCol =col+coloffset;
                    if(neighborRow>=0&&neighborRow<n&&neighborCol>=0&&neighborCol<n)
                    {
                        sum+=*(*(matrix+neighborRow)+neighborCol);
                        count++;
                    }
                }
            }
           *(tempRow+col) = sum/count;
        }
    for(int col = 0;col<n;col++)
    {
       *(*(matrix+row)+col) = *(tempRow+col);
    }
    }
    free(tempRow);
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
    printf("\nWe will First Generate Random Sonar Image\n");
    generateMatrix(matrix, n);
    displayMatrix(matrix, n, "Original Matrix (Sonar Intensity Values)");
    printf("\nSecond we will Rotating Matrix 90 Clockwise\n");
    rotateMatrix90Clockwise(matrix, n);
    displayMatrix(matrix, n, "After 90 Clockwise Rotation");
    printf("\n3rd we will Applying 3x3 Smoothing Filter\n");
    applySmoothingFilter(matrix, n);
    displayMatrix(matrix, n, "After Smoothing Filter");
    for (int i = 0; i < n; i++) {
        free(*(matrix + i));
    }
    free(matrix);
    return 0;
}