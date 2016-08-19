The "poor man" PIC programmer
=============================

So, after all I started this project to provide tools for PIC-programming to a friend of mine, but
I regained interest in PICs me too, so I wanted to provide him a PIC programmer while keeping my
old one for me.

I decided to design a new programmer, provide him some transistors and let him have fun on the breadboard.

In this programmer there is no VCC control (because it is not even supported by pickle) while we still have
the VPP control.

You find the schematic below. I'm going to try to explain it as much easily as I can.. (well I re-read this explanation, and I'm sure it's by far more complicated than it should.. I may rewrite it in future..)

![new programmer] (images/prog2.jpg)

Schematic explanation
----------------------

Basically you can see three main blocks:
- The first one (upper) is the simplest, it employs a NPN transistor, and it manages the PGC signal (ProGramming Clock)
- The second one (middle) is very similar to the previous one, but it has few extra components. It manages the PGD (ProGramming Data)
- The last one seems very different, but if you look at it carefully, you'll note that it is composed by two sub-blocks; the one at the left is
indeed about the same thing of the previous two blocks. This whole block is the Vpp switch circuit.

Other than those three blocks you can see a couple of extra capacitors (at the very bottom) attached to Vcc and 13V (Vpp) rails. These two capacitors just filters the power supplies and make sure the supplies can handle sudden current sink increases. Technically we call these capacitors "by-pass capacitors" and you'll probably find a lot of them in commercial apparatus, typically at least one nearby each integrated circuit.

For the purpose they serve, it is important to fit them as much close as possible to the supply "user", and to keep their wires as short as possible. That is, you place the one for the 13V close to the PNP transistor, and the one for Vcc close to the PIC.

Theoretically you should place both capacitors close to the PIC, but since I used a quite small PNP transistor (because I had it at home), and since the capacitor causes short, but large, current inrushes every time the voltage is applied, I preferred to stay safe with respect to the trasistor's ability to handle large current spikes.

If you haven't these components available, then you can *try* omitting them, but I can't assure you anything. BTW putting, say, 10nF, instead of 100nF is better than nothing.

Theoretically the bigger capacity, the better. In practice it isn't so simple, because the components are not ideal, so the bigger they are, the bigger is their unpleasant parastic inductance, and because of the "inrush current" thing I explained above.

Now let's move to the real "core". Let's take a look to the first block (upper).

The open-collector output
-------------------------

Remember that we have an RPi that drives its GPIO with 3.3V signals. What this means?

This means that when the RPi wants to represent a logic *H*, it will apply 3.3V to the
GPIO, and when it wants to represent a logic *L*, it will put the GPIO to 0V.

So, it is like a 3.3V generator with a switch on its "+" wire, operated by the CPU:
when the switch is closed, the 3.3V are available on the output (GPIO);
when the switch is opened, the output will go to 0V. We can emulate a RPi GPIO this way, right?

...**NO!!**, this is not exactly what happens!

Let's assume that you connect a LED (with the proper resistor) to GND and to the RPi GPIO.
The LED:
- switches on when the RPi GPIO has *H* level
- switches off when the RPi GPIO has *L* level

Now connects the LED to your second circuit (the one with the switch)

```                   dummy RPi ~ external circuit
                                ~
 +------------+  +3.3V     /    ~      led     R
 | Generator  |-----------+  o--------I>|---/\/\/----
 |    3.3V    |-------   switch ~                    |
 +------------+      |          ~                   GND
                    GND         ~
```

The LED:
- switches on when the switch is closed
- switches off when the switch is opened

Ok, for now nothing weird :)

But now let's do this change: Connect the LED (with it's resistor) to the RPi GPIO and to the 3.3V

- switches *off when the RPi GPIO has *H* level
- switches *on* when the RPi GPIO has *L* level

We have swapped on/off states with respect to logic levels, but we are still able to control
the LED.

This means that:
- when the RPi GPIO is @0V, then *current* can run from the +3.3V to the GND through the RPi GPIO
- when the RPi GPIO is @3.3V, then *current* can run from the RPi GPIO @ +3.3V to the GND

Now repeat the experiment with the switch

```                   dummy RPi ~ external circuit
                                ~                 +3.3V
 +------------+  +3.3V     /    ~      led     R    |
 | Generator  |-----------+  o--------I>|---/\/\/----
 |    3.3V    |-------   switch ~
 +------------+      |          ~
                    GND         ~
```

