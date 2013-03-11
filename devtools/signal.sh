#!/bin/bash

s=`kill -l`

for i in $s
do
    echo $i | grep -q '^SIG.*'
    test $? != 0 && continue
    if [ $i == "SIGHUP" ] ; then
        echo "    if (!strcmp(key, \"$i\"))"
    else
        echo "    else if (!strcmp(key, \"$i\"))"
    fi
    echo "        signal_rule[$i] = value;"
done
