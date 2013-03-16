#!/bin/bash

option()
{
    echo "# 该组可被程序的命令行选项覆盖。"
    echo "# 实际应用中，除了WORKDIR之外，都应该使用命令选项"
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
    echo "# 评测一个空程序的得到的内存使用，"
    echo "# C，C++，PASCAL为编译型语言，应设为0"
    echo "# 单位kb"
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

    
    echo "# 用户程序和spj程序的资源限制"
    echo "# 用户程序忽略RLIMIT_AS,RLIMIT_CPU,RLIMIT_FSIZE"
    echo "# spj程序忽略RLIMIT_FSIZE"
    echo "# 如果没有指定RLIMIT_CPU,RLIMIT_AS，则spj程序回退到用户程序"
    echo "# 指定的时间和内存限制"
    echo "[RESOURCE]"
    for i in $res
    do
        echo "$i = RLIM_INFINITY;RLIM_INFINITY"
    done
    echo ""
}

environ()
{
    echo "# 用户进程和spj程序的环境变量"
    echo "# 除了从父进程继承而来的环境变量以外，"
    echo "# 下面的键值对用于添加一个环境变量，"
    echo "# 如果值为空，则删除指定键的环境变量"
    echo "[ENVIRON]"
    echo ""
}

signal()
{
    echo "# 用户程序和spj程序的信号处理方式"
    echo "# 值为0，则不将信号传递到程序"
    echo "# 值为1，则将信号传递到程序"
    echo "# 值为2，则终止程序"
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

feedback()
{
    echo "# 下面的键值用于指定用户程序由于收到被禁止的信号而被终止时，"
    echo "# 向用户反馈的提示信息"
    echo "[FEEDBACK]"
    echo "SIGFPE = Floating point exception"
}

syscall()
{
    syscall=/usr/include/i386-linux-gnu/bits/syscall.h
    sys=`sed -n '9,$p' $syscall | awk '{print $2}'`

    echo "# 用户程序和spj程序的系统调用规则"
    echo "# 值为true，则该系统调用被允许使用"
    echo "# 值为false，则系统调用被禁用，终止程序"
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
feedback
exit 0
