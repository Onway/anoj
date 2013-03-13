/*
 * -*- coding: utf-8 -*-
 * Filename      : parse.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "parse.h"

extern int ltime;
extern int memory;
extern int fsize;
extern int preused;
extern char * lang;
extern char * cfgfile;
extern char * workdir;
extern char * datadir;
extern char * const * command;
extern GString * input;
extern GString * answer;
extern GString * output;
extern Result * result;

static gboolean parse_option(GKeyFile * kfile);
static gboolean parse_preused(GKeyFile * kfile);
static gboolean parse_signal(GKeyFile * kfile);
static gboolean parse_syscall(GKeyFile * kfile);
static gboolean parse_resource(GKeyFile * kfile);
static gboolean parse_environ(GKeyFile * kfile);
static gboolean check_data();
static gint cmp_func(gconstpointer a, gconstpointer b);
static void set_output();

static char * first_file = "judge.ini";
static char * second_file = "/etc/wyuoj/judge.ini";
static GPtrArray * gpar;
static int gpar_index;
static GRand * grand;
static char * parmstr = "[--] <command>";
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

    option = g_option_context_new(parmstr);
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

gboolean
parse_keyfile()
{
    gboolean suc = FALSE;
    GError * gerr = NULL;
    GKeyFile * kfile = NULL;

    kfile = g_key_file_new();
    g_assert(kfile);
    
    if (cfgfile)
        suc = g_key_file_load_from_file(kfile, cfgfile,
                G_KEY_FILE_NONE, &gerr);
    if (!suc) {
        gerr ? g_error_free(gerr) : 1;
        gerr = NULL;
        suc = g_key_file_load_from_file(kfile, first_file,
                G_KEY_FILE_NONE, &gerr);
    }
    if (!suc) {
        gerr ? g_error_free(gerr) : 1;
        gerr = NULL;
        suc = g_key_file_load_from_file(kfile, second_file,
                G_KEY_FILE_NONE, &gerr);
    }
    if (!suc) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return suc;
    }

    return parse_option(kfile) && parse_preused(kfile) &&
        parse_signal(kfile) && parse_syscall(kfile) &&
        parse_resource(kfile) && parse_environ(kfile);
}

static gboolean
parse_option(GKeyFile * kfile)
{
    GError * gerr = NULL;
    
    !ltime && (
            ltime = g_key_file_get_integer(kfile, "OPTION", "TIME", &gerr));
    if (gerr) goto ERROR;

    !memory && (
            memory = g_key_file_get_integer(kfile, "OPTION", "MEMORY", &gerr));
    if (gerr) goto ERROR;

    !fsize && (
            fsize = g_key_file_get_integer(kfile, "OPTION", "FSIZE", &gerr)); 
    if (gerr) goto ERROR;

    !lang && (
            lang = g_key_file_get_string(kfile, "OPTION", "LANG", &gerr));
    if (gerr) goto ERROR;

    !workdir && (
            workdir = g_key_file_get_string(kfile, "OPTION", "WORKDIR", &gerr));
    if (gerr) goto ERROR;

    !datadir && (
            datadir = g_key_file_get_string(kfile, "OPTION", "DATADIR", &gerr));
    if (gerr) goto ERROR;

    return TRUE;

ERROR:
    g_string_assign(result->err, gerr->message);
    g_error_free(gerr);
    return FALSE;
}

static gboolean
parse_preused(GKeyFile * kfile)
{
    GError * gerr = NULL;

    preused = g_key_file_get_integer(kfile, "PARSER", lang, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
    }

    return !gerr;
}

static gboolean
parse_signal(GKeyFile * kfile)
{
    GError * gerr = NULL;
    gsize  length;
    char * group = "SIGNAL";
    char ** keys = NULL;
    char ** key;
    int value;

    keys = g_key_file_get_keys(kfile, group, &length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_assert(keys);

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_integer(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        auto_signal_rule(*key, value);
    }

    return TRUE;
}

static gboolean
parse_syscall(GKeyFile * kfile)
{
    GError * gerr = NULL;
    gsize length;
    char * group = "SYSCALL";
    char ** keys = NULL;
    char ** key;
    gboolean value;

    keys = g_key_file_get_keys(kfile, group, &length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_assert(keys);

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_boolean(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        auto_syscall_rule(*key, value);
    }

    return TRUE;
}

static gboolean
parse_resource(GKeyFile * kfile)
{
    GError * gerr = NULL;
    gsize length;
    char * group = "RESOURCE";
    char ** keys = NULL;
    char ** key;
    char ** value;
    int i;
    rlim_t lcur, lmax;

    keys = g_key_file_get_keys(kfile, group, &length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_assert(keys);

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_string_list(kfile, group, *key, &length, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        if (length != 2) {
            g_string_printf(result->err, "Invalid key-value %s", *key);
            return FALSE;
        }

        !strcmp(value[0], "RLIM_INFINITY") ?
            (lcur = RLIM_INFINITY) : (lcur = atol(value[0]));
        !strcmp(value[1], "RLIM_INFINITY") ?
            (lmax = RLIM_INFINITY) : (lmax = atol(value[1]));

        auto_resource_rule(*key, lcur, lmax);
    }

    return TRUE;
}

static gboolean
parse_environ(GKeyFile * kfile)
{
    GError * gerr = NULL;
    gsize length;
    char * group = "ENVIRON";
    char ** keys = NULL;
    char ** key;
    char * value;

    keys = g_key_file_get_keys(kfile, group, &length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_assert(keys);

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_string(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            return FALSE;
        }

        auto_environ_rule(*key, value);
    }

    return TRUE;
}

gboolean
parse_data()
{
    GDir * gdir = NULL;
    GError * gerr = NULL;
    const char * name = NULL;
    
    grand = g_rand_new();
    gpar = g_ptr_array_new();

    gdir = g_dir_open(datadir, 0, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }

    while ((name = g_dir_read_name(gdir)) != NULL)
        g_ptr_array_add(gpar, (char *)name);

    if (gpar->len && (gpar->len % 2) == 0 && check_data())
        return TRUE;

    g_string_assign(result->err, "Parse data directory error");
    return FALSE;
}

gboolean
next_data()
{
    char * path;

    if (gpar_index >= gpar->len) return FALSE;

    path = g_build_path("/", datadir,
            g_ptr_array_index(gpar, gpar_index++), NULL);
    g_string_assign(answer, path);
    g_free(path);
    
    path = g_build_path("/", datadir,
            g_ptr_array_index(gpar, gpar_index++), NULL);
    g_string_assign(input, path);
    g_free(path);

    set_output();
    
    return TRUE;
}

static gboolean
check_data()
{
    int i;
    char ** first = NULL;
    char ** second = NULL;;
    gboolean ret = TRUE;

    g_ptr_array_sort(gpar, cmp_func);   

    for (i = 0; i < gpar->len; i += 2) {
        first = g_strsplit(g_ptr_array_index(gpar, i), ".", 0);
        second = g_strsplit(g_ptr_array_index(gpar, i + 1), ".", 0);

        if (g_strcmp0(first[0], second[0]) || g_strcmp0(first[1], "ans") ||
                g_strcmp0(second[1], "in"))
            ret = FALSE;

        g_strfreev(first);
        g_strfreev(second);
        if (!ret) return FALSE;
    }

    return ret;
}

static gint
cmp_func(gconstpointer a, gconstpointer b)
{
    return strcmp(*(char **)a, *(char **)b);
}

static void
set_output()
{
    static char * clist = 
"ABCDEFGHIGKLMNOPQRSTUVWXYZ"
"abcdefghigklmnopqrstuvwxyz"
"1234567890";

    char * path;
    int i, cnt = 16;
    int length = strlen(clist);
    GString * rnd = g_string_new("");

    while (cnt--)
        g_string_append_c(rnd, clist[g_rand_int(grand) % length]);
    g_string_append(rnd, ".out");

    path = g_build_path("/", workdir, rnd->str, NULL);
    g_string_assign(output, path);

    g_string_free(rnd, TRUE);
    g_free(path);
}
