/*
 * -*- coding: utf-8 -*-
 * Filename      : cmdline.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include "cmdline.h"

extern int ltime;
extern int memory;
extern int fsize;
extern char * lang;
extern char * cfgfile;
extern char * workdir;
extern char * datadir;
extern char * const * command;
extern Result * result;

static char * param_str = "[--] <command>";
static char * summary =
"  <command> is mandatory non-option argument whice to be judged.\n"
"  If <command> with some options, you must use two dash -- before <command>.";
static GOptionEntry entries[] = 
{
    {"time", 't', 0, G_OPTION_ARG_INT, &ltime,
        "Time limit in milliseconds", "N"},
    {"memory", 'm', 0, G_OPTION_ARG_INT, &memory,
        "Memory limit in kbytes", "N"},
    {"fsize", 'f', 0, G_OPTION_ARG_INT, &fsize,
        "Answer output limit in kbytes", "N"},
    {"lang", 'l', 0, G_OPTION_ARG_STRING, &lang,
        "User program language.",
        "Language"},
    {"conf", 'c', 0, G_OPTION_ARG_STRING, &cfgfile,
        "Specify a configuration file",
        "Filepath"},
    {"workdir", 'w', 0, G_OPTION_ARG_STRING, &workdir,
        "User program working directory",
        "Directory"},
    {"datadir", 'd', 0, G_OPTION_ARG_STRING, &datadir,
        "Directory holds the input and output data files",
        "Directory"},
    {NULL}
};

gboolean
parse_cmdline(int *argc, char ***argv)
{
    GOptionContext * option = NULL;
    GError * gerr = NULL;

    option = g_option_context_new(param_str);
    g_option_context_add_main_entries(option, entries, NULL);
    g_option_context_set_summary(option, summary);
    if (!g_option_context_parse(option, argc, argv, &gerr)) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }

    if ((*argc) == 1 || ((*argc) == 2 && strcmp((*argv)[1], "--") == 0)) {
        g_string_assign(result->err, "<command> not found");
        return FALSE;
    } else if ((*argc) > 2 && strcmp((*argv)[1], "--") == 0)
        command = &(*argv)[2];
    else
        command = &(*argv)[1];

    return TRUE;
}
