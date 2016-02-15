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
import signal
import commands
import traceback
import subprocess

RID = None
PID = None
TIME = None
MEMORY = None
OUTSIZE = None
LANG = None
CODE = None
REMOTE = ""

FROM = ["127.0.0.1", "192.168.1.106"]
TO = "http://127.0.0.1:8888/cgi-bin/result.py"
WORKDIR = "/tmp"
DATADIR = "/home/anojer/data"


class CompileTimeout(Exception):
    pass


def randstr(n = 16):
    st = ""
    for i in range(n + 1):
        st = st + chr(97 + random.randint(0, 25))
    return st


def sighandler(signo, frame):
    raise CompileTimeout


def clean_exit(tmpstr):
    os.chdir(WORKDIR)
    os.system("rm -rf %s*" % tmpstr)
    exit(0)


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
        exit(0)

    f = open(srcfile, "w", 0644)
    f.write(CODE)
    f.close()

    signal.signal(signal.SIGALRM, sighandler)
    signal.alarm(5)

    try:
        popen = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT)
        stdout, stderr = popen.communicate()
    except CompileTimeout:
        send_result(**{"result": "Compile Error",
            "debug": "Compile too long time"})
        popen.terminate()
        clean_exit(tmpstr)
    except Exception:
        send_result(**{"result": "Internal Error",
            "debug": traceback.format_exc()})
        clean_exit(tmpstr)

    signal.alarm(0)
    if popen.returncode != 0:
        send_result(**{"result": "Compile Error", "msg": stdout})
        clean_exit(tmpstr)
    return tmpstr


def do_judge(tmpstr):
    cmd = "judger -t %s -f %s -l %s -d %s -m %s " % (TIME, OUTSIZE, LANG,
            os.path.join(DATADIR, PID), MEMORY)

    if LANG != "java":
        workdir = WORKDIR
    else:
        workdir = os.path.join(WORKDIR, tmpstr)
    cmd += "-w %s " % workdir

    cmd += "-- "

    if LANG == "c" or LANG == "c++":
        cmd += "./%s" % tmpstr
    elif LANG == "python":
        cmd += "python -S %s.py" % tmpstr
    elif LANG == "java":
        cmd += "java -Djava.security.manager -Djava.security.policy=%s Main" % (
                "/etc/anoj/java.policy", )
    
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

    clean_exit(tmpstr)


def save_history():
    submit = os.path.join("/home/anojer/history/submit")
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

    if "REMOTE_ADDR" in os.environ:
        REMOTE = os.environ["REMOTE_ADDR"]

    if REMOTE not in FROM or RID == "" or PID == "" or CODE == "":
        exit(1)

    pid = os.fork()
    if pid != 0:
        exit(0)

    save_history()
    do_judge(do_compile())
