//
// Created by 米昱文 on 2019-02-20.
// 提供求解器操作所需的数据结构,及其相关操作
//
// 存储文字的方法：对于文字lit，如果在子句中的形式是var，在子句的数据结构中则以2*(var-1)的形式存储它。
//                          如果在子句中的形式是-var，则以2*（var-1）+1的形式存储它。
// 按这种方式存储若原本子句集中有n个文字，现在变为最多2n个。
//
#ifndef SUDOKU2_SOLVER_H
#define SUDOKU2_SOLVER_H

#include "math.h"
#include "vector.h"
#include <stdio.h>
#include "clause.h"
//======================================================================

typedef int bool;
static const bool true = 1;
static const bool false = 0;

// 变量lbool用来表示文字的赋值情况
typedef char lbool;
static const lbool l_True = 1;
static const lbool l_False = -1;
static const lbool l_UnDefined = 0;


int varnum;
int clanum;
// 对文字处理的相关操作
typedef int lit;// lit表示文字在子句中的存储形式


static inline lit getLit(int var) {
    return 2 * (abs(var) - 1);
}
// l是var的存储形式， l ^ 1得到-var的存储形式neg_l
static inline lit lit_opposite(lit l) {
    return l ^ 1;
}
// 由lit得到原始变量(不考虑正负)
static inline int getVar(lit l) {
    return (l >> 1) + 1;
}

static inline int getSign(lit l) {
    if (l % 2 == 0) return 1;
    else return 0;
}
//======================================================================
// 求解器的结构实现
typedef struct solver {
    int size;// 整个子句集中文字的个数
//    int cap;// 文字的上限
    int cur_level;// 当前操作所在的决策层
    int tail;// 指向子句集中最后一个不满足的子句, 即前tail个子句都不可满足
    bool satisfied;// true表示该合取范式可满足，false表示该合取范式不满足
    bool * decisions; // 决策某变量var时，其对应的存储形式为lit，decisions[lit]设置为true
                      // 主要用来回溯
    lit * level_decision; // level_decision[i]表示第i个决策层上的决策变量
    lbool * assigns;// 对于文字的赋值assigns[lit] = 1 for true, -1 for false, 0 for undefined

    int * levels;// 每个lit被赋值时对应的赋值层
    int * counts;// 每个lit的个数

    vec_p clauses;// 该结构中包含 子句的个数 以及整个子句集
                  //
} solver;

//======================================================================
// 实现求解器的相关函数

// 初始化solver（从cnf文件中读取子句集信息到solver结构,创建solver中的子句集）
void addLit(int tempint, vec_i * lits);
bool readFile(char * filename, solver * s, int i);
solver * solver_intialize();
bool solver_addClause(solver * s, lit * begin, lit * end);
void solver_update(solver * s);


// 进行DPLL求解
bool DPLL_Solve(solver * s);
lit find_decision(solver * s);
bool Assign(solver * s, lit decision, bool new_level);
bool unit_propagate(solver * s);
bool find_unit(solver * s, lit * l);
bool BackTracking(solver * s, lit * decision);
lit BackTracking_OneLevel(solver * s);
void intialize_counts(solver * s);

// 将求解结果输出到文件中以及进行结果检验
void print_solution(solver * s, char * filename, double time);
bool check_result(solver * s, char * savefile);
void solver_delete(solver * s);



#endif //SUDOKU2_SOLVER_H
