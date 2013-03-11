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
    data_test();

    return 0;
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
