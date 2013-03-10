/*
 * -*- coding: utf-8 -*-
 * Filename      : exit.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include "exit.h"

extern Result * result;

void
exit_func()
{
    g_assert(result != NULL);

    switch(result->code) {
        printf("Result: ");

        case EXIT_AC : printf("Accetped\n");
                       break;
        case EXIT_PE : printf("Presentation Error\n");
                       break;
        case EXIT_WA : printf("Wrong Answer\n");
                       break;
        case EXIT_RE : printf("Runtime Error\n");
                       break;
        case EXIT_IE : print("Internal Error\n");
                       break;
        case EXIT_TLE : printf("Time Limit Exceeded\n");
                        break;
        case EXIT_MLE : printf("Memory Limit Exceeded\n");
                        break;
        case EXIT_OLE : printf("Output Limit Exceeded\n");
                        break;
    }

    printf("Time: %d\n", result->time);
    printf("Memory: %d\n", result->memory);

    g_assert(result->msg != NULL);
    printf("Message: %s\n", result->msg->str);

    if (result->gerr == NULL)
        printf("Error: \n");
    else
        printf("Error: %s\n", result->gerr->message);

    exit(0);
}   
