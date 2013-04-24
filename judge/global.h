/*
 * -*- coding: utf-8 -*-
 * Filename      : global.h
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-10
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <pwd.h>
#include <sys/time.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/resource.h>

#ifndef GLOBAL_H
#define GLOBAL_H

/*
#define G_DISABLE_ASSERT
 */

#define SIGNAL_NUM 65
#define SYSCALL_NUM 400
#define RESOURCE_NUM 20
#define FMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define SPJTIME 5
#define is_nonprint(c) ((c) == '\n' || (c) == '\t' || (c) == ' ')

/* 结果代码，前三个位置不可调换 */
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

/* 信号处理方式 */
typedef enum
{
    SIG_DELIVER,
    SIG_NOTHING,
    SIG_TERMINATE,
}Sigaction;

typedef struct
{
    Status code;
    int time;
    int memory;
    GString * msg; /* 返回用户的信息 */
    GString * err; /* 错误调式信息 */
}Result;

typedef struct
{
    int resource;
    struct rlimit lmt;
}Resource;

typedef struct
{
    const char * key;
    const char * value;
}Environ;

#endif
