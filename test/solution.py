#!/usr/bin/env python
# Filename      : solution.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-05-04

import os
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
        time.sleep(2)

def print_html_head():
    print '''
<!DOCTYPE html
PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
    <title> autotest result </title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <style type="text/css">
        table {
            width: 70%;
            text-align: center;
        }
    </style>
    </head>
    <body>
        <table align="center">
            <tr>
                <th>run_id</th>
                <th>problem_id</th>
                <th>language</th>
                <th width="30%">description</th>
                <th>expect</th>
                <th>result</th>
            </tr>
'''

def print_html_foot():
    print '''
        </table>
    </body>
    <script type="text/javascript">
        var trs = document.getElementsByTagName("tr");
        for (i = 1; i < trs.length; i += 2) {
            trs[i].setAttribute("bgcolor", "#f0f0f0");
        }
    </script>
</html>
'''

def get_real_result(respath, rid):
    fpath = os.path.join(respath, rid)
    fd = open(fpath, "r")
    result = json.load(fd)
    return result["result"]

def analyse_result(xmlfile, respath):
    tags = ["rid", "pid", "lang", "desc", "expect"]

    print_html_head()
    root = ET.parse(xmlfile).getroot()
    for solution in root:
        print "<tr>"
        for tag in tags:
            print "<td>" + solution.find(tag).text + "</td>"
        print "<td>" + get_real_result(respath, solution.find("rid").text)\
                + "</td>"
        print "</tr>"
    print_html_foot()

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
