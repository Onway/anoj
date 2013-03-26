#!/usr/bin/env python
# Filename      : refresh.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-24

import os
import cgi
import commands

if __name__ == "__main__":
    print "Content-Type: application/json"
    print ""

    form = cgi.FieldStorage()
    rid = form.getvalue("rid", "")

    # get the last runid
    if rid == "":
        os.chdir(os.path.join(os.environ["HOME"], ".wyuoj/history/result"))
        print commands.getoutput("ls -1 | wc -l")

    # get result by runid
    else:
        result = os.path.join(os.environ["HOME"], ".wyuoj/history/result")
        result = os.path.join(result, rid)
        f = open(result, "r")
        print f.readline()
        f.close()