Oooh.. nothing works anymore! Our dummy RPi fails, we cannot control the LED! What happened?

Well, you have probably already guessed:
- The RPi GPIO "connects" to the GND or to the 3.3V depending by logic state. The LED
  lights when the GPIO is connected to GND
- The dummy RPi "connects" to the 3.3V or remains *floating* depending by switch state

No current can flow through a *floating* thing..

If you measure the Voltage at the output of our RPi you may be surprised that it's
*not* zero, even when the switch opened.

This is because our fake GPIO, once is left *floating* is actually connected to the +3.3V
through the LED (and its resistor).

This is *very similar* to what we call "open-collector". This a kind of output that can
only *source* current, but it cannot *sink* current!

The real RPi outputs are what we call "totem pole" or "push-pull". They can both source and sink (relatively large amount of) current.

The "real" open-collector
--------------------------
*well, an open collector, is exactly the opposite: it can sink current, and it cannot source*

(I'm going to omit the generator from the schematic. I'm just using "GND" and "3.3V" labels)


```    dummy RPi   ~ external circuit
                   ~
         /         ~     led     R
     ---+  o------------|<I---/\/\/---
     |  switch     ~                 |
    GND            ~                GND

```

```    dummy RPi   ~ external circuit
                   ~               +3.3V
         /         ~    led     R    |
     ---+  o------------|<I---/\/\/---
     |  switch     ~
    GND            ~

```

These two circuits are the alter-egos of the above twos.
However this time, the first one (LED attached to GND) does not work, the second does.

So this time our dummy RPi has a real *open-collector* output: It can *sink* current,
but it cannot *source* current.

That is, when the switch is close, we can *firmly* sustain 0V condition, when the switch
is opened, our output is *floating*, and its only connection become the R and LED path
to the 3.3V - if you measure the voltage with a DMM you will see about 3.3V (don't touch
anything with your fingers, otherwise your measure will be not precise).

Open-collector with *pull-up* resistor
--------------------------------------

Now look at this

```
       dummy RPi        ~ external circuit
                Rpup    ~
         3.3V --/\/\--  ~
                     |  ~
          /          |  ~   led     R
  -------+  o--------+------|<I---/\/\/---
  |    switch           ~                |
 GND                    ~               GND
                        ~
```


What does happen here ?

Well, when the switch is closed, it's almost the same; you just waste a, possibly small,
amount of current, that flows thought *Rpup*. LED is off, because you have 0V at both
its leads, and no current can flow.

But when the switch is opened the LED will lit. It will be less bright, but it will lit.

The current flows thought the LED, it's R, and *Rpup*.
The voltage at the output of out GPIO would be neither 0V or 3.3V. The more the "load" (the led + its R) draws current, the lower the V will be.

Let's simplify things. Let's look again at these two circuits:


```
       dummy RPi        ~ external circuit
                Rpup    ~
         3.3V --/\/\--  ~
                     |  ~          3.3V
          /          |  ~    Rload   |
  -------+  o--------+-------/\/\/---
  |    switch           ~
 GND                    ~
                        ~
```


```
       dummy RPi        ~ external circuit
                Rpup    ~
         3.3V --/\/\--  ~
                     |  ~
          /          |  ~     Rload
  -------+  o--------+-------/\/\/---
  |    switch           ~            |
 GND                    ~           GND
                        ~
```


Here, all is the same, but we have no LEDs, so we can easily apply Ohm law.
The first circuit is trivial:
- switch closed: V on the GPIO = 0V; I(Rpup) = 3.3/Rpup; I(Rload) = 3.3/Rload - current flows
- switch opened: V on the GPIO = 3.3V; both leads of Rpup and Rload are @3.3V, so voltage *across* them is zero; I(Rpup) = 0/Rpdn; I(Rload) = 0/Rload - no current flows

It's evident that:
1) the output voltage is nominal in both cases (3.3V when the switch is opened, 0V when its closed)

2) the Rload can be either large or small. In the latter case large currents can flow, but nothing weird happens.

In the second circuit:
- switch closed: V on the GPIO = 3.3V; I(Rpup) = 3.3/Rpup; I(Rload) = 0/Rload
- switch opened: V on the GPIO = X; I(Rpup) = I(Rload) = 3.3V/(Rpup + Rload).

Calculating "X" is easy: It's equal to the V across Rload. We know the current flowing through Rload (let's call it *I*), so:

X = Rload * I = Rload * 3.3V/(Rpup + Rload)

