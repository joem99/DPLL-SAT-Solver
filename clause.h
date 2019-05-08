//
// Created by 米昱文 on 2019-02-23.
// 该头文件构造求解器所需的子句结构及其相关操作
//

#ifndef SUDOKU2_CLAUSE_H
#define SUDOKU2_CLAUSE_H

typedef int lit;
//======================================================================
// 子句结构的实现

typedef struct clause {
    int size;// 该子句中的文字的个数
    int level;// 判断该子句为真时的决策层 初始化为-1表示该子句还没有满足
              // 主要用于回溯过程
    lit * lits;// 指向该子句中文字的指针（读取的时候用vec_i）
} clause;

//对子句的相关操作函数
static inline int clause_size(clause c) {
    return c.size;
}

static clause clause_intialize(lit * begin, lit * end);

#endif //SUDOKU2_CLAUSE_H
