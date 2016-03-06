#!/bin/sh

ID=`id -u`
if test $ID -ne "0"; then
        echo "Need to be root"
        exit 1
fi

rmmod gpio-bb 1>/dev/null 2>/dev/null
rm -f /dev/gpio-bb

mkdir -p /lib/modules/3.10.25+/kernel/local/
cp ~/gpio-bb.ko /lib/modules/3.10.25+/kernel/local/
chown -R 0:0 /lib/modules/3.10.25+/kernel/local/
depmod -a
sync

modprobe gpio-bb

exit 0