Now it's evident that:
2) if our Rload is very large, a lot larger than Rpup, this equation can be approximated with the following: X = Rload * 3.3V/Rload = 3.3V. X is approaching to 3.3V.
1) if Rload is very small (let's think to zero), we have 0 * 3.3V/Rpup = 0. X is zero.

So far we can require to our dummy raspberry:
- To sink huge currents (as much as the generator can provide), if we connect our load to 3.3V (first schematic), and all works OK.
- To source **small** currents, if we connect our load to GND (second schematic), and all work OK.

We have converted back our open-collector-like dummy-raspberry in something that is close to the original RPi output!

We can say that, *if the load does not draw too much current* our circuit can work in place of a totem-pole output.

Connecting the dummy-rpi to the real rpi
----------------------------------------

Now let's try to control our dummy RPi with a real RPi..
We need to remove our switch, and exchange it with a NPN transistor.


```
RPi  ~      dummy RPi    ~ external circuit
     ~           Rpup    ~
     ~    3.3V --/\/\--  ~
     ~                |  ~
     ~                |  ~     Rload
     ~       |/-------+-------/\/\/---
GPIO--/\/\---| T         ~            |
     ~ Rlim  |\          ~           GND
     ~         |         ~
     ~	     GND
```

The transistor T act exactly as a remotely-operated switch.
Our circuit is the same as above, just the switch is controlled by a RPi GPIO.

So what happens exactly?
- when the RPi GPIO is high the switch (transistor) closes, and we get 0V on the output (no current in Rload)
- when the Rpi GPIO is low the switch (transistor) opens, and we get (nearly) 3.3V on the output (provided that the load draws small current amount, that is Rload is >> than Rpup

We have basically reproduced the raspberry GPIO on our output. We have two side effects however
1) the logic states are inverted (but this is not a problem, just reverse them in SW)
2) the current the load can draw must be very mild.

Rlim is just to limit the current am-out flowing from the RPi to our circuit. Is is required not to fry the RPi and/or the transistor.

This is a true open-collector. The name "open-collector" is because of the the upper leg of our transistor (that is called "collector"); it would be basically "open", "floating" .. and we put Rpup to avoid this...

Open-collector as voltage shifter
---------------------------------

Now, you may wonder why the heck should we ever want to complicate thinks like above..

Well one possible motivation is that we can achieve a voltage shifter by exploiting this kind of circuit!
What does happen if we supply 5V to our circuit?

```
RPi  ~      dummy RPi    ~ external circuit
     ~           Rpup    ~
     ~      5V --/\/\--  ~
     ~                |  ~
     ~                |  ~     Rload
     ~       |/-------+-------/\/\/---
GPIO--/\/\---| T         ~            |
     ~ Rlim  |\          ~           GND
     ~         |         ~
     ~	     GND
```

- when the RPi GPIO is high the switch (transistor) closes, and we get 0V on the output (no current in Rload)
- when the Rpi GPIO is low the switch (transistor) opens, and we get (nearly) 5V on the output (provided that the load draws small current amount, that is Rload is >> than Rpup

Wow! we can now control a 5V output with our RPi!
This is what we needed!



A default value
---------------

The above circuit is definitely enough. It works.
However we can improve it a little..

Consider that our dummy RPi (well, let's call it open-collector level shifter) is connected to a RPi. But what does happen if the RPi GPIO is not configured yet (it's still an input, and we did not configured it as output yet), or if we have not physically connected or circuit to the RPi yet?

The output state is basically unpredictable! the voltage at the input of our circuit is undefined. It can be anything. If you touch the input with a finger the voltage will jump high to low or vice-versa. Our circuit, left alone, goes crazy!

This is not necessarily a problem, however it would be as if we send random commands to a PIC eventually attached. It's better to avoid it, in order not to perform unwanted actions..

Fortunately there is a trick to force it to assume a "default" value in this kind of situations!

```
RPi  ~          dummy RPi    ~ external circuit
     ~               Rpup    ~
     ~          5V --/\/\--  ~
     ~                    |  ~
     ~                    |  ~     Rload
     ~           |/-------+-------/\/\/---
GPIO--/\/\---+---| T         ~            |
     ~ Rlim  |   |\          ~           GND
     ~       \      |        ~
     ~	     /     GND
     ~   Rpd \
     ~       |
     ~	    GND
```

We have added the resistor Rpd. What happens?

When the RPi is attached, and it's GPIO is driving our circuit, nothing changes.
We have a (little) extra current drawn from the RPi when it's GPIO is high, that flows through Rpd, but this does not cause any effect at all.

However, when the RPi is detached, or it's GPIO is not driving our input, the voltage on the input is "defaulted" to zero by the Rpd resistor!

This way the default value is zero on the input, that means 5V on the output. Could we do also the opposite? Well.. Yes and no..

If we connect Rpd (that is "Resistor Pull-Down") to +5V (and we have to change it's name in "Rpup") we can achieve this.. But there are drawbacks!
1) If the RPi GPIO is configured as an input, we are basically pushing 5V into the RPi GPIO, flowing through Rpup and Rlim. This is not good!

2) We are in the "less-safe" state: our output is firmly driven at 0V. If something odd happens, the output is stiff, and it will desperately try to remain to 0V even at the cost to burnout the transistor (thing about a short-circuit, let's say we have a zero-ohm Rload because something metallic is fallen on the PIC socket connector). On the other hand we know that the 5V state is weak. Whatever it happens we'll provide mild current, and it's more unlikely to cause damages with mild currents ;)


That's it!
----------

Now look at our last circuit and the first block in the programmer schematic. Do you note something? :)


