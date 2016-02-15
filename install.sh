#!/bin/bash

mkdir -p /etc/anoj


grep -q anojer /etc/passwd
if [ $? -ne 0 ] ; then
    useradd -m -r -U -s /bin/bash anojer
fi


cd judge
make
make install


cd ..
cp -f mini-httpd.conf /etc/
cp -rf web /home/anojer
chown -R anojer:anojer /home/anojer/web


if [ ! -d /home/anojer/test ] ; then
    cp -rf test /home/anojer/
    chown -R anojer:anojer /home/anojer/test
fi


if [ ! -d /home/anojer/data ] ; then
    mkdir /home/anojer/data
    chown -R anojer:anojer /home/anojer/data
    chmod 0750 /home/anojer/data
fi


if [ ! -d /home/anojer/history ] ; then
    mkdir /home/anojer/history
    mkdir /home/anojer/history/submit
    mkdir /home/anojer/history/result
    chown -R anojer:anojer /home/anojer/history
fi

cd judge
make clean
cd ..
