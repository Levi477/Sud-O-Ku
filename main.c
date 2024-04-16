#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0
#define N 9
#define EASY 35
#define MEDIUM 28
#define HARD 22
#define GIVE_UP 17

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
// Function to generate a random Sudoku grid with exactly 17 hints
void generateSudoku(int grid[N][N],int level) {
    srand(time(NULL));

    // Fill the Sudoku grid using backtracking
    fillSudoku(grid, 0, 0);

    // Count the number of filled cells
    int filledCount = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != EMPTY) {
                filledCount++;
            }
        }
    }

    // Randomly remove excess numbers to create exactly 17 hints
    int numToRemove = filledCount - level; // Calculate the number of cells to remove
    while (numToRemove > 0) {
        int row = rand() % N;
        int col = rand() % N;
        if (grid[row][col] != EMPTY) { // If the cell is not empty
            grid[row][col] = EMPTY; // Remove the number
            numToRemove--; // Decrement the count of cells to remove
        }
    }
}


// Function to print the Sudoku grid with grids
void printGrid(int grid[N][N]) {
    printf("Generated Sudoku Grid:\n");
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
    unsigned int level = MEDIUM;
    unsigned int choice;
 
    printf("\n****************************************************************\n");
    printf("███████╗██╗   ██╗██████╗        ██████╗       ██╗  ██╗██╗   ██╗\n");
    printf("██╔════╝██║   ██║██╔══██╗      ██╔═══██╗      ██║ ██╔╝██║   ██║\n");
    printf("███████╗██║   ██║██║  ██║█████╗██║   ██║█████╗█████╔╝ ██║   ██║\n");
    printf("╚════██║██║   ██║██║  ██║╚════╝██║   ██║╚════╝██╔═██╗ ██║   ██║\n");
    printf("███████║╚██████╔╝██████╔╝      ╚██████╔╝      ██║  ██╗╚██████╔╝\n");
    printf("╚══════╝ ╚═════╝ ╚═════╝        ╚═════╝       ╚═╝  ╚═╝ ╚═════╝ \n");
    printf("****************************************************************\n\n");

    printf(" █▀▄ █ █▀▀ █▀▀ █ █▀▀ █░█ █░░ ▀█▀ █▄█\n");
    printf(" █▄▀ █ █▀░ █▀░ █ █▄▄ █▄█ █▄▄ ░█░ ░█░\n\n");

    printf(
    "1. Easy\n"
    "2. Medium\n"
    "3. Hard\n"
    "4. GIVE UP !!\n"
    "----------------------------\n"
                );

    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        level = EASY;
        printf("Difficulty Set to Easy.\n");
        break;
    case 2:
        level = MEDIUM;
        printf("Difficulty Set to Medium.\n");
        break;
    case 3:
        level = HARD;
        printf("Difficulty Set to Hard.\n");
        break;
    case 4:
        level = GIVE_UP;
        printf("Difficulty Set to VERY HARD.\n");
        break;    
    default:
        level = MEDIUM;
        printf("Invalid Input!\n");
        printf("Difficulty Set to Medium.\n");
        break;
    }

    

    // Initialize the grid with EMPTY values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = EMPTY;
        }
    }

    generateSudoku(grid,level);
    printGrid(grid);

    return 0;
}
