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

char * cfgfile;
char * workdir;
char * datadir;
char * lang;
char * const * command;

GKeyFile * kfile;
Result * result;

void
init_global()
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
}
