/*
 * -*- coding: utf-8 -*-
 * Filename      : parse.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "global.h"

extern int ltime;
extern int memory;
extern int fsize;
extern char * lang;
extern char * workdir;
extern char * datadir;
extern char * const * command;
extern GString * input;
extern GString * answer;
extern GString * output;
extern Result * result;

/* 获取下一组输入，答案，输出文件路径，不存在返回FALSE */
gboolean next_data();
/* 解析数据目录，为next_data准备数据 */
gboolean parse_data();
/* 解析命令行选项参数 */
gboolean parse_cmdline(int *argc, char ***argv);

static void set_output();       /* 设置输出文件名 */
static gboolean check_data();   /* 检查数据目录的文件名 */

/* 排序比较函数 */
static gint cmp_func(gconstpointer a, gconstpointer b);

static GRand * grand;       /* 随机数发生器 */
static GPtrArray * gpar;    /* 指针数组，被next_data使用 */
static int gpar_index;      /* 指针数组的当前位置 */

/* 以下数据被parse_cmdline使用 */
static char * parmstr = "[--] <command>";
static char * summary =
"  <command> is mandatory non-option argument whice to be judged.\n"
"  If <command> with some options, you must use two dash -- before <command>.";
static GOptionEntry entries[] = 
{
    {"time", 't', 0, G_OPTION_ARG_INT, &ltime,
        "Time limit in milliseconds", "N"},
    {"memory", 'm', 0, G_OPTION_ARG_INT, &memory,
        "Memory limit in kbytes.No effect for java", "N"},
    {"fsize", 'f', 0, G_OPTION_ARG_INT, &fsize,
        "Answer output limit in kbytes", "N"},
    {"lang", 'l', 0, G_OPTION_ARG_STRING, &lang,
        "User program language",
        "Language"},
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

    /* 解析完选项后，剩下一个参数或者两个的错误情况 */
    if ((*argc) == 1 || ((*argc) == 2 && strcmp((*argv)[1], "--") == 0)) {
        g_string_assign(result->err, "<command> not found");
        return FALSE;
    } else if ((*argc) > 2 && strcmp((*argv)[1], "--") == 0)
        command = &(*argv)[2];
    else
        command = &(*argv)[1];

    !ltime && (ltime = 1000);
    !memory && (memory = 32768);
    !fsize && (fsize = 1024); 
    !lang && (lang = g_strdup("c"));
    !workdir && (workdir = g_strdup("/tmp"));
    !datadir && (datadir = g_strdup("/tmp/data"));
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
    g_assert(grand && gpar);

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

    g_string_assign(result->err, "parse data directory error");
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
