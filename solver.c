//
// Created by 米昱文 on 2019-02-20.
// 实现一个基于DPLL算法的SAT求解器
// 主要函数是DPLL_solve
//
#include "solver.h"
#include <stdlib.h>
#include <stdio.h>

// 最主要的函数，进行DPLL求解
bool DPLL_Solve(solver * s) {
    lit decision;// 当前决策
    bool back = false;// 判断是否回溯，如果没有回溯，就要选择一个新的decision
                      // 没有回溯则不用再找一个decision（直接用回溯到的决策层的lit的neg）
    while (true) {
        if (!back) {
            // 选择该决策层的lit
            decision = find_decision(s);
        } else back = false;
        if (!Assign(s, decision, true)) {
            // 产生冲突
            if (!BackTracking(s, &decision)) {
                // 回溯失败
                return false;
            } else {
                // 回溯到了neg(lit)还没有选择过的一层
                back = true;
                continue;
            }
        }
        if (s->satisfied) {
            return true;
        } else {
            // 进行单子句传播
            if (!unit_propagate(s)) {
                // 单子句传播过程中出错
                if (!BackTracking(s, &decision)) {
                    return false;
                } else {
                    back = true;
                    continue;
                }
            }
        }
        if (s->satisfied) {
            return true;
        }
    }
}

// 最初从文件中读入数据时计数每个文字的个数
void intialize_counts(solver * s) {
    for (int i = 0; i < s->size * 2; ++i) {
        s->counts[i] = 0;
    }
}

// 该函数用来找到出现次数最多的lit
lit find_decision(solver * s) {
//    lit maxlit = -1;// maxlit表示数量最多的lit
//    int maxnum = -1;// maxnum表示最大的数量
//    for (int i = 0; i < s->size * 2; ++i) {
//        if(maxnum < s->counts[i]) {
//            maxnum = s->counts[i];
//            maxlit = i;
//        }
//    }
//    return maxlit;
//    clause temp;
//    for (int i = 0; i < s->tail; ++i) {
//        temp = vecp_begin(&s->clauses)[i];
//        if (temp.size == 2) {
//            return temp.lits[0];
//        }
//    }
    for (int i = 0; i < clause_size(vecp_begin(&s->clauses)[0]); ++i) {
        if (s->assigns[vecp_begin(&s->clauses)[0].lits[i]] == l_UnDefined) {
            return vecp_begin(&s->clauses)[0].lits[i];
        }
    }
//    if (s->assigns[vecp_begin(&s->clauses)[0].lits[0]] != l_UnDefined) {
//        printf("%d ", s->assigns[vecp_begin(&s->clauses)[0].lits[0]]);
//        printf("%d ", vecp_begin(&s->clauses)[0].lits[0]);
//        printf("%d ", s->assigns[vecp_begin(&s->clauses)[0].lits[1]]);
//        printf("%d ", vecp_begin(&s->clauses)[0].lits[1]);
//        return vecp_begin(&s->clauses)[0].lits[1];
//    }
//    return vecp_begin(&s->clauses)[0].lits[0];
}


// 对该层决策lit进行赋值，back == true意味着经过了回溯
bool Assign(solver * s, lit decision, bool new_level) {
    intialize_counts(s);
    clause temp;
    int count;// 用来计数一个子句中为false的个数
    int i, j;
    if (new_level) {
        s->cur_level ++;// 当前决策阶段的决策层数+1
        s->decisions[decision] = true;// 当前所选lit已被作为一层决策
        s->level_decision[s->cur_level] = decision;//该层的决策lit为decision
    }
    s->levels[decision] = s->cur_level;//该lit（decision）对应的决策层
    s->assigns[decision] = l_True;// 该decision的赋值为true
    s->assigns[lit_opposite(decision)] = l_False;// 该decision对应的neg赋值为false
    // 遍历整个还未满足子句的所有文字
    for (i = 0; i < s->tail; ++i) {
        temp = vecp_begin(&s->clauses)[i];//  temp是当前子句
        for (j = 0, count = 0; j < clause_size(temp); ++j) {
            if (s->assigns[temp.lits[j]] == l_False) {
                count ++;
            }
            if (s->assigns[temp.lits[j]] == l_UnDefined) {
                s->counts[temp.lits[j]] ++;
            }
            //如果该子句中有一个文字为真，则整个子句为真，需要改变tail，将该已满足的子句后移
            else if (s->assigns[temp.lits[j]] == l_True) {
                (&temp)->level = s->cur_level;// 该子句在该决策层被满足
                if (s->tail == 1) {
                    s->tail --;
                    s->satisfied = true;
                    return true;
                }
                vecp_begin(&s->clauses)[i] = vecp_begin(&s->clauses)[--s->tail];
                vecp_begin(&s->clauses)[s->tail] = temp;
                i--;// 此时把最后一个不满足的子句放到了temp的位置，所以要重新遍历该位置子句的文字
                break;
            }
        }// 说明该子句的所有文字都为假，所以整个范式为假，产生冲突
        if (count == clause_size(temp)) {
            return false;
        }
    }
    return true;
}

// 进行单子句传播
bool unit_propagate(solver * s) {
    lit l;// l用来储存单子句中的那个lit
    // 能找到单子句
    while (find_unit(s, &l)) {
        if (!Assign(s, l, false)) {
            return false;// 给单子句中的文字赋值后导致有子句不满足
        }
    }
    return true;
}

