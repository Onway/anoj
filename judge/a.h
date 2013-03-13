/*
 * -*- coding: utf-8 -*-
 * Filename      : autocode.h
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-11
 */

#include "global.h"

#ifndef AUTOCODE_H
#define AUTOCODE_H

void auto_signal_rule(const char * key, int value);
void auto_syscall_rule(const char * key, gboolean value);
void auto_resource_rule(const char * key, rlim_t rlim_cur, rlim_t rlim_max);
void auto_environ_rule(const char * key, const char * value);

#endif
