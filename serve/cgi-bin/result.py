#!/usr/bin/env python
# Filename      : result.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-23

import json
import cgi
import cgitb
cgitb.enable()


if __name__ == "__main__":
    print "Content-Type: text/html"
    print ""

    form = cgi.FieldStorage()
    kdict = {}
    kdict["rid"] = form.getvalue("rid", "")
    kdict["pid"] = form.getvalue("pid", "")
    kdict["time"] = form.getvalue("time", "")
    kdict["momory"] = form.getvalue("momory", "")
    kdict["codelen"] = form.getvalue("codelen", "")
    kdict["result"] = form.getvalue("result", "")
    kdict["msg"] = form.getvalue("msg", "")
    kdict["debug"] = form.getvalue("debug", "")

    f = open("/tmp/result.txt", "w")
    f.write(json.dumps(kdict))
    f.close()
