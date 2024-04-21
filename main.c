#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>

#define N 9
#define EMPTY 0
#define EASY 35
#define MEDIUM 28
#define HARD 22
#define GIVE_UP 17
#define false 0
#define true 1

void printReadme(){

    printf("██████╗░███████╗░█████╗░██████╗░███╗░░░███╗███████╗\n");
    printf("██╔══██╗██╔════╝██╔══██╗██╔══██╗████╗░████║██╔════╝\n");
    printf("██████╔╝█████╗░░███████║██║░░██║██╔████╔██║█████╗░░\n");
    printf("██╔══██╗██╔══╝░░██╔══██║██║░░██║██║╚██╔╝██║██╔══╝░░\n");
    printf("██║░░██║███████╗██║░░██║██████╔╝██║░╚═╝░██║███████╗\n");
    printf("╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝╚═════╝░╚═╝░░░░░╚═╝╚══════╝\n");

    printf("\n\n-> This program Generates Random Sudoku.\n");
    printf("-> It has two types of functionalities.\n");
    printf("\t 1. Either Computer can show you solution of Randomly Generated Sudoku\n");
    printf("\t 2. Or You can solve Randomely Generated Sudoku\n");
    printf("\t\t - There are total four types of difficulty modes in this option.\n");
    printf("\t\t - Easy -> 35 Hints Given\n");
    printf("\t\t - Medium -> 28 Hints Given\n");
    printf("\t\t - Hard -> 22 Hints Given\n");
    printf("\t\t - GIVE_UP(Hardest) -> 17 Hints Given\n");
    printf("\t # After 10 minutes You will be given option to see solution.\n");

    initscr();
    start_color();
    init_pair(11,COLOR_CYAN,COLOR_BLUE);
    attron(COLOR_PAIR(11));
    printf("\n\n\n Press Enter to Generate Sudoku Grid....");
    attroff(COLOR_PAIR(11));
    refresh();
    endwin();

}

void clearScreen() {
#ifdef _WIN32
    system("cls"); // for Windows
#else
    system("clear"); // for Unix-like systems
#endif
}

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

// Function to find an empty cell in the Sudoku grid
int findEmptyCell(int grid[N][N], int *row, int *col) {
    for (*row = 0; *row < N; (*row)++) {
        for (*col = 0; *col < N; (*col)++) {
            if (grid[*row][*col] == EMPTY) {
                return 1; // Found an empty cell
            }
        }
    }

    *row = -1;
    *col = -1;
    return 0; // No empty cell found
}

// Function to solve the Sudoku grid
int solveSudoku(int grid[N][N]) {
    // Find an empty cell
    int row, col;
    if (!findEmptyCell(grid, &row, &col)) {
        // If no empty cell is found, the puzzle is solved
        return 1;
    }

    // Try placing numbers 1 to 9 in the empty cell
    for (int num = 1; num <= N; num++) {
        // Check if the number is safe to place in the current cell
        if (isSafe(grid, row, col, num)) {
            // Place the number in the cell
            grid[row][col] = num;

            // Recursively solve the rest of the puzzle
            if (solveSudoku(grid)) {
                return 1; // Puzzle is solved
            }

            // If solving the rest of the puzzle fails, backtrack and try the next number
            grid[row][col] = EMPTY;
        }
    }

    // If no number can be placed in the current cell, backtrack
    return 0;
}

// Function to generate a random solvable Sudoku puzzle
void generateSudoku(int grid[N][N]) {
    // Initialize the grid with empty cells
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = EMPTY;
        }
    }

    // Fill the diagonal cells with random numbers
    srand(time(NULL));
    for (int i = 0; i < N; i += 3) {
        for (int j = 0; j < N; j += 3) {
            int num;
            do {
                num = rand() % N + 1;
            } while (!isSafe(grid, i, j, num));
            grid[i][j] = num;
        }
    }

    // Solve the grid using backtracking
    solveSudoku(grid);
}

