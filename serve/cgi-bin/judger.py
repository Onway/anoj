#!/usr/bin/env python
# Filename      : judger.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-23

import os

import cgi
import cgitb
cgitb.enable()

import urllib
import urllib2
import random
import commands

RID = None
PID = None
TIME = None
MEMORY = None
OUTSIZE = None
LANG = None
CODE = None
REMOTE = ""

ALLOW =  ["127.0.0.1", ]
URL = "http://127.0.0.1:8888/cgi-bin/result.py"
WORKDIR = os.path.join(os.environ["HOME"], ".wyuoj/tmp")
DATADIR = os.path.join(os.environ["HOME"], ".wyuoj/data")

def randstr(n = 16):
    st = ""
    for i in range(n + 1):
        st = st + chr(97 + random.randint(0, 25))
    return st

def send_result(**kdict):
    kdict["rid"] = RID

    if not kdict.has_key("time"):
        kdict["time"] = 0
    if not kdict.has_key("m"):
        kdict["momory"] = 0
    if not kdict.has_key("codelen"):
        kdict["codelen"] = len(CODE)
    if not kdict.has_key("result"):
        kdict["result"] = "result"
    if not kdict.has_key("msg"):
        kdict["msg"] = "msg"
    if not kdict.has_key("debug"):
        kdict["debug"] = "debug"
    urllib2.urlopen(urllib2.Request(URL, urllib.urlencode(kdict)))

def do_compile():
    cmd = ""
    javadir = ""
    tmpstr = randstr()

    os.chdir(WORKDIR)
    if LANG == "c":
        srcfile = tmpstr + ".c"
        binfile = tmpstr
        cmd = "gcc -o %s %s --static" % (srcfile, binfile)
    elif LANG == "c++":
        srcfile = tmpstr + ".cpp"
        binfile = tmpstr
        cmd = "g++ -o %s %s --static" % (srcfile, binfile)
    elif LANG == "java":
        javadir = tmpstr
        srcfile = "Main.java"
        binfile = "Main"
        os.mkdir(javadir)
        cmd = "javac %s" % srcfile
    elif LANG == "python":
        srcfile = tmpstr + ".py"
        os.chmod(srcfile, 0755)
        cmd = "pyflakes %s" % srcfile
    else:
        send_result(**{"debug": "unsupported language"})
        exit(1)

    status, output = commands.getstatusoutput(cmd)
    if status != 0:
        send_result(**{"result": "Compile Error", "msg": output})
        os.system("rm -rf %s*" % tmpstr)
        exit(1)
    return tmpstr

def do_judge(tmpstr):
    cmd = "judger "
    workdir = ""

    if TIME != 0: cmd += "-t %s " % TIME
    if MEMORY != 0: cmd += "-m %s " % MEMORY
    if OUTSIZE != 0: cmd += "-f %s " % OUTSIZE
    if LANG != "": cmd += "-l %s " % LANG
    
    cmd += "-w %s " % WORKDIR
    cmd += "-d %s " % os.path.join(DATADIR, PID)
    cmd += "-c %s " % os.path.join(os.environ["HOME"], ".wyuoj/judger.ini")
    cmd += "-- "

    workdir = WORKDIR 
    if LANG == "c" or LANG == "c++":
        cmd += "./%s" % tmpstr
    elif LANG == "python":
        cmd += "./%s.py" % tmpstr
    elif LANG == "java":
        workdir = os.path.join(workdir, tmpstr)
        cmd += "java -cp %s Main" % os.path.join(workdir)
    
    os.chdir(workdir)
    status, output = commands.getstatusoutput(cmd)
    if status == 0:
        kdict = {}
        for i in output.split("\n"):
            if i != "":
                t = i.split(":")
                kdict[t[0]] = t[1]
        send_result(**kdict)
    else:
        send_result(**{"result": "Internal Error", "debug": "%s error" % cmd})

    os.chdir(WORKDIR)
    os.system("rm -rf %s*" % tmpstr)

if __name__ == "__main__":
    form = cgi.FieldStorage()
    RID = form.getvalue("rid", "")
    PID = form.getvalue("pid", "")
    TIME = form.getvalue("time", "")
    MEMORY = form.getvalue("memory", "")
    OUTSIZE = form.getvalue("outsize", "")
    LANG = form.getvalue("lang", "")
    CODE = form.getvalue("code", "")

    if "REMOTE_ADDR" in os.environ:
        REMOTE = os.environ["REMOTE_ADDR"]

    if REMOTE not in ALLOW or RID == "" or PID == "" or CODE == "":
        exit(1)

    pid = os.fork()
    if pid != 0:
        exit(0)

    do_judge(do_compile())
