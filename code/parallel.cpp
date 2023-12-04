#include <stdio.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <omp.h>
using namespace std;

const int N = 16; // sudoku board size
bool done = false;
int solution[N][N];

// function to print the sudoku board
void print_board(int board[N][N]) {
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
bool solve_board(int board[N][N], int row, int col) {
    if (done == true){
        return false;
    }
    if (row == N - 1 && col == N) {
        return true;
    }
    if (col == N) {
        row++;
        col = 0;
    }
    if (board[row][col] > 0) {
        return solve_board(board, row, col + 1);
    }
    for (int num = 1; num <= N; num++) {
        if (is_safe(board, row, col, num)) {
#pragma omp task firstprivate(num, col, row, board)
            {
                int new_board[N][N];
                std::memcpy(new_board, board, sizeof(int) * N * N);
                new_board[row][col] = num;
                if (solve_board(new_board, row, col + 1)) {
#pragma omp critical
                    {
                        std::memcpy(solution, new_board, sizeof(int) * N * N);
                        done = true;
                    }
                }
            }
        }
    }
#pragma omp taskwait
    board[row][col] = 0;
    return false;
}

int main(){
	int nthreads = 0;

    omp_set_num_threads(16);

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    int board[N][N];

    // Open the input file
    ifstream infile("testcases/16x16_hard.txt");

    // Read the values from the file into the array
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            infile >> board[i][j];
        }
    }

    // Close the input file
    infile.close();

    // Print the array to check if it was read correctly
    // print_board(board);

auto t1 = high_resolution_clock::now();

#pragma omp parallel sections
{
    if (solve_board(board, 0, 0) || done) {
        std::cout << "solution: " << std::endl;
    } 
    else {
        std::cout << "No solution found." << std::endl;
    }
}
    auto t2 = high_resolution_clock::now();

    print_board(solution);

#pragma omp parallel 
		{
#pragma omp master
			nthreads = omp_get_num_threads();
		}

    duration<double, std::milli> ms_double = t2 - t1;

   std::cout << "\nexecution time : " << ms_double.count() << " ms\nnumber of threads : " << nthreads;

}


