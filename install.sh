#!/bin/bash

mkdir /etc/wyuoj
grep -q wyuojer /etc/passwd
if [ $? -ne 0 ] ; then
    useradd -m -r -U -s /bin/bash -p wyuojer wyuojer
fi

cd judge
make
make install
make clean

cd ..
cp -f wyuoj.ini /etc/wyuoj
cp -rf web /home/wyuojer
chown -R wyuojer:wyuojer /home/wyuojer/web

if [ ! -d /home/wyuojer/data ] ; then
    mkdir /home/wyuojer/data
    chown -R wyuojer:wyuojer /home/wyuojer/data
fi
