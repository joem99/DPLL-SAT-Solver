//
// Created by 米昱文 on 2019-02-20.
// 使用长度可变的数组来储存子句和文字
//  vec_i用来储存文字， vec_p用来储存子句集。
//

#ifndef SUDOKU2_VECTOR_H
#define SUDOKU2_VECTOR_H

#include <stdlib.h>
#include "clause.h"

//==========================================================================
// vec_i 用来储存子句的文字
typedef struct vec_i {
    int size;// 子句中文字的个数
    int cap;// 储存上限
    int * p;// 指向第一个文字的指针
} vec_i;

static inline void veci_intialize(vec_i * v) {
    v -> size = 0;
    v -> cap = 10;
    v -> p = (int *) malloc(sizeof(int) * v -> cap);
}

static inline void veci_delete(vec_i * v) {
    free(v -> p);
}

static inline int * veci_begin(vec_i * v) {
    return v -> p;
}

static inline int veci_size(vec_i * v) {
    return v -> size;
}

static inline void veci_clear(vec_i * v) {
    v -> size = 0;
}

static inline void veci_push(vec_i * v, int data) {
    if (v -> size == v -> cap) {
        int newcap = (v -> cap) * 2;
        v -> p = (int *) realloc(v -> p, sizeof(int) * newcap);
        v -> cap = newcap;
    }
    v -> p[v -> size++] = data;
}

//==========================================================================
// vec_p用来储存子句
typedef struct vec_p {
    int size;// 子句集中子句的个数
    int cap;
    clause * p;// 指向第一个子句
} vec_p;

static inline void vecp_intialize(vec_p * v) {
    v -> size = 0;
    v -> cap = 10;
    v -> p = (clause *) malloc(sizeof(clause) * v->cap);
}

static inline void vecp_delete(vec_p * v) {
    free(v -> p);
}

static inline clause * vecp_begin(vec_p * v) {
    return v -> p;
}

static inline int vecp_size(vec_p * v) {
    return v -> size;
}

static inline void vecp_push(vec_p * v, clause c) {
    if (v -> size == v -> cap) {
        int newcap = v -> cap * 2;
        v -> p = (clause *) realloc(v -> p, sizeof(clause) * newcap);
        v -> cap = newcap;
    }
    v -> p[v -> size++] = c;
}

//==========================================================================
// 用来储存数独转sat时的子句

typedef struct vec_c {
    int size;
    int cap;
    vec_i * clause;
} vec_c;

static inline void vecc_intialize(vec_c * v) {
    v -> size = 0;
    v -> cap = 10;
    v -> clause = (vec_i*) malloc(sizeof(vec_i) * v -> cap);
}

static inline void vecc_push(vec_c * v, vec_i vi) {
    if (v -> size == v -> cap) {
        int newcap = v -> cap * 2;
        v -> clause = (vec_i*) realloc(v -> clause, sizeof(vec_i) * newcap);
        v -> cap = newcap;
    }
    v -> clause[v -> size++] = vi;
}

static inline int vecc_size(vec_c * v) {
    return v->size;
}
#endif //SUDOKU2_VECTOR_H

