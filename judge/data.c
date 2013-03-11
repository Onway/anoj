/*
 * -*- coding: utf-8 -*-
 * Filename      : data.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-12
 */

#include "data.h"

extern char * workdir;
extern char * datadir;
extern Result * result;

extern GString * input;
extern GString * answer;
extern GString * output;

static GPtrArray * gpar;
static int gpar_index;
static GRand * grand;

static gboolean check_data();
static gint data_compare_func(gconstpointer a, gconstpointer b);
static void set_output();

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

    g_ptr_array_sort(gpar, data_compare_func);   

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
data_compare_func(gconstpointer a, gconstpointer b)
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
