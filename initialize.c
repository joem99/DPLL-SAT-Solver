//
// Created by 米昱文 on 2019-02-21.
// 这个文件用来进行DPLL处理子句集前的操作
// 包括从cnf文件中读取子句集的信息
// 初始化solver，根据从文件中读取的数据创建子句集
//


#include <stdlib.h>
#include "solver.h"
#include <string.h>

// 从cnf文件中读取子句集的信息,并生成解析文件
//i为1时生成解析文件
bool readFile(char * filename, solver * s, int i) {
//    s = solver_intialize();
    FILE * file;
    FILE * analysis;
    if (i) {
        char tempfile[1000];
        memcpy(tempfile, filename, strlen(filename) - 4);
        char * analysisfile = strcat(tempfile, ".txt");
        analysis = fopen(analysisfile, "w");
    }
    file = fopen(filename, "r");
    char temp;
    char temp2[5];
    int tempint;
    vec_i lits;// 用于存储一个子句中的文字
    veci_intialize(&lits);
    temp = fgetc(file);
    if (temp != 'c' && temp != 'p') return false;
    while (temp != 'p') {
        while (temp != '\n') {
            temp = fgetc(file);
        }
        temp = fgetc(file);
    }
    fgets(temp2, 5, file);
    fscanf(file, "%d", &s->size);
    fscanf(file, "%d", &s->tail);
    varnum = s->size;
    clanum = s->tail;
    solver_update(s);
    while (fscanf(file, "%d", &tempint) != EOF) {
        if (i) fprintf(analysis, "%d ", tempint);

        veci_clear(&lits);
        while (tempint != 0) {
            if (i) fprintf(analysis, "V %d ", tempint);
            addLit(tempint, &lits);
            fscanf(file, "%d", &tempint);
        }
        if (i) fprintf(analysis, "\n");
        lit * begin;
        begin = veci_begin(&lits);
        if (!solver_addClause(s, begin, begin + veci_size(&lits))) {
            veci_delete(&lits);
            return false;
        }
    }
    veci_delete(&lits);
    return true;
}

// 添加子句中的文字
void addLit(int tempint, vec_i * lits) {
    if (tempint > 0) veci_push(lits, getLit(tempint));
    else veci_push(lits, lit_opposite(getLit(tempint)));
}

// 对求解器结构进行初始化
solver * solver_intialize() {
    solver * s = (solver *) malloc(sizeof(solver));
    // 初始化solver的性质
    s -> size = 0;
//    s -> cap = 0;
    s -> tail = 0;
    s -> cur_level = -1;// 决策树的根层为0
    s -> satisfied = false;

    // 初始化数组变量
    s -> decisions = NULL;
    s -> level_decision = NULL;
    s -> assigns = NULL;
    s -> levels = NULL;
    s -> counts = NULL;

    vecp_intialize(&s->clauses);

    return s;
}

// 根据文件读取到的文字个数和子句个数为将s中的变量初始化
void solver_update(solver * s) {
    int var_num = s->size;
    int clause_num = s->tail;
    s->decisions = (bool *) malloc(sizeof(bool)*var_num*2);
    s->level_decision = (lit*) malloc(sizeof(lit)*clause_num);
    s->assigns = (lbool*) malloc(sizeof(lbool)*var_num*2);
    s->levels = (int *) malloc(sizeof(int)*var_num*2);
    s->counts = (int *) malloc(sizeof(int)*var_num*2);

    // 索引的最大值是(var_num-1)*2+1
    for (int i = 0; i < var_num*2; ++i) {
        s->decisions[i]       = false;
        s->level_decision[i]  = -1;
        s->assigns[i]         = l_UnDefined;
        s->levels[i]          = -1;
        s->counts[i]          = 0;
    }
}

// 读入文件添加子句
bool solver_addClause(solver * s, lit * begin, lit * end) {
    lit * i, *j;
    if (begin == end) return false;

    //在该子句中将lit由小到大排序
    for (i = begin + 1; i < end; ++i) {
        lit l = *i;
        for (j = i; j > begin && *(j - 1) > l; j--) {
                *j = *(j - 1);
        }
        *j = l;
    }
    // 在solver的子句集中添加新的子句
    vecp_push(&s->clauses, clause_intialize(begin, end));
    return true;
}

// 根据文件读入信息将该行子句初始化
clause clause_intialize(lit * begin, lit * end) {
    int size = (int)(end - begin);
    clause * c = (clause *)malloc(sizeof(clause));
    c -> lits = (lit *) malloc(sizeof(lit) * size);
    for (int i = 0; i < size; ++i) {
        c->lits[i] = begin[i];
    }

    c->size = size;
    c->level = -1;
    return (*c);
}