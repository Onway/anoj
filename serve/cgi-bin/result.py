#!/usr/bin/env python
# Filename      : result.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-23

import cgi
import cgitb
cgitb.enable()

print "Content-Type: text/html"
print ""

form = cgi.FieldStorage()
print form

f = open("/tmp/result.txt", "w")
f.write(str(form) + "\n")
f.close()
