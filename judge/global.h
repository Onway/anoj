/*
 * -*- coding: utf-8 -*-
 * Filename      : global.h
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifndef GLOBAL_H
#define GLOBAL_H

/*
#define G_DISABLE_ASSERT
 */

#define SIGNAL_NUM 70
#define SYSCALL_NUM 400
#define RESOURCE_NUM 20

typedef enum
{
    EXIT_AC,
    EXIT_PE,
    EXIT_WA,
    EXIT_RE,
    EXIT_TLE,
    EXIT_MLE,
    EXIT_OLE,
    EXIT_IE,
}Status;

typedef struct
{
    Status code;
    int time;
    int memory;
    GString * msg;
    GError * gerr;
}Result;

void init_global();

#endif