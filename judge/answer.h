/*
 * -*- coding: utf-8 -*-
 * Filename      : answer.h
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "global.h"

#ifndef ANSWER_H
#define ANSWER_H

#define is_nonprint(c) ((c) == '\n' || (c) == '\t' || (c) == ' ')

void judge_common_answer();
void judge_special_answer();

#endif
