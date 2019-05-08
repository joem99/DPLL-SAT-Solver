//
// Created by 米昱文 on 2019-02-24.
// 该文件用来将数独问题转化成sat问题
// 目标是根据数独形式创建.cnf文件
// 参考文献：http://www.cs.cmu.edu/~hjain/papers/sudoku-as-SAT.pdf
//

#include "vector.h"
#include "sudoku.h"
#include <stdio.h>

/**
 * 三维数组var用来储存数独中每个位置的取值
 * 如：var[0][0][0]用数字1表示，它表示在第一个空取1
 */
int var[9][9][9];
vec_c clauses;

// 初始化var，用来保存数独变量的取值可能
void intialize() {
    int id = 0;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            for (int k = 0; k < 9; ++k) {
                var[i][j][k] = ++id;
            }
        }
    }
}

// 将数独转换成sat问题
char * encoding() {
    char * filename;
    intialize();
    vecc_intialize(&clauses);
    // 设定已赋值的元素， 即论文中的Assigned
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (sudoku[i][j] != 0) {
                vec_i clause;
                veci_intialize(&clause);
                veci_push(&clause, var[i][j][sudoku[i][j] - 1]);
                vecc_push(&clauses, clause);
            }
        }
    }

    // 每一行中必须有1-9个值， 即论文中的Rowd
    for (int i = 0; i < 9; ++i) {
        for (int k = 0; k < 9; ++k) {
            vec_i clause;
            veci_intialize(&clause);
            for (int j = 0; j < 9; ++j) {
                veci_push(&clause, var[i][j][k]);
            }
            vecc_push(&clauses, clause);
        }
    }

    // 每一列中必须有1-9个值， 即论文中的Cold
    for (int j = 0; j < 9; ++j) {
        for (int k = 0; k < 9; ++k) {
            vec_i clause;
            veci_intialize(&clause);
            for (int i = 0; i < 9; ++i) {
                veci_push(&clause, var[i][j][k]);
            }
            vecc_push(&clauses, clause);
        }
    }

    // 一个3 X 3区域内必须有1-9个值， 即论文中的Blockd
    for (int r = 0; r < 3; ++r) {
        for (int s = 0; s < 3; ++s) {
            for (int k = 0; k < 9; ++k) {
                vec_i clause;
                veci_intialize(&clause);
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        veci_push(&clause, var[r * 3 + i][s * 3 + j][k]);
                    }
                }
                vecc_push(&clauses, clause);
            }
        }
    }

    // 每一格只能取1-9中的一个，即论文中的Cellu
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            for (int k = 0; k < 9 - 1; ++k) {
                for (int m = k + 1; m < 9; ++m) {
                    vec_i clause;
                    veci_intialize(&clause);
                    veci_push(&clause, - var[i][j][k]);
                    veci_push(&clause, - var[i][j][m]);
                    vecc_push(&clauses, clause);
                }
            }
        }
    }
    // Celld
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            vec_i clause;
            veci_intialize(&clause);
            for (int k = 0; k < 9; ++k) {
                veci_push(&clause, var[i][j][k]);
            }
            vecc_push(&clauses, clause);
        }
    }

    //Rowu
    for (int i = 0; i < 9; ++i) {
        for (int k = 0; k < 9; ++k) {
            for (int j1 = 0; j1 < 8; ++j1) {
                for (int j2 = j1 + 1; j2 < 9; ++j2) {
                    vec_i clause;
                    veci_intialize(&clause);
                    veci_push(&clause, -var[i][j1][k]);
                    veci_push(&clause, -var[i][j2][k]);
                    vecc_push(&clauses, clause);
                }
            }
        }
    }

    //Colu
    for (int i = 0; i < 9; ++i) {
        for (int k = 0; k < 9; ++k) {
            for (int j1 = 0; j1 < 8; ++j1) {
                for (int j2 = j1 + 1; j2 < 9; ++j2) {
                    vec_i clause;
                    veci_intialize(&clause);
                    veci_push(&clause, -var[j1][i][k]);
                    veci_push(&clause, -var[j2][i][k]);
                    vecc_push(&clauses, clause);
                }
            }
        }
    }

//    //Blocku
//    for (int r = 0; r < 3; ++r) {
//        for (int c = 0; c < 3; ++c) {
//            for (int k = 0; k < 9; ++k) {
//                for (int i = 0; i < 9; ++i) {
//                    for (int j = i + 1; j < 9; ++j) {
//                        vec_i clause;
//                        veci_intialize(&clause);
//                        veci_push(&clause, -var[r * 3 + i / 3][c * 3 + i % 3][k]);
//                        veci_push(&clause, -var[r * 3 + j / 3][c * 3 + j % 3][k]);
//                        vecc_push(&clauses, clause);
//                    }
//                }
//            }
//        }
//    }

    filename = getCNF();
    return filename;
}

// 将数独转换成的sat问题存储到文件"sudoku.cnf中"
char * getCNF() {
    char * filename = "sudoku.cnf";
    FILE * cnf;
    vec_i temp;
    cnf = fopen(filename, "w");
    fprintf(cnf, "p cnf %d %d", 9 * 9 * 9, vecc_size(&clauses));
    fprintf(cnf, "\n");
    for (int i = 0; i < vecc_size(&clauses); ++i) {
        temp = clauses.clause[i];
        for (int j = 0; j < veci_size(&temp); ++j) {
            fprintf(cnf,"%d ", temp.p[j]);
        }
        fprintf(cnf, "0\n");
        veci_delete(&temp);
    }
    free(clauses.clause);
    fclose(cnf);
    return filename;
}

