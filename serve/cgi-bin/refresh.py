#!/usr/bin/env python
# Filename      : refresh.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-24

if __name__ == "__main__":
    print "Content-Type: application/json"
    print ""

    f = open("/tmp/result.txt", "r")
    print f.readline()
    f.close()
