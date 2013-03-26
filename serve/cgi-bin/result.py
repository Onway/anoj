#!/usr/bin/env python
# Filename      : result.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-23

import os
import json
import cgi
import cgitb
cgitb.enable()


if __name__ == "__main__":
    form = cgi.FieldStorage()
    
    # get history submit
    action = form.getvalue("action", "")
    if action == "q":
        print "Content-Type: application/json"
        print ""
        rid = form.getvalue("rid", "")
        result = os.path.join(os.environ["HOME"], ".wyuoj/history/submit")
        result = os.path.join(result, rid)
        f = open(result, "r")
        print f.readline()
        f.close()
        exit(0)
        
    # write the judger result
    # this page not use for browser, but for judger.py script
    print "Content-Type: text/html"
    print ""
    kdict = {}
    kdict["rid"] = form.getvalue("rid", "")
    kdict["pid"] = form.getvalue("pid", "")
    kdict["time"] = form.getvalue("time", "")
    kdict["memory"] = form.getvalue("memory", "")
    kdict["codelen"] = form.getvalue("codelen", "")
    kdict["result"] = form.getvalue("result", "")
    kdict["msg"] = form.getvalue("msg", "")
    kdict["debug"] = form.getvalue("debug", "")

    result = os.path.join(os.environ["HOME"], ".wyuoj/history/result")
    result = os.path.join(result, kdict["rid"])
    f = open(result, "w")
    f.write(json.dumps(kdict))
    f.close()
