#!/usr/bin/env python
# Filename      : cgi-bin/analyse.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-05-04

import json
import xml.etree.ElementTree as ET

print "Content-Type: application/json"
print

print '''
[
{"rid": "1", "pid": "1000", "desc": "hello world", "lang": "c", "expect": "Accepted", "result": "Wrong Answer"},
{"rid": "1", "pid": "1000", "desc": "hello world", "lang": "c", "expect": "Accepted", "result": "Wrong Answer"},
{"rid": "1", "pid": "1000", "desc": "hello world", "lang": "c", "expect": "Accepted", "result": "Wrong Answer"}
]
'''
