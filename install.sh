#!/bin/bash

mkdir -p /etc/wyuoj
grep -q wyuojer /etc/passwd
if [ $? -ne 0 ] ; then
    useradd -m -r -U -s /bin/bash wyuojer
fi

cd judge
make
make install

cd ..
cp -f wyuoj.ini /etc/wyuoj
cp -rf web /home/wyuojer
chown -R wyuojer:wyuojer /home/wyuojer/web

if [ ! -d /home/wyuojer/data ] ; then
    mkdir /home/wyuojer/data
    chown -R wyuojer:wyuojer /home/wyuojer/data
fi

if [ ! -d /home/wyuojer/history ] ; then
    mkdir /home/wyuojer/history
    mkdir /home/wyuojer/history/submit
    mkdir /home/wyuojer/history/result
    chown -R wyuojer:wyuojer /home/wyuojer/history
fi