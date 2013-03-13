/*
 * -*- coding: utf-8 -*-
 * Filename      : judge.h
 * Author        : Liu Yi <aluohuai@126.com>
 * Create Date   : 2013-03-13
 */

#include "global.h"

#ifndef JUDGE_H
#define JUDGE_H

#define is_nonprint(c) ((c) == '\n' || (c) == '\t' || (c) == ' ')

void special_judge();
void normal_judge();

#endif
