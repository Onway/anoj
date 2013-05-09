#!/usr/bin/env python
# Filename      : solution.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-05-04

import sys
import time
import json
import urllib
import urllib2
import xml.etree.ElementTree as ET

URL = "http://127.0.0.1:8888/cgi-bin/judger.py"

def request_judge(xmlfile):
    kdict = {}

    root = ET.parse(xmlfile).getroot()
    for solution in root:
        for ele in solution:
            if ele.tag == 'code':
                kdict[ele.tag] = ele.text.strip()
            else:
                kdict[ele.tag] = ele.text
        print 'sending request >>>', repr(kdict)
        print
        urllib2.urlopen(urllib2.Request(URL, urllib.urlencode(kdict)))
        # time.sleep(2)

def analyse_result(xmlfile, respath):
    pass

def print_usage(argv):
    print 'Usage: %s <xmlfile> [respath]' % argv[0]

if __name__ == '__main__':
    if not (2 <= len(sys.argv) <= 3):
        print_usage(sys.argv)
        exit(1)
    
    xmlfile = sys.argv[1]
    if len(sys.argv) == 3:
        respath = sys.argv[2]
        analyse_result(xmlfile, respath)
    else:
        request_judge(xmlfile)
