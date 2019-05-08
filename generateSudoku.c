//
// Created by 米昱文 on 2019-02-25.
// 用来随机生成数独并进行挖空
// 挖好空的数独保存在数组sudoku[9][9]中
//

#include "sudoku.h"
#include "solver.h"

// 初始化9*9的数组
void intialize_sudoku() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            sudoku[i][j] = 0;
        }
    }
}

// 检查生成的数独是否符合数独规范
// 若sudoku是符合规范的数独，返回true
// 否则返回false
bool check(int sudoku[9][9]) {
    int used[9 + 1];// 该数组用来储存1-9个数字是否出现过
    // 未出现过为0，出现之后置为1

    // 检查一行中是否存在重复数字
    for (int i = 0; i < 9; ++i) {
        memset(used, 0, sizeof(used));
        for (int j = 0; j < 9; ++j) {
            int val = sudoku[i][j];
            if (used[val])
                return false;
            if (val)
                used[val] = 1;
        }
    }

    // 检查一列中是否存在重复数字
    for (int j = 0; j < 9; ++j) {
        memset(used, 0, sizeof(used));
        for (int i = 0; i < 9; ++i) {
            int val = sudoku[i][j];
            if (used[val])
                return false;
            if (val)
                used[val] = 1;
        }
    }

    // 检查一个3 X 3 区域内是否存在重复数字
    // block表示第几个区域
    for (int block = 0; block < 9; ++block) {
        int r = block / 3, c = block % 3;
        memset(used, 0, sizeof(used));
        for (int m = 0; m < 9; ++m) {
            int i = m / 3, j = m % 3;
            int val = sudoku[3 * r + i][3 * c + j];
            if (used[val])
                return false;
            if (val)
                used[val] = 1;
        }
    }
    return true;
}

// 随机生成数独
bool generate(int sudoku[9][9], int pos) {
    int r = pos / 9, c = pos % 9;

    if (pos == 9 * 9)
        return true;
    if (sudoku[r][c])
        return generate(sudoku, pos + 1);

    // generate a random permutation of [1 .. 9]
    int random[9];
    for (int i = 0; i < 9; ++i)
        random[i] = i + 1;//random是1-9

    srand(time(NULL));
    //打乱随机数的顺序
    for (int i = 0; i < 9; ++i) {
        int j = rand() % 9;
        int tmp = random[i];
        random[i] = random[j];
        random[j] = tmp;
    }

    for (int i = 0; i < 9; ++i) {
        sudoku[r][c] = random[i];
        if (check(sudoku) && generate(sudoku, pos + 1)) {
            return true;
        }
    }
    sudoku[r][c] = 0;
    return false;
}

// 挖洞法将完整的数独挖洞，将挖洞后的数独保存到sudoku1.txt中
void dig_hole() {
//    FILE * create_sudoku = fopen("sudoku1.txt", "w");
    int holes = 81 - 30;// holes是挖掉的洞数
    srand(time(NULL));
    //随机删除holes个数
    for (int i = 1; i <= holes; i++) {
        int a = rand() % 81;
        int r = a / 9;
        int c = a % 9;
        if (sudoku[r][c] != 0)
            sudoku[r][c] = 0;
        else
            i--;
    }
    // 输出挖洞后的数独信息
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (i % 3 == 0 && j == 0 && i != 0) {
                printf("---------------------\n");
            }
            if (sudoku[i][j] == 0) {
                printf(" _");
            } else {
                printf(" %d", sudoku[i][j]);
            }
            if ((j + 1) % 3 == 0 && j != 8) {
                printf(" |");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

// 打印求解之后的数独，将完整数独保存到sudoku2.txt中
void print_sudoku(solver * s) {
//    FILE * result_sudoku = fopen("sudoku2.txt", "w");
    int value[9][9];
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            for (int var = i * 9 * 9 + j * 9 + 1; var <= i * 9 * 9 + j * 9 + 9; ++var) {
                if (s->assigns[getLit(var)] == l_True) {
                    value[i][j] = var % 9;
                    if (value[i][j] == 0) value[i][j] = 9;
                }
            }
        }
    }
    printf("数独结果:\n");
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (i % 3 == 0 && j == 0 && i != 0) {
                printf( "---------------------\n");
            }
            printf(" %d", value[i][j]);
            if ((j + 1) % 3 == 0 && j != 8) {
                printf(" |");
            }
        }
        printf("\n");
    }
}