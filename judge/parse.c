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
extern char * cfgfile;
extern char * workdir;
extern char * datadir;
extern char * const * command;
extern GString * input;
extern GString * answer;
extern GString * output;
extern Result * result;
extern char * feedback[];

/* 获取下一组输入，答案，输出文件路径，不存在返回FALSE */
gboolean next_data();
/* 解析数据目录，为next_data准备数据 */
gboolean parse_data();
/* 解析配置文件 */
gboolean parse_keyfile();
/* 解析命令行选项参数 */
gboolean parse_cmdline(int *argc, char ***argv);

static void set_output();       /* 设置输出文件名 */
static gboolean check_data();   /* 检查数据目录的文件名 */

/* 配置文件的组解析 */
static gboolean parse_signal(GKeyFile * kfile);
static gboolean parse_syscall(GKeyFile * kfile);
static gboolean parse_resource(GKeyFile * kfile);
static gboolean parse_environ(GKeyFile * kfile);
static gboolean parse_feedback(GKeyFile * kfile);

/* 排序比较函数 */
static gint cmp_func(gconstpointer a, gconstpointer b);

static GRand * grand;       /* 随机数发生器 */
static GPtrArray * gpar;    /* 指针数组，被next_data使用 */
static int gpar_index;      /* 指针数组的当前位置 */

/* 以下数据被parse_cmdline使用 */
static char * first_file = "judger.ini";
static char * second_file = "/etc/wyuoj/judger.ini";
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
parse_keyfile()
{
    GError * gerr = NULL;
    GKeyFile * kfile = NULL;

    kfile = g_key_file_new();
    g_assert(kfile);
    
    /* 如果命令行参数指定配置文件，任何错误都会退出 */
    if (cfgfile) {
        g_key_file_load_from_file(kfile, cfgfile, G_KEY_FILE_NONE, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        goto endtrue;
    }

    /* 当前目录配置文件，文件不存在之外的错误都会退出 */
    g_key_file_load_from_file(kfile, first_file, G_KEY_FILE_NONE, &gerr);
    if (!gerr) goto endtrue;
    if (gerr && gerr->code != G_KEY_FILE_ERROR_NOT_FOUND) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }

    /* 上一个文件不存在 */
    g_error_free(gerr), gerr = NULL;
    g_key_file_load_from_file(kfile, second_file, G_KEY_FILE_NONE, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }

endtrue:
    return parse_signal(kfile) && parse_syscall(kfile) &&
        parse_resource(kfile) && parse_environ(kfile) && parse_feedback(kfile);
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
    if (!length) return TRUE;   /* 如果没有任何信号 */

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_integer(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        auto_signal_rule(*key, value);
    }

    g_strfreev(keys);
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

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_boolean(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        auto_syscall_rule(*key, value);
    }

    g_strfreev(keys);
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

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_string_list(kfile, group, *key, &length, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        if (length != 2) {
            g_string_printf(result->err, "invalid key-value %s", *key);
            return FALSE;
        }

        !strcmp(value[0], "RLIM_INFINITY") ?
            (lcur = RLIM_INFINITY) : (lcur = atol(value[0]));
        !strcmp(value[1], "RLIM_INFINITY") ?
            (lmax = RLIM_INFINITY) : (lmax = atol(value[1]));

        auto_resource_rule(*key, lcur, lmax);
    }

    g_strfreev(keys);
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

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_string(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }

        auto_environ_rule(*key, value);
    }

    return TRUE;
}

static gboolean
parse_feedback(GKeyFile * kfile)
{
    GError * gerr = NULL;
    gsize length;
    int i;
    char * group = "FEEDBACK";
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

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_string(kfile, group, *key, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }

        auto_feedback(*key, value);
    }

    for (i = 0; i < SIGNAL_NUM; ++i)
        if (feedback[i] == NULL)
            feedback[i] = g_strdup("Invalid Signal");

    g_strfreev(keys);
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
