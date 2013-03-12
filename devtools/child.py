#!/usr/bin/env python
# Filename      : child.py
# -*- coding: utf-8 -*-
# Author        : Yiu Yi <aluohuai@126.com>
# Create Date   : 2013-03-12

import os
import resource

res = {
resource.RLIMIT_CPU : 'RLIMIT_CPU', 
resource.RLIMIT_CORE : 'RLIMIT_CORE', 
resource.RLIMIT_FSIZE : 'RLIMIT_FSIZE', 
resource.RLIMIT_DATA : 'RLIMIT_DATA', 
resource.RLIMIT_STACK : 'RLIMIT_STACK', 
resource.RLIMIT_RSS : 'RLIMIT_RSS', 
resource.RLIMIT_NPROC : 'RLIMIT_NPROC', 
resource.RLIMIT_NOFILE : 'RLIMIT_NOFILE', 
resource.RLIMIT_MEMLOCK : 'RLIMIT_MEMLOCK', 
# resource.RLIMIT_VMEM : 'RLIMIT_VMEM', 
resource.RLIMIT_AS : 'RLIMIT_AS', 
}

if __name__ == '__main__':
    msg = raw_input()
    print '[IO]'
    print msg

    print '[WORKDIR]'
    print os.getcwd()

    print '[ENVIRON]'
    print os.environ

    print '[RESOURCE]'
    for key in res.keys():
        print res[key] + ': ', resource.getrlimit(key)

    exit(0)
