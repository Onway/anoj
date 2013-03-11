/*
 * -*- coding: utf-8 -*-
 * Filename      : keyfile.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include "keyfile.h"

extern int ltime;
extern int memory;
extern int fsize;
extern int preused;
extern char * lang;
extern char * cfgfile;
extern char * workdir;
extern char * datadir;
extern Result * result;

static char * first_file = "judge.ini";
static char * second_file = "/etc/wyuoj/judge.ini";

static gboolean parse_option(GKeyFile * kfile);
static gboolean parse_preused(GKeyFile * kfile);
static gboolean parse_signal(GKeyFile * kfile);
static gboolean parse_syscall(GKeyFile * kfile);
static gboolean parse_resource(GKeyFile * kfile);
static gboolean parse_environ(GKeyFile * kfile);

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
        suc = g_key_file_load_from_file(kfile, first_file,
                G_KEY_FILE_NONE, &gerr);
    }
    if (!suc) {
        gerr ? g_error_free(gerr) : 1;
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

    preused = g_key_file_get_integer(kfile, "PARSE", lang, &gerr);
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
    gsize * length = NULL;
    char * group = "SIGNAL";
    char ** keys = NULL;
    char ** key;
    int value;

    keys = g_key_file_get_keys(kfile, group, length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_free(length);
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
    gsize * length = NULL;
    char * group = "SYSCALL";
    char ** keys = NULL;
    char ** key;
    gboolean value;

    keys = g_key_file_get_keys(kfile, group, length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_free(length);
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
    gsize * length = NULL;
    char * group = "RESOURCE";
    char ** keys = NULL;
    char ** key;
    char ** value;
    int i;
    rlim_t lcur, lmax;

    keys = g_key_file_get_keys(kfile, group, length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_free(length);
    g_assert(keys);

    for (key = keys; *key != NULL; ++key) {
        value = g_key_file_get_string_list(kfile, group, *key, length, &gerr);
        if (gerr) {
            g_string_assign(result->err, gerr->message);
            g_error_free(gerr);
            return FALSE;
        }
        if (*length != 2) {
            g_string_printf(result->err, "Invalid key-value %s", *key);
            return FALSE;
        }

        !strcmp(value[0], "RLIM_INFINITY") ?
            (lcur = RLIM_INFINITY) : (lcur = atoi(value[0]));
        !strcmp(value[1], "RLIM_INFINITY") ?
            (lmax = RLIM_INFINITY) : (lmax = atoi(value[1]));

        auto_resource_rule(*key, lcur, lmax);
    }

    return TRUE;
}

static gboolean
parse_environ(GKeyFile * kfile)
{
    GError * gerr = NULL;
    gsize * length = NULL;
    char * group = "ENVIRON";
    char ** keys = NULL;
    char ** key;
    char * value;

    keys = g_key_file_get_keys(kfile, group, length, &gerr);
    if (gerr) {
        g_string_assign(result->err, gerr->message);
        g_error_free(gerr);
        return FALSE;
    }
    if (!length) return TRUE;
    g_free(length);
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