// Function to print the Sudoku grid with curses library
void printGrid(int grid[N][N], int row, int col, time_t start_time) {
    clear();
    initscr(); // Initialize curses mode
    start_color(); // Start color functionality
    cbreak(); // Disable line buffering
    keypad(stdscr, TRUE); // Enable keypad mode for capturing special keys

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Black text on yellow background
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // White text on blue background
    init_pair(3,COLOR_RED,COLOR_YELLOW); // Red text on yellow background for timer
    init_pair(4,COLOR_GREEN,COLOR_BLACK); // Green text on black background for | and -

    // Calculate elapsed time
    time_t current_time = time(NULL);
    int elapsed_time = (int)(current_time - start_time - 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "Elapsed Time: %02d:%02d", elapsed_time / 60, elapsed_time % 60);
    attroff(COLOR_PAIR(3));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == row && j == col) {
                attron(COLOR_PAIR(1)); // Enable color pair 1 (black text on yellow background)
                mvprintw(i * 2 + 1, j * 4, "%2d", grid[i][j]);
                attroff(COLOR_PAIR(1)); // Disable color pair 1
            } else {
                attron(COLOR_PAIR(2)); // Enable color pair 2 (white text on blue background)
                mvprintw(i * 2 + 1, j * 4, "%2d", grid[i][j]);
                attroff(COLOR_PAIR(2)); // Disable color pair 2
            }
            if ((j + 1) % 3 == 0 && j != N - 1) {
                attron(COLOR_PAIR(4));
                mvprintw(i * 2 + 1, (j + 1) * 4 - 2, " |");
                attroff(COLOR_PAIR(4));
            }
        }
        if ((i + 1) % 3 == 0 && i != N - 1) {
            for (int k = 0; k < N * 4 + 2; k++) {
                attron(COLOR_PAIR(4));
                mvprintw(i * 2 + 2, k, "-");
                attroff(COLOR_PAIR(4));
            }
        }
    }
    printw("\n--------------------------------");

    refresh(); // Refresh the screen
}

// Function to delete a specified number of elements from the Sudoku grid
void deleteElements(int grid[N][N], int numToDelete) {
    srand(time(NULL));
    int count = 0;
    while (count < numToDelete) {
        int row = rand() % N;
        int col = rand() % N;
        if (grid[row][col] != EMPTY) {
            grid[row][col] = EMPTY;
            count++;
        }
    }
}

