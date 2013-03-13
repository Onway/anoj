/*
 * -*- coding: utf-8 -*-
 * Filename      : parse.h
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "global.h"

#ifndef PARSE_H
#define PARSE_H

gboolean parse_cmdline(int *argc, char ***argv);
gboolean parse_keyfile();
gboolean parse_data();
gboolean next_data();

#endif
