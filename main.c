#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0
#define N 9

// Function to check if a number can be placed safely in the grid
int isSafe(int grid[N][N], int row, int col, int num) {
    // Check if the number is present in the same row
    for (int i = 0; i < N; i++) {
        if (grid[row][i] == num) {
            return 0;
        }
    }

    // Check if the number is present in the same column
    for (int i = 0; i < N; i++) {
        if (grid[i][col] == num) {
            return 0;
        }
    }

    // Check if the number is present in the same 3x3 box
    int boxRowStart = (row / 3) * 3;
    int boxColStart = (col / 3) * 3;
    for (int i = boxRowStart; i < boxRowStart + 3; i++) {
        for (int j = boxColStart; j < boxColStart + 3; j++) {
            if (grid[i][j] == num) {
                return 0;
            }
        }
    }

    return 1;
}

// Function to fill the Sudoku grid
int fillSudoku(int grid[N][N], int row, int col) {
    if (row == N - 1 && col == N) {
        return 1;
    }

    if (col == N) {
        row++;
        col = 0;
    }

    if (grid[row][col] != EMPTY) {
        return fillSudoku(grid, row, col + 1);
    }

    for (int num = 1; num <= N; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            if (fillSudoku(grid, row, col + 1)) {
                return 1;
            }

            grid[row][col] = EMPTY;
        }
    }

    return 0;
}

// Function to generate a random Sudoku grid with more empty cells
void generateSudoku(int grid[N][N]) {
    srand(time(NULL));

    // Fill the Sudoku grid using backtracking
    fillSudoku(grid, 0, 0);

    // Randomly remove more numbers to create empty cells
    int numToRemove = rand() % (N*N - 17) + 17; // Randomly select number of cells to remove, ensuring at least 17 cells remain filled
    for (int i = 0; i < numToRemove; i++) {
        int row = rand() % N;
        int col = rand() % N;
        while (grid[row][col] == EMPTY) { // Ensure not removing from an already empty cell
            row = rand() % N;
            col = rand() % N;
        }
        grid[row][col] = EMPTY;
    }
}

// Function to print the Sudoku grid with grids
void printGrid(int grid[N][N]) {
    printf("Generated Sudoku Grid:\n");
    // printf(" -----------------------\n");
    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == EMPTY) {
                printf(" .");
            } else {
                printf(" %d", grid[i][j]);
            }
            if ((j + 1) % 3 == 0 && j != N - 1) {
                printf("|");
            }
        }
        printf("|\n");
        if ((i + 1) % 3 == 0 && i != N - 1) {
            printf(" -----------------------\n");
        }
    }
    printf(" -----------------------\n");
}

int main() {
    int grid[N][N];

    // Initialize the grid with EMPTY values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = EMPTY;
        }
    }

    generateSudoku(grid);
    printGrid(grid);

    return 0;
}

