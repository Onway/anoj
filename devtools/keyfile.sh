#!/bin/bash

option()
{
    echo "[OPTION]"
    echo "TIME = 1000"
    echo "MEMORY = 65536"
    echo "FSIZE = 4096"
    echo "LANG = C"
    echo "DATADIR = /home/onway/wyuoj/data/1000"
    echo "WORKDIR = /tmp"
    echo ""
}

parser()
{
    echo "[PARSER]"
    echo "C = 0"
    echo "C++ = 0"
    echo "PASCAL = 0"
    echo "JAVA = 0"
    echo "PYTHON = 0"
    echo "PHP = 0"
    echo ""
}

resource()
{
    res="RLIMIT_AS RLIMIT_CORE RLIMIT_CPU RLIMIT_DATA RLIMIT_FSIZE RLIMIT_LOCKS RLIMIT_MEMLOCK RLIMIT_MSGQUEUE RLIMIT_NICE RLIMIT_NOFILE RLIMIT_NPROC RLIMIT_RSS RLIMIT_RTPRIO RLIMIT_RTTIME RLIMIT_SIGPENDING RLIMIT_STACK"

    echo "[RESOURCE]"
    for i in $res
    do
        echo "$i = RLIM_INFINITY;RLIM_INFINITY"
    done
    echo ""
}

environ()
{
    echo "[ENVIRON]"
    echo ""
}

signal()
{
    echo "[SIGNAL]"
    sig=`kill -l`
    for i in $sig
    do
        echo $i | grep -q '^SIG.*'
        test $? != 0 && continue
        echo "$i = 0"
    done
    echo ""
}

syscall()
{
    syscall=/usr/include/i386-linux-gnu/bits/syscall.h
    sys=`sed -n '9,$p' $syscall | awk '{print $2}'`

    echo "[SYSCALL]"
    for i in $sys
    do
        echo "$i = true"
    done
    echo ""
}

option
parser
resource
signal
syscall
environ
exit 0
