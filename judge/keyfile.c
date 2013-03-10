/*
 * -*- coding: utf-8 -*-
 * Filename      : keyfile.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include "keyfile.h"

extern Result * result;
extern GKeyFile * kfile;
static char * first_file = "judge.ini";
static char * second_file = "/etc/wyuoj/judge.ini";

gboolean
parse_keyfile()
{
    gboolean suc;

    kfile = g_key_file_new();
    g_assert(kfile);
    
    suc = g_key_file_load_from_file(kfile, first_file,
            G_KEY_FILE_NONE, &result->gerr);
    if (suc == FALSE) {
        if (result->gerr)
            g_error_free(result->gerr);
        suc = g_key_file_load_from_file(kfile, second_file,
                G_KEY_FILE_NONE, &result->gerr);
    }

    return suc;
}
