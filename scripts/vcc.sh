#!/bin/bash

nVCC=21
GPIODIR=/sys/class/gpio

if [ $# != 1 ]; then
    echo "please specify (only) on or off"
    exit -1
else
    if [ $1 == "on" ]; then
	val=0
    else
	val=1
    fi
    echo $val | sudo tee $GPIODIR/gpio$nVCC/value > /dev/null
fi
