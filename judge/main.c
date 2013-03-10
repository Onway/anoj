/*
 * -*- coding: utf-8 -*-
 * Filename      : main.c
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-11
 */

#include "global.h"
#include "cmdline.h"

int
main(int argc, char *argv[])
{
    init_global();
    if (!parse_cmdline(&argc, &argv))
        exit_func();
    return 0;
}