Another use of open-collectors
------------------------------
We have seen that the open-collector circuit can be used to achieve level-shifting. But we can do more with it!

Look at the second block in the programmer schematic. It's identical to the first one, but there are two extra resistors.. Why?

This circuits can *also* work as the first one: when the RPi drives it's GPIO (with zero or 3.3V), we get the output assume 5V and 0V respectively.However another scenario is possible..

Suppose that the PIC wants to send data back to the RPi by using the same pin. Actually the second block is connected to the PIC PGD pin, that *is* a bidirectional data signal. This means that when we send proper commands to the PIC on the PGD pin, the PIC stop listening to signals coming from PGD, and switch it's PGD pin to output mode! It's like configuring the RPi GPIOs as inputs and outputs.. The PIC does this dynamically depending by the command we send him.

If our RPi is driving it's GPIO high (thus our circuit output is low), it would be a problem: our circuit pushes low PGD line firmly, while the PIC is pretending to drive it high and low at it's own desire. This is a short circuit!

But if we are smart enough (and hopefully our SW is), and we put our RPi output low just before the PIC starts to transmit, then our open-collector just barely tries to put it's output to 5V. Remember that the current is mild!

In this conditions, the PIC will win! it's totem-pole drive, is capable to both sink and source a (relatively) big current amount; some current will flow in the Rpup, but this is not an issue, and the PGD pic PIN can freely be high or low as per PIC desire.

So, if we could route this signal to a RPi input, we could listen to the PIC when it's driving the PGD output. This is exactly what we have done here.
We have also a wire to a RPi input that serves for this purpose!

However remember that the PIC works at 5V, while our RPI works at 3.3V.. We need another voltage shifter! Fortunately this time is easier: we have to *lower* the voltage, and for this purpose a couple of resistors are enough!

The voltage divider
-------------------

Consider the following circuit
```
    +5V
    |
    |
    \
    / R1
    \
    |
    +------- output
    |
    \
    / R2
    \
    |
    |
   GND
```

What's on the output?

Suppose that R1 == R2. We would get 2.5V ! This can be calculated using the Ohm law.

Remember that I = V / R, thus the total current flowing from the 5V source and the GND is I = 5 / (R1 + R2), And V(R2) is V = R2 * I.
So the voltage across R2 (that is the same as the voltage on the output) is
V = R2 * (5 / (R1 + R2))

So in our example with R1 == R2

V = R * 5 / 2R = 5 / 2 = 2.5V

Is this true for whatever R value ? Yes and no!

Huge R value will works until the current we can provide on output becomes too mild to be useful. Lowering R value will make the output more "stiff" (that is more current can be drawn without affecting the voltage too much), but more current will also from through R1 and R2. The resistor would heat up, and our voltage source struggles.

This kind of circuit can be used only when mild current output are required! I wouldn't use it for power feeding a device. This is OK only to represent logical values !


That's it (2)
-------------

If you combine our first block with the voltage divider the you'll obtain the second block in the schematic programmer.

Resistor values are carefully chosen in such a way that current through R1 and R2 is mild enough to be tolerated by open collector circuit
(RPup << (R1 + R2)) and so that a 5V value would be translated to a (about) 3.3V value to the RPi !