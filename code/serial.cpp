#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
using namespace std;

const int N = 9; // sudoku board size

// function to print the sudoku board
void print_board(int board[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// function to check if a number is safe to be placed at a certain cell
bool is_safe(int board[][N], int row, int col, int num) {
    // check row
    for (int j = 0; j < N; j++) {
        if (board[row][j] == num) {
            return false;
        }
    }
    // check column
    for (int i = 0; i < N; i++) {
        if (board[i][col] == num) {
            return false;
        }
    }
    // check box
    int box_size = sqrt(N);
    int box_row_start = row - row % box_size;
    int box_col_start = col - col % box_size;
    for (int i = box_row_start; i < box_row_start + box_size; i++) {
        for (int j = box_col_start; j < box_col_start + box_size; j++) {
            if (board[i][j] == num) {
                return false;
            }
        }
    }
    // number is safe
    return true;
}

// function to solve the sudoku board recursively
bool solve_board(int board[][N], int row, int col) {
    // if last row and last column reached, the board is solved
    if (row == N - 1 && col == N) {
        return true;
    }
    // if last column reached, move to the next row
    if (col == N) {
        row++;
        col = 0;
    }
    // if cell is already filled, skip it
    if (board[row][col] > 0) {
        return solve_board(board, row, col + 1);
    }
    // try to fill cell with values 1 to N
    for (int num = 1; num <= N; num++) {
        if (is_safe(board, row, col, num)) {
            board[row][col] = num;
            if (solve_board(board, row, col + 1)) {
                return true;
            }
            board[row][col] = 0; // backtrack
        }
    }
    // no value fits the cell
    return false;
}

int main(){

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    int board[N][N];

    ifstream infile("testcases/9x9_hard.txt");
    

    // Read the values from the file into the array
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            infile >> board[i][j];
        }
    }

    // Close the input file
    infile.close();

auto t1 = high_resolution_clock::now();

if (solve_board(board, 0, 0)) {
    // print the solution
    std::cout << "solution found." << std::endl;
} else {
    std::cout << "No solution found." << std::endl;
}

   auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

   std::cout << "\nexecution time : " << ms_double.count() << " ms\n";


   print_board(board);
}

