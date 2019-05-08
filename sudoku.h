//
// Created by 米昱文 on 2019-02-27.
// 该头文件用来声明生成随机数独
// 并将挖洞后的数独文件转化cnf文件
//

#ifndef SUDOKU2_SUDOKU_H
#define SUDOKU2_SUDOKU_H

#include <string.h>
#include <time.h>
#include <stdio.h>
#include "solver.h"

// 该数组用来储存挖洞好的数独
int sudoku[9][9];

//=========================================
// 随机生成数独和挖洞的函数定义
void intialize_sudoku();
bool check(int sudoku[9][9]);
bool generate(int sudoku[9][9], int pos);
void dig_hole();
void print_sudoku(solver * s);

// 将数独转换成sat问题的操作函数
char * encoding();
void intialize();
char * getCNF();
#endif //SUDOKU2_SUDOKU_H
