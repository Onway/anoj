#!/bin/bash


syscall=/usr/include/i386-linux-gnu/bits/syscall.h

s=`sed -n '9,$p' $syscall | awk '{print $2}'`
for i in $s
do
    if [ $i == "SYS__llseek" ] ; then
        echo "    if (!strcmp(key, \"$i\"))"
    else
        echo "    else if (!strcmp(key, \"$i\"))"
    fi
    echo "        syscall_rule[$i] = value;"
done
