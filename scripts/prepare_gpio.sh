#!/bin/bash

nVPP=21
nVCC=16
DO=20
DI=19
CK=26
GPIODIR=/sys/class/gpio

echo $nVPP | sudo tee $GPIODIR/export > /dev/null
echo $nVCC | sudo tee $GPIODIR/export > /dev/null
echo $DO | sudo tee $GPIODIR/export > /dev/null
echo $CK | sudo tee $GPIODIR/export > /dev/null
echo $DI | sudo tee $GPIODIR/export > /dev/null

echo in | sudo tee $GPIODIR/gpio$DI/direction > /dev/null

echo high | sudo tee $GPIODIR/gpio$nVPP/direction > /dev/null
echo high | sudo tee $GPIODIR/gpio$nVCC/direction > /dev/null

echo low | sudo tee $GPIODIR/gpio$DO/direction > /dev/null
echo low | sudo tee $GPIODIR/gpio$CK/direction > /dev/null