// 寻找子句集中的单子句
// 子句集中存在单子句返回true，否则返回false
bool find_unit(solver * s, lit * l) {
    clause temp;
    int count;// 用来计数子句中还没有赋值的文字
    int i, j;
    // 遍历所有还未满足的子句集
    for (i = 0; i < s->tail; ++i) {
        temp = vecp_begin(&s->clauses)[i];
        for (j = 0, count = 0; j < clause_size(temp); ++j) {
            if (s->assigns[temp.lits[j]] == l_UnDefined) {
                count ++;
                (*l) = temp.lits[j];
            }
        }// 找到了单子句
        if (count == 1) {
            return true;
        }
    }
    return false;
}


// 当产生冲突时进行回溯
// 当回溯成功时返回true，回溯到的决策层应为该层的原lit被赋值过，但它的neg还没有被赋值过
// 回溯失败时说明合取范式不满足，返回false
bool BackTracking(solver * s, lit * decision) {
    lit l;// l用来储存当前决策层的lit，然后用来判断l的neg是否被赋值过
    // 当前只对根决策层赋值过
    if (s->cur_level == 0 // 并且该文字正负都不可以，则说明该范式不满足
    && s->decisions[lit_opposite(s->level_decision[0]) == true]) {
        return false;
    }
    l = BackTracking_OneLevel(s);
    // 如果l和它的neg都被赋值过，说明还要再回溯一层
    while (s->decisions[l] == true && s->decisions[lit_opposite(l)] == true) {
        if (s->cur_level + 1 == 0) return false;
        s->decisions[l] = false;
        s->decisions[lit_opposite(l)] = false;
        l = BackTracking_OneLevel(s);
    }
    *decision = lit_opposite(l);
    return true;
}

// 向上回溯一层，即若在cur_level层的文字为lit，则恢复其为该lit赋值前的状态
// 返回值为回溯前的cur_level层的lit
lit BackTracking_OneLevel(solver * s) {
    clause temp;
    // 搜索所有lit，找到该决策层对应的lit
    for (int i = 0; i < s->size*2; ++i) {
        // 将求解器结构的相关变量修改到赋值前的状态
        if (s->levels[i] == s->cur_level) {
            s->assigns[i] = l_UnDefined;
            s->assigns[lit_opposite(i)] = l_UnDefined;
            s->levels[i] = -1;
//            s->level_decision[s->cur_level] = -1;
//            这里不对，因为下面还要用到这个决策层的decision
        }
    }
    // 遍历所有已满足的子句,找到在该决策层上因这次决策而满足的子句
    for (int i = s->tail; i < vecp_size(&s->clauses); ++i) {
        temp = vecp_begin(&s->clauses)[i];
        if (temp.level == s->cur_level) {
//            vecp_begin(&s->clauses)[i] = vecp_begin(&s->clauses)[s->tail];
//            vecp_begin(&s->clauses)[s->tail] = temp;
            s->tail++;
            (&temp)->level = -1;
        }
        // 这里可以break，因为后面的都不是在这层确认为真的了
        else break;
    }
    return s->level_decision[s->cur_level--];
}

// 用来输出合取范式的求解结果
void print_solution(solver * s, char * filename, double time) {
    int var;
    int value;
    FILE * file = fopen(filename, "w");
    fprintf(file, "%d\n", s->satisfied);
    if(s->satisfied) {
        for (int i = 0; i < s->size*2; i += 2) {
            var = getVar(i);
            value = (s->assigns[i] == l_True) ? 1 : 0;
            if (value == 1) {
                fprintf(file, "%d ", var);
            } else {
                fprintf(file, "-%d ", var);
            }
        }
    }

    fprintf(file, "\n%f ms\n", time);
    fclose(file);// 必须关闭文件，否则无法保存
}

// 通过读取res文件判断其是否为其相应cnf文件的解
bool check_result(solver * s, char * filename) {
    // 这里从文件中读取最好判断一下个数是否相符
    int val[500];
    int tempint;
    FILE * file = fopen(filename, "r");
//    FILE * resultfile = fopen("result.txt", "w");
    fscanf(file, "%d", &tempint);
    for (int i = 0; i < varnum; ++i) {
        fscanf(file, "%d", &tempint);
        val[i] = tempint > 0 ? 1 : 0;
    }
    bool result;
    for (int i = 0; i < clanum; ++i) {
        result = false;
        clause temp = vecp_begin(&s->clauses)[i];
        for (int j = 0; j < clause_size(temp); ++j) {
            if (getSign(temp.lits[j])) {
                if (val[getVar(temp.lits[j]) - 1]) {
                    result = true;
                    break;
                }
            } else {
                if (!val[getVar(temp.lits[j]) - 1]) {
                    result = true;
                    break;
                }
            }
        }
        if (!result) {
            printf("在第%d行出现错误\n", i + 1);
            break;
        }
    }
    return result;
}

// 删除求解器结构
void solver_delete(solver * s) {
    vecp_delete(&s->clauses);

    if (s->decisions != 0) {
        free(s->decisions);
        free(s->level_decision);
        free(s->assigns  );
        free(s->levels   );
        free(s->counts   );
    }

    free(s);
}