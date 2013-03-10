/*
 * -*- coding: utf-8 -*-
 * Filename      : global.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include "global.h"

int ltime;
int memory;
int fsize;
int preused;

int signal_rule[SIGNAL_NUM];
gboolean syscall_rule[SYSCALL_NUM];
struct rlimit resource_rule[RESOURCE_NUM];

GString * input;
GString * output;
GString * answer;
GString * workdir;
GString * datadir;
GString * lang;
char * const * command;

GKeyFile * kfile;

Result * result;

void
init_global()
{
    input = g_string_new("");
    output = g_string_new("");
    answer = g_string_new("");
    workdir = g_string_new("");
    datadir = g_string_new("");
    lang = g_string_new("");
    g_assert(input && output && answer && workdir && datadir && lang);

    result = (Result *) malloc(sizeof(Result));
    memset(result, 0, sizeof(Result));
    g_assert(result);

    result->code = EXIT_IE;
    result->msg = g_string_new("");
    result->gerr = NULL;
    g_assert(result->msg);
}
