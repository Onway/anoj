#!/usr/bin/env python
# Filename      : wyuojsrv.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-22

import os
import sys
import signal
import logging
import SocketServer
import BaseHTTPServer
import CGIHTTPServer

HOST = "127.0.0.1"
#HOST = "192.168.1.102"
PORT = 8888
SERVER = None
LOGGER = None
WORKDIR = os.path.join(os.environ["HOME"], ".wyuoj")
PIDFILE = os.path.join(os.environ["HOME"], ".wyuoj/wyuojsrv.pid")
LOGFILE = os.path.join(os.environ["HOME"], ".wyuoj/wyuojsrv.log")

class ThreadingServer(SocketServer.ThreadingMixIn, BaseHTTPServer.HTTPServer):
    pass

class CGIHandler(CGIHTTPServer.CGIHTTPRequestHandler):
    def log_message(self, format, *args):
        LOGGER.info("%s: %s" % (self.address_string(), format%args))
        
def _init_logger():
    global LOGGER

    LOGGER = logging.getLogger()
    handler = logging.FileHandler(LOGFILE)
    formatter = logging.Formatter("%(asctime)s %(levelname)s %(message)s")
    handler.setFormatter(formatter)
    LOGGER.addHandler(handler)
    LOGGER.setLevel(logging.NOTSET)

def _switch_daemon():
    pid = os.fork()
    if pid != 0:
        exit(0)
    os.setsid()
    os.umask(0)
    #os.chdir(WORKDIR)

    pid = os.fork()
    if pid != 0:
        exit(0)

    sys.stdout.flush()
    sys.stderr.flush()
    si = open("/dev/null", "r")
    so = open("/dev/null", "a+")
    se = open("/dev/null", "a+")
    os.dup2(si.fileno(), sys.stdin.fileno())
    os.dup2(so.fileno(), sys.stdout.fileno())
    os.dup2(se.fileno(), sys.stderr.fileno())

def do_start():
    global SERVER

    if os.path.exists(PIDFILE):
        print "Already in running"
        return
    if not os.path.exists(WORKDIR):
        os.mkdir(WORKDIR, 0755)

    _switch_daemon()
    _init_logger()

    pfile = open(PIDFILE, "w", 0644)
    pfile.write("%s" % os.getpid())
    pfile.close()

    SERVER = ThreadingServer((HOST, PORT), CGIHandler)
    try:
        LOGGER.info("server start")
        SERVER.serve_forever()
    except KeyboardInterrupt:
        SERVER.shutdown()
        LOGGER.info("server shutdown")
        os.remove(PIDFILE)

def do_debug():
    if os.path.exists(PIDFILE):
        print "Already in running"
        return
    if not os.path.exists(WORKDIR):
        os.mkdir(WORKDIR, 0755)

    pfile = open(PIDFILE, "w", 0644)
    pfile.write("%s" % os.getpid())
    pfile.close()

    server = ThreadingServer((HOST, PORT), CGIHTTPServer.CGIHTTPRequestHandler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        server.shutdown()
        os.remove(PIDFILE)

def do_stop():
    try:
        pfile = open(PIDFILE, "r")
    except IOError:
        print "Not in running"
        return

    pid = pfile.readline()
    pfile.close()
    pid = int(pid)
    os.kill(pid, signal.SIGINT)

def print_usage():
    print "usage: %s <start | stop | debug>" % sys.argv[0]

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print_usage()
        sys.exit(1)

    if sys.argv[1] == "start":
        do_start()
    elif sys.argv[1] == "stop":
        do_stop()
    elif sys.argv[1] == "debug":
        do_debug()
    else:
        print_usage()
