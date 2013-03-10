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
extern GString * lang;
extern GString * workdir;
extern GString * datadir;
extern Result * result;
extern char * const * command;

static char * tmp_lang;
static char * tmp_workdir;
static char * tmp_datadir;

static char * param_str = "[--] <command>";
static char * summary =
"<command> is mandatory non-option argument whice to be judged.\n"
"If <command> with some options, you must use two dash -- before <command>.";
static GOptionEntry entries[] = 
{
    {"time", 't', 0, G_OPTION_ARG_INT, &ltime,
        "Time limit in milliseconds", "N"},
    {"memory", 'm', 0, G_OPTION_ARG_INT, &memory,
        "Memory limit in kbytes", "N"},
    {"fsize", 'f', 0, G_OPTION_ARG_INT, &fsize,
        "Answer output limit in kbytes", "N"},
    {"lang", 'l', 0, G_OPTION_ARG_STRING, &tmp_lang,
        "User program language.",
        "Language"},
    {"workdir", 'w', 0, G_OPTION_ARG_STRING, &tmp_workdir,
        "User program working directory",
        "Directory"},
    {"datadir", 'd', 0, G_OPTION_ARG_STRING, &tmp_datadir,
        "Directory holds the input and output data files",
        "Directory"},
    {NULL}
};

static void test();

gboolean
parse_cmdline(int *argc, char ***argv)
{
    GOptionContext * option = NULL;

    option = g_option_context_new(param_str);
    g_option_context_add_main_entries(option, entries, NULL);
    g_option_context_set_summary(option, summary);
    if (!g_option_context_parse(option, argc, argv, &result->gerr))
        return FALSE;

    if (tmp_lang) {
        g_string_assign(lang, tmp_lang);
        g_free(tmp_lang);
    }
    if (tmp_workdir) {
        g_string_assign(workdir, tmp_workdir);
        g_free(tmp_workdir);
    }
    if (tmp_datadir) {
        g_string_assign(datadir, tmp_datadir);
        g_free(tmp_datadir);
    }

    if ((*argc) == 1 || ((*argc) == 2 && strcmp((*argv)[1], "--") == 0)) {
        result->gerr = g_error_new(1, 0, "<command> not found");
        return FALSE;
    } else if ((*argc) > 2 && strcmp((*argv)[1], "--") == 0)
        command = &(*argv)[2];
    else
        command = &(*argv)[1];

    test();
    return TRUE;
}

static void
test()
{
    printf("time = %d\n", ltime);
    printf("memory = %d\n", memory);
    printf("fsize = %d\n", fsize);
    printf("lang = %s\n", lang->str);
    printf("workdir = %s\n", workdir->str);
    printf("datadir = %s\n", datadir->str);
    printf("command = ");

    char * const * ix;
    for (ix = command; *ix != NULL; ++ix)
        printf("%s ", *ix);
    printf("\n");
}