int main() {
    int grid[N][N];
    char choiceHelper;
    unsigned int level = MEDIUM;
    unsigned int choice;
    int gridSol[N][N];
    unsigned int initChoice,secondChoice;
    char readme;
    time_t start_time;

   
    printf("\n****************************************************************\n");
    printf("███████╗██╗   ██╗██████╗        ██████╗       ██╗  ██╗██╗   ██╗\n");
    printf("██╔════╝██║   ██║██╔══██╗      ██╔═══██╗      ██║ ██╔╝██║   ██║\n");
    printf("███████╗██║   ██║██║  ██║█████╗██║   ██║█████╗█████╔╝ ██║   ██║\n");
    printf("╚════██║██║   ██║██║  ██║╚════╝██║   ██║╚════╝██╔═██╗ ██║   ██║\n");
    printf("███████║╚██████╔╝██████╔╝      ╚██████╔╝      ██║  ██╗╚██████╔╝\n");
    printf("╚══════╝ ╚═════╝ ╚═════╝        ╚═════╝       ╚═╝  ╚═╝ ╚═════╝ \n");
    printf("****************************************************************\n\n");
    printf("Hit enter to Generate Random Sudoku ..\n");
    printf("Press \'r\' to open readme Document\n");
    if( (readme = getchar()) == 'r') {
        clearScreen();
        printReadme();
        while ((getchar()) != '\n');  // Flush input buffer
        getchar();
    }
    initChoice = 1; 
    if(initChoice == 1){

    time(&start_time);
    generateSudoku(grid);

    for(int i = 0;i<N;i++){
        for(int j=0;j<N;j++){
            gridSol[i][j] = grid[i][j]; 
        }
    }

    deleteElements(grid, 81 - level);


        clearScreen();
        printf("# Generated Random Sudoku\n");
        printf("\t 1. Solve randomley Generated sudoku\n");
        printf("\t 2. Solve randomley Generated sudoku Manually\n");
        printf(">");
        scanf("%d",&secondChoice);

        if(secondChoice == 1){
            printGrid(grid,0,0,start_time);
            init_pair(5,COLOR_RED,COLOR_GREEN);
            init_pair(6,COLOR_GREEN,COLOR_RED);
            attron(COLOR_PAIR(5));
            printw("Generated Sudoku ^^^^ \n");    
            attroff(COLOR_PAIR(5));
            attron(COLOR_PAIR(6));
            printw("\nHit enter to see solution Sudoku :\n");
            attroff(COLOR_PAIR(6));
            getch();
            printGrid(gridSol,0,0,start_time);
            getch();
            endwin();

        }

        if(secondChoice == 2){

    clearScreen();
    
    printf(" █▀▄ █ █▀▀ █▀▀ █ █▀▀ █░█ █░░ ▀█▀ █▄█\n");
    printf(" █▄▀ █ █▀░ █▀░ █ █▄▄ █▄█ █▄▄ ░█░ ░█░\n\n");

    printf(
    "1. Easy\n"
    "2. Medium\n"
    "3. Hard\n"
    "4. GIVE UP !!\n"
    "----------------------------+\n");

    scanf("%d",&choice);
    while ((getchar()) != '\n');  // Flush input buffer

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

    // Print the modified Sudoku grid as a question to solve
  int row = 0, col = 0;

    // Print the Sudoku grid with curses library
    printGrid(grid, row, col,start_time);

    // Get user input for empty cells
    while (1) {
        int ch = getch(); // Get a character from the user

        switch (ch) {
            case KEY_UP:
                row = (row == 0) ? N - 1 : row - 1; // Move cursor up
                break;
            case KEY_DOWN:
                row = (row == N - 1) ? 0 : row + 1; // Move cursor down
                break;
            case KEY_LEFT:
                col = (col == 0) ? N - 1 : col - 1; // Move cursor left
                break;
            case KEY_RIGHT:
                col = (col == N - 1) ? 0 : col + 1; // Move cursor right
                break;
            case '\n':
                // Prompt the user to enter a digit for the current cell
                if (grid[row][col] == EMPTY) {
                    printw("Enter a digit (1-9) for cell (%d, %d): ", row+1, col+1);
                    refresh();
                    int digit;
                    scanw("%d", &digit);

                    // Validate user input
                       if (digit >= 1 && digit <= 9 && isSafe(grid, row, col, digit)) {
                        grid[row][col] = digit;
                        printGrid(grid,row,col,start_time);

                    } else {
                        printw("Invalid input! Please try again.");
                        refresh();
                    printGrid(grid,row,col,start_time);
                    }
                 
                } else {
                    printw("Cell (%d, %d) is already filled.", row+1, col+1);
                    refresh();
                    printGrid(grid,row,col,start_time);
                }
                break;
            case 'q': // Press 'q' to quit
                endwin(); // End curses mode
                return 0;
 }

    time_t current_time;
        time(&current_time);
        // start_time = current_time;
        if ((int)(current_time - start_time) >= 600) { // 600 seconds = 10 minutes
            // Prompt user if they want to see the solution
            printf("\nDo you want to see the solution? (y/n): ");
            char seeSolution;
            scanf("%c", &seeSolution);
            if (seeSolution == 'y' || seeSolution == 'Y') {
                // Show the solution
               printGrid(gridSol, 0, 0, start_time);
               while ((getchar()) != '\n');  // Flush input buffer
               printf("Solution displayed.\n");
               getchar();
               endwin();
               return 0;
            } else {
                // Continue with the game
                printf("Solution not displayed.\n");
                fflush(stdin); // Clear input buffer
            }
        }

        // Print the Sudoku grid with updated cursor position
        printGrid(grid, row, col,start_time);

    }
        }
    endwin();

    }

    endwin();
    return 0;
}
