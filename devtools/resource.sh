#!/bin/bash

s="RLIMIT_AS RLIMIT_CORE RLIMIT_CPU RLIMIT_DATA RLIMIT_FSIZE RLIMIT_LOCKS RLIMIT_MEMLOCK RLIMIT_MSGQUEUE RLIMIT_NICE RLIMIT_NOFILE RLIMIT_NPROC RLIMIT_RSS RLIMIT_RTPRIO RLIMIT_RTTIME RLIMIT_SIGPENDING RLIMIT_STACK"

for i in $s
do
    if [ $i == "RLIMIT_AS" ] ; then
        echo "    if (!strcmp(key, \"$i\"))"
    else
        echo "    else if (!strcmp(key, \"$i\"))"
    fi
    echo "        tmp->resource = $i;"
done

echo "    else {"
echo "        free(tmp);"
echo "        return;"
echo "    }"
