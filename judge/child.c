/*
 * -*- coding: utf-8 -*-
 * Filename      : child.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-12
 */

#include "child.h"

extern int ltime;
extern char * workdir;
extern char * const * command;
extern GString * input;
extern GString * output;
extern GSList * resource_rule;
extern GSList * environ_rule;

static void set_io();
static void reset_resource();
static void foreach_resource(gpointer data, gpointer user_data);
static void foreach_environ(gpointer data, gpointer user_data);

void
start_child()
{
    char ** env;

    env = g_get_environ();

    chdir(workdir);
    set_io();
    g_slist_foreach(resource_rule, foreach_resource, NULL);
    g_slist_foreach(environ_rule, foreach_environ, &env);
    reset_resource();

    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL))
        exit(1);

    execvpe(command[0], command, (char * const *)env);
    exit(2);
}

static void
set_io()
{
    int infd, outfd;

    infd = open(input->str, O_RDONLY);
    outfd = open(output->str, O_WRONLY | O_CREAT, FILE_NODE);

    dup2(infd, STDIN_FILENO);
    dup2(outfd, STDOUT_FILENO);
    
    close(infd);
    close(outfd);
    close(STDERR_FILENO);
}

static void 
reset_resource()
{
    struct rlimit t;

    if (ltime % 1000 == 0)
        t.rlim_cur = ltime / 1000 + 1;
    else
        t.rlim_cur = ltime / 1000 + 2;
    t.rlim_max = t.rlim_cur + 1;
    setrlimit(RLIMIT_CPU, &t);
}

static void
foreach_resource(gpointer data, gpointer user_data)
{
    Resource * ptr;
    
    ptr = (Resource *)data;
    setrlimit(ptr->resource, &ptr->lmt);
}

static void
foreach_environ(gpointer data, gpointer user_data)
{
    Environ * ptr;
    char *** env;

    ptr = (Environ *)data;
    env = (char ***)user_data;

    if (!strcmp(ptr->value, ""))
        *env = g_environ_unsetenv(*env, ptr->key);
    else
        *env = g_environ_setenv(*env, ptr->key, ptr->value, TRUE);
}
