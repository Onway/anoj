#!/usr/bin/env python
# Filename      : judger.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-23

# This script should not be used for browser,
# it recieved the POST data and sent nothing.
# For practical use, rewrite TO variable and add comment to save_result() call.
import os

import cgi
import cgitb
cgitb.enable()

import json
import urllib
import urllib2
import random
import commands
import ConfigParser

RID = None
PID = None
TIME = None
MEMORY = None
OUTSIZE = None
LANG = None
CODE = None
REMOTE = ""

FROM = None
TO = None
WORKDIR = None
DATADIR = None
INIFILE = ["/home/wyuojer/wyuoj.ini", "/etc/wyuoj/wyuoj.ini"]

def randstr(n = 16):
    st = ""
    for i in range(n + 1):
        st = st + chr(97 + random.randint(0, 25))
    return st

def send_result(**kdict):
    kdict["rid"] = RID
    kdict["pid"] = PID

    if not kdict.has_key("time"):
        kdict["time"] = 0
    if not kdict.has_key("memory"):
        kdict["memory"] = 0
    if not kdict.has_key("codelen"):
        kdict["codelen"] = len(CODE)
    if not kdict.has_key("result"):
        kdict["result"] = "Internal Error"
    if not kdict.has_key("msg"):
        kdict["msg"] = ""
    if not kdict.has_key("debug"):
        kdict["debug"] = ""
    urllib2.urlopen(urllib2.Request(TO, urllib.urlencode(kdict)))

def do_compile():
    cmd = ""
    javadir = ""
    tmpstr = randstr()

    os.chdir(WORKDIR)
    if LANG == "c":
        srcfile = tmpstr + ".c"
        binfile = tmpstr
        cmd = "gcc -o %s %s --static" % (binfile, srcfile)
    elif LANG == "c++":
        srcfile = tmpstr + ".cpp"
        binfile = tmpstr
        cmd = "g++ -o %s %s --static" % (binfile, srcfile)
    elif LANG == "java":
        javadir = tmpstr
        srcfile = "Main.java"
        binfile = "Main"
        os.mkdir(javadir)
        os.chdir(javadir)
        cmd = "javac %s" % srcfile
    elif LANG == "python":
        srcfile = tmpstr + ".py"
        cmd = "pyflakes %s" % srcfile
        #cmd = "ls"
    else:
        send_result(**{"debug": "unsupported language"})
        exit(1)

    f = open(srcfile, "w", 0644)
    f.write(CODE)
    f.close()

    status, output = commands.getstatusoutput(cmd)
    if status != 0:
        send_result(**{"result": "Compile Error", "msg": output})
        os.chdir(WORKDIR)
        os.system("rm -rf %s*" % tmpstr)
        exit(1)
    return tmpstr

def do_judge(tmpstr):
    cmd = "judger -t %s -f %s -l %s -d %s " % (TIME, OUTSIZE, LANG,
            os.path.join(DATADIR, PID))

    if LANG != "java":
        workdir = WORKDIR
    else:
        workdir = os.path.join(WORKDIR, tmpstr)
    cmd += "-w %s " % workdir

    if LANG != "java":
        cmd += "-m %s " % MEMORY
        cmd += "-c %s " % os.path.join("/etc/wyuoj/judger.ini")
    
    cmd += "-- "

    if LANG == "c" or LANG == "c++":
        cmd += "./%s" % tmpstr
    elif LANG == "python":
        cmd += "python -S %s.py" % tmpstr
    elif LANG == "java":
        cmd += "java -Xms%dk -Xmx%dk -Djava.security.policy=%s Main" % (
                int(MEMORY), int(MEMORY) * 2,
                os.path.join("/etc/wyuoj/java.policy"))
    
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

def save_history():
    submit = os.path.join("/home/wyuojer/history/submit")
    if not os.path.exists(submit):
        os.system("mkdir -p %s" % submit)

    kdict = {}
    kdict["rid"] = RID
    kdict["pid"] = PID
    kdict["time"] = TIME
    kdict["memory"] = MEMORY
    kdict["outsize"] = OUTSIZE
    kdict["lang"] = LANG
    kdict["code"] = CODE

    submit = os.path.join(submit, RID)
    f = open(submit, "w");
    f.write(json.dumps(kdict))
    f.close()

if __name__ == "__main__":
    print "Content-Type: text/html"
    print ""

    form = cgi.FieldStorage()
    RID = form.getvalue("rid", "")
    PID = form.getvalue("pid", "")
    TIME = form.getvalue("time", "1000")
    MEMORY = form.getvalue("memory", "32768")
    OUTSIZE = form.getvalue("outsize", "1024")
    LANG = form.getvalue("lang", "c")
    CODE = form.getvalue("code", "")

    cf = ConfigParser.ConfigParser();
    cf.read(INIFILE)
    FROM = cf.get("DEFAULT", "FROM").split(",")
    TO = cf.get("DEFAULT", "TO")
    WORKDIR = cf.get("DEFAULT", "WORKDIR")
    DATADIR = cf.get("DEFAULT", "DATADIR")

    if "REMOTE_ADDR" in os.environ:
        REMOTE = os.environ["REMOTE_ADDR"]

    if REMOTE not in FROM or RID == "" or PID == "" or CODE == "":
        exit(1)

    pid = os.fork()
    if pid != 0:
        exit(0)

    save_history()
    do_judge(do_compile())
