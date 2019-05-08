#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include "sudoku.h"
#include <time.h>

bool main(int argc, char ** argv) {
    clock_t start, finish;
    double duration;
//    intialize_sudoku();
//    generate(sudoku, 0);
//    dig_hole();
//    encoding();
//    solver * s = solver_intialize();
    int op = 1;
    while (op) {
//        system("cls");
        printf("\n\n");
        printf("             Menu for Sudoku or SAT-solver \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. SolveSAT          \n");
        printf("    	  2. Sudoku       \n");
        printf("          3. Check       \n");
        printf("    	  0. EXIT\n     ");
        printf("-------------------------------------------------\n");
        printf("    Please choose your operation[0~15]:");
        scanf("%d", &op);
        char tempfile[1000];
        char * savefile;
        char filename[1000];
//        if (strlen(argv[1]) == 1) {
//            op = atoi(argv[1]);
//        }
//        else {
//            op = atoi(argv[2]);
//            memcpy(tempfile, argv[1], strlen(argv[1]) - 4);
//            strcpy(filename, argv[1]);
//        }
        switch (op) {
            case 1: {
                solver *s = solver_intialize();
                printf("Please input the filename:");
                scanf("%s", filename);
                memcpy(tempfile, filename, strlen(filename) - 4);
                savefile = strcat(tempfile, ".res");
                if (!readFile(filename, s, 1)) {
                    printf("Error!\n\n");
                    exit(1);
                }
                start = clock();
                printf("\nanalysis file: %s.txt\n", filename);
                if (DPLL_Solve(s)) {
                    printf("SATISFIABLE\n");
//                    printf("Please input the filename to save the result:");
//                    scanf("%s", savefile);
                    finish = clock();
                    duration = (double) (finish - start) / CLOCKS_PER_SEC;
                    print_solution(s, savefile, duration * 1000);
                } else {
                    printf("UNSAT\n");
//                    printf("Please input the filename to save the result:");
//                    scanf("%s", savefile);
                    finish = clock();
                    duration = (double) (finish - start) / CLOCKS_PER_SEC;
                    print_solution(s, savefile, duration * 1000);
                }
                break;
            }

                //随机生成数独，转换成cnf文件后保存到sudoku.cnf中并进行求解（执行这项功能时是有一个参数2）
            case 2: {
                solver *s = solver_intialize();
                intialize_sudoku();
                generate(sudoku, 0);
                dig_hole();
                encoding();
                start = clock();
                readFile("sudoku.cnf", s, 0);
                if (DPLL_Solve(s)) {
                    print_sudoku(s);
                    finish = clock();
                    duration = (double) (finish - start) / CLOCKS_PER_SEC;
                }
                break;
//                else {
//                    printf("UNSAT\n");
//                    finish = clock();
//                    duration = (double)(finish - start) / CLOCKS_PER_SEC;
//                } // 这个else应该可以去掉
                printf("\n%f ms\n", duration * 1000);
            }

                //这个功能第一个参数是保存cnf求解结果的res文件
            case 3: {
                solver *s = solver_intialize();
                printf("Please input the file of the result:");
                scanf("%s", filename);
//                scanf("%s", savefile);
                savefile = (char *) malloc((strlen(filename) - 4) * sizeof(char));
                memcpy(savefile, filename, strlen(filename) - 4);
                strcat(savefile, ".cnf");
                //第三个参数为0表示不需要生成相应的解析文件
                if (!readFile(savefile, s, 0)) {
                    printf("Error!\n\n");
                    exit(1);
                }
                if (check_result(s, filename)) {
                    printf("Right!");
                }

            }
            case 0: {
                break;
            }
        }
    }
    return 0;
}