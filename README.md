The PIC revenge
======================

Intro
------
So, long time was past since last time I hacked with a PIC microcontroller on my own
(despite I use some dsPIC on my job, but this is another story).

I started to moving toward ARM stuff (both MCU and Linux Soc) and other stuff, my new
PC has not a parallel port for programming PICs and IIRC the last time I tried to compile
the IDE I used to develop on PICs on my Linux box, it failed, because the SW went
maintained.

My PIC drawer and my old parallel programmer started collecting dust.. Until now :)

Recently happened that a friend of mine, an excellent programmer, but a novice in
electronic, started to hack with his new RPi2, and started to get interested in MCUs
as well.

This revived my interest in my old PIC microcontrollers, and I had to find a way to
use them on a recent PC/SW, so that I could also make my friend able to experiment with
them. Also, of course, I wanted to go with only FOSS SW :)

After googling around I found "k8048" project, from Darron Broad.
[original site] (http://dev.kewl.org/k8048/Doc/)

It's a nice, FOSS, SW capable of programming a wide range of PICs microcontroller by
using, among other things, the RPi as programmer, exploiting some GPIOs!
Since for certain usages, an HW adapter is still needed, and since I have my old
programmer already done with proper socket for my PICs already soldered, I decided
to adapted my old parallel programmer to be driven by RPi2 :)

In this repo you can find a mirror of latest stable version of k8048, with some
patches and scripts by me (most notably RPi2 support) as well as some photos and
HW schematics.

Software
--------

The dotfiles directory contains my config file for k8048, that is suitable for
usage on RPi2, with my HW programmer (see the next chapter for details).

I also moved the default GPIO used in the original configuration sample file, so
that they do not collide with the RPi serial port (that I use as tty).

In the script directory you can find a script to setup GPIO before powering up the
programmer (so that input are inputs, output are outputs, VPP control is switched off)
as well as a script to manually turn on and off VCC (that is, my custom HW programmer
can switch on/off VPP and VCC as well, but the latter is not supported by k8048, so
you have to switch it on manually after running k8048, and switch it off before
removing the PIC from the socket.

Hardware
--------

So, I resumed my old hardware programmer and I removed a some dust from it, and I
checked its conditions..

![The old programmer](images/programmer.png)

The hardware programmer was done as per a schematic found over the internet years ago.

![programmer schematic](images/programmer_schem.gif)

The original schematic is in black, while the red stuff has been added for the RPi.
Basically the two transistors act as switches to enable/disable the VCC and VPP,
while the IC SN74LS07 is a Open Collector buffer.

I checked on the datasheet of the SN74LS07 if the 3.3V signals from the RPi were
sufficient to drive it, and it turned out that 2V is the recommended minimum high
level, so it's fine for the RPi.

The open-collector logic, plus the pullup resistor in the schematic make sure that
the PIC is programmed with proper voltage (5V) even if the RPi has 3.3V outputs.

The only problem was about the logic level of the data from the programmer to the
RPi, that has 5V high level. I don't know if the RPi inputs are 5V tolerant, I
think they aren't, and I wanted to go safe, and modify the circuit.

As you can see, in red, I have added a couple of resistors to lower the 5V signal
to about 3.3V. Maybe there are better solutions, but this was something that I
could readily made with spare parts I had at home :)

The jumper purpose is to eventually disable the resistor if I want to switch back
to 5V signals.

Finally I build a simple cable to connect the RPi GPIOs to the DB-25 connector of
my old parallel programmer.

![LPT RPi cable](images/cable.png)

And finally..
-------------

I connected all the things together, I put a PIC on the programmer socket and
I powered up the thing.

No smoke, that's a good indication :)

![The crappy-hacky setup](images/setup.jpg)

You can see other wires on the RPi; those are the serial cable (for tty), and
a reset cable, connected to a little ARM board you can spot on the left. I use it
to remotely reset the RPi when I hack in the RPi kernel and I'm not at home
(usually the girl house). The ARM board runs my UAKEH project and controls also
other stuff, but it's OT here :P

Finally I run k8048, and...

This is the first success log, of my setup correctly identifying a PIC16F871 :) :)

```
pi@raspberrypi:~$ k14 id
[0000] [PROGRAM]  0800 WORDS
[2000] [USERID0]  3FFF .
[2001] [USERID1]  3FFF .
[2002] [USERID2]  3FFF .
[2003] [USERID3]  3FFF .
[2004] [RESERVED] 0000
[2005] [RESERVED] 0000
[2006] [DEVICEID] 0D21 DEV:D20 REV:01 PIC16F871
[2007] [CONFIG]   3FFF
[2100] [DATA]     0040 BYTES
```