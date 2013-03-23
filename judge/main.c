/*
 * -*- coding: utf-8 -*-
 * Filename      : main.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-11
 */

#include "global.h"

int ltime;                  /* 时间限制 */
int fsize;                  /* 输出限制 */
int memory;                 /* 内存限制 */
int preused;                /* 解析器内存 */

GString * input;            /* 输入文件 */
GString * output;           /* 输出文件 */
GString * answer;           /* 答案文件 */

char * cfgfile;             /* 配置文件 */
char * workdir;             /* 工作目录 */
char * datadir;             /* 数据目录 */
char * lang;                /* 程序语言 */
char * const * command;     /* 被测程序 */

/* 从配置文件转换而来适合程序使用的信息 */
int signal_rule[SIGNAL_NUM];
gboolean syscall_rule[SYSCALL_NUM];
GSList * resource_rule;
GSList * environ_rule;
const char * feedback[SIGNAL_NUM];

Result * result;            /* 评测结果 */

static void init_varible(); /* 全局变量初始化 */
static void exit_func();    /* 本程序唯一出口 */

int
main(int argc, char *argv[])
{
    struct stat sbuf;
    int max_time, max_memory;

    init_varible();
    max_time = max_memory = 0;

    if (!parse_cmdline(&argc, &argv) || !parse_keyfile() || !parse_data())
        exit_func();

    /* 多个输入用例，多次运行程序 */
    while (next_data()) {
        execute_command();
        result->code != EXIT_AC ? unlink(output->str), exit_func() : 1;

        stat(answer->str, &sbuf);
        sbuf.st_mode & S_IXOTH ? special_judge() : normal_judge();
        result->code != EXIT_AC ? unlink(output->str), exit_func() : 1;

        max_time = MAX(max_time, result->time);
        max_memory = MAX(max_memory, result->memory);
        unlink(output->str);
    }

    result->time = max_time;
    result->memory = max_memory;
    exit_func();

    return 0;
}

static void
init_varible()
{
    input = g_string_new("");
    output = g_string_new("");
    answer = g_string_new("");
    result = (Result *)malloc(sizeof(Result));
    g_assert(input && output && answer && result);

    result->code = EXIT_IE;
    result->time = result->memory = 0;
    result->msg = g_string_new("");
    result->err = g_string_new("");
    g_assert(result->msg && result->err);
}

static void
exit_func()
{
    printf("result:");
    switch(result->code) {
        case EXIT_AC : printf("Accetped\n");
                       break;
        case EXIT_PE : printf("Presentation Error\n");
                       break;
        case EXIT_WA : printf("Wrong Answer\n");
                       break;
        case EXIT_RE : printf("Runtime Error\n");
                       break;
        case EXIT_IE : printf("Internal Error\n");
                       break;
        case EXIT_TLE : printf("Time Limit Exceeded\n");
                        break;
        case EXIT_MLE : printf("Memory Limit Exceeded\n");
                        break;
        case EXIT_OLE : printf("Output Limit Exceeded\n");
                        break;
    }

    printf("time:%d\n", result->time);
    printf("memory:%d\n", result->memory);
    printf("msg:%s\n", result->msg->str);
    printf("debug:%s\n", result->err->str);

    exit(0);
}   
