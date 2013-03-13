/*
 * -*- coding: utf-8 -*-
 * Filename      : main.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-11
 */

#include "global.h"
#include "parse.h"
#include "command.h"
#include "judge.h"

int ltime;
int memory;
int fsize;
int preused;

int signal_rule[SIGNAL_NUM];
gboolean syscall_rule[SYSCALL_NUM];
GSList * resource_rule;
GSList * environ_rule;

GString * input;
GString * output;
GString * answer;

char * cfgfile;
char * workdir;
char * datadir;
char * lang;
char * const * command;
Result * result;

static void cmdline_test();
static void keyfile_test();
static void data_test();
static void user_child_test();
static void answer_test();
static void do_judge();
static void each_resource(gpointer data, gpointer user_data);
static void each_environ(gpointer data, gpointer user_data);
static void init_varible();
static void exit_func();

int
main(int argc, char *argv[])
{
    struct stat sbuf;
    int max_time, max_memory;

    init_varible();
    max_time = max_memory = 0;

    /*
    !parse_cmdline(&argc, &argv) && exit_func();
    !parse_keyfile() && exit_func();
    !parse_data() && exit_func();
    */

    if (!parse_cmdline(&argc, &argv) || !parse_keyfile() || !parse_data())
        exit_func();

    while (next_data()) {
        execute_command();
        if (result->code != EXIT_AC)
            exit_func();

        stat(answer->str, &sbuf);
        sbuf.st_mode & S_IXOTH ? special_judge() : normal_judge();

        if (result->code != EXIT_AC)
            exit_func();

        max_time = MAX(max_time, result->time);
        max_memory = MAX(max_memory, result->memory);
    }

    result->time = max_time;
    result->memory = max_memory;
    exit_func();

    return 0;
}

static void
do_judge()
{
    struct stat sbuf;
    int max_time = 0, max_memory = 0;
    
    while (next_data()) {
        execute_command();
        if (result->code != EXIT_AC)
            exit_func();

        stat(answer->str, &sbuf);
        if (sbuf.st_mode & S_IXOTH)
            special_judge();
        else
            normal_judge();
        if (result->code != EXIT_AC)
            exit_func();

        max_time = MAX(max_time, result->time);
        max_memory = MAX(max_memory, result->memory);
    }

    result->time = max_time;
    result->memory = max_memory;
    exit_func();
}

static void
init_varible()
{
    input = g_string_new("");
    output = g_string_new("");
    answer = g_string_new("");
    g_assert(input && output && answer);

    result = (Result *) malloc(sizeof(Result));
    g_assert(result);
    result->time = result->memory = 0;
    result->code = EXIT_IE;
    result->msg = g_string_new("");
    result->err = g_string_new("");
    g_assert(result->msg && result->err);

    memset(signal_rule, 0, sizeof(int) * SIGNAL_NUM);
    memset(syscall_rule, 0, sizeof(gboolean) * SYSCALL_NUM);
}

static void
exit_func()
{
    printf("Result: ");
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

    printf("Time: %d\n", result->time);
    printf("Memory: %d\n", result->memory);
    printf("Message: %s\n", result->msg->str);
    printf("Error: %s\n", result->err->str);

    exit(0);
}   
