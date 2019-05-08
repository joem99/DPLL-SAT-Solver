cmake_minimum_required(VERSION 3.13)
project(sudoku2 C)

set(CMAKE_C_STANDARD 11)

add_executable(sudoku2 main.c vector.h solver.c solver.h initialize.c clause.h sudokuToSAT.c generateSudoku.c sudoku.h)