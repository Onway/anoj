/*
 * -*- coding: utf-8 -*-
 * Filename      : main.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-11
 */

#include "global.h"
#include "cmdline.h"
#include "keyfile.h"
#include "data.h"
#include "answer.h"

extern int ltime;
extern int memory;
extern int fsize;
extern int preused;

extern int signal_rule[SIGNAL_NUM];
extern gboolean syscall_rule[SYSCALL_NUM];
extern GSList * resource_rule;
extern GSList * environ_rule;
extern GString * input;
extern GString * output;
extern GString * answer;

extern char * cfgfile;
extern char * workdir;
extern char * datadir;
extern char * lang;
extern char * const * command;
extern Result * result;

static void cmdline_test();
static void keyfile_test();
static void data_test();
static void user_child_test();
static void answer_test();
static void do_judge();
static void each_resource(gpointer data, gpointer user_data);
static void each_environ(gpointer data, gpointer user_data);

int
main(int argc, char *argv[])
{
    init_global();

    !parse_cmdline(&argc, &argv) && exit_func();
    /* cmdline_test(); */
    !parse_keyfile() && exit_func();
    /* keyfile_test(); */
    !parse_data() && exit_func();
    /* data_test(); */

    /* user_child_test(); */
    /* answer_test(); */
    do_judge();
    return 0;
}

static void
do_judge()
{
    struct stat sbuf;
    int max_time = 0, max_memory = 0;
    
    while (next_data()) {
        judge_user_program();
        if (result->code != EXIT_AC)
            exit_func();

        stat(answer->str, &sbuf);
        if (sbuf.st_mode & S_IXOTH)
            judge_special_answer();
        else
            judge_common_answer();
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
keyfile_test()
{
    int i;
    Resource * res;

    cmdline_test();
    printf("\n");

    printf("preused = %d\n", preused);
    printf("\n");

    printf("syscall:\n");
    for (i = 0; i < SYSCALL_NUM; ++i)
        printf("%d ", syscall_rule[i]);
    printf("\n");
    printf("\n");

    printf("signal:\n");
    for (i = 0; i < SIGNAL_NUM; ++i)
        printf("%d ", signal_rule[i]);
    printf("\n");
    printf("\n");

    printf("resource:\n");
    g_slist_foreach(resource_rule, each_resource, NULL);
    printf("\n");

    printf("environ:\n");
    g_slist_foreach(environ_rule, each_environ, NULL);
    printf("\n");
}

static void
cmdline_test()
{
    printf("time = %d\n", ltime);
    printf("memory = %d\n", memory);
    printf("fsize = %d\n", fsize);
    lang != NULL && printf("lang = %s\n", lang);
    cfgfile && printf("cfgfile = %s\n", cfgfile);
    workdir && printf("workdir = %s\n", workdir);
    datadir && printf("datadir = %s\n", datadir);
    printf("command = ");
    char * const * ix;
    for (ix = command; *ix != NULL; ++ix)
        printf("%s ", *ix);
    printf("\n");
}

static void
data_test()
{
    while (next_data()) {
        printf("input = %s\n", input->str);
        printf("answer = %s\n", answer->str);
        printf("output = %s\n", output->str);
        printf("\n");
    }
}

static void
user_child_test()
{
    pid_t child;
    int status;
    char * content;

    while (next_data()) {
        child = fork();
        if (child == 0)
            start_user_child();
        
        while (1) {
            wait(&status);
            if (WIFEXITED(status)) {
                printf("child exit with code %d\n", WEXITSTATUS(status));
                printf("child output:\n");
                g_file_get_contents(output->str, &content, NULL, NULL);
                content && printf("%s", content);
            } else if (WIFSIGNALED(status)) {
                printf("child killed by signal: %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                if (WSTOPSIG(status) == SIGTRAP) {
                    ptrace(PTRACE_CONT, child, NULL, NULL);
                    continue;
                }
                printf("child stoped signal: %d, kill it\n", WSTOPSIG(status));
                kill(child, SIGKILL);
            } else
                printf("unknow child exit status\n");
            break;
        }

        unlink(output->str);
    }
}

static void
answer_test()
{
    pid_t child;
    int status;
    char * content;
    struct stat sbuf;

    while (next_data()) {
        child = fork();
        if (child == 0)
            start_user_child();
        
        while (1) {
            wait(&status);
            if (WIFEXITED(status)) {
               stat(answer->str, &sbuf);
               if (sbuf.st_mode & S_IXOTH)
                   judge_special_answer();
               else
                   judge_common_answer();
               if (result->code == EXIT_AC)
                   break;
               else {
                   unlink(output->str);
                   exit_func();
               }
            } else if (WIFSIGNALED(status)) {
                printf("child killed by signal: %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                if (WSTOPSIG(status) == SIGTRAP) {
                    ptrace(PTRACE_CONT, child, NULL, NULL);
                    continue;
                }
                printf("child stoped signal: %d, kill it\n", WSTOPSIG(status));
                kill(child, SIGKILL);
            } else
                printf("unknow child exit status\n");
            break;
        }
        unlink(output->str);
    }
    exit_func();
}

static void
each_resource(gpointer data, gpointer user_data)
{
    Resource * d = (Resource *)data;
    printf("%d = %ld;%ld\n",
            d->resource, d->lmt.rlim_cur, d->lmt.rlim_max);
}

static void
each_environ(gpointer data, gpointer user_data)
{
    Environ * e = (Environ *)data;
    printf("%s = %s\n", e->key, e->value);
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
