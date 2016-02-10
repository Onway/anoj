#!/usr/bin/env python
# Filename      : datalist.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-05-04

import os
import sys
import xml.etree.ElementTree as ET

if len(sys.argv) != 3:
    print 'usage: %s <xmlfile> <datadir>' % sys.argv[0]
    exit(1)

XMLFILE = sys.argv[1]
DATADIR = sys.argv[2]

problemset = ET.parse(XMLFILE).getroot()
for problem in problemset:
    dirname = problem.attrib['id']
    os.chdir(DATADIR)
    if os.path.exists(dirname):
        os.system('rm -rf %s' % dirname)
    os.mkdir(dirname)

    os.chdir(dirname)
    for f in problem:
        filename = f.attrib['name']
        fd = open(filename, 'w')
        fd.write(f.text.strip())

        if f.attrib.has_key('exec') and f.attrib['exec'] == 'true':
            os.fchmod(fd.fileno(), 0755)
        
        fd.close()

