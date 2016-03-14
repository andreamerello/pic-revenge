Blink LED
=========

Blink an LED approximately at a 1 Hz frequency.

### Hardware for this example
 - MCLR input pin must be connected to VDD.
 - An LED must be connected to pin RC0. LED anode must be connected to the pin through a ~330 ohm resistor and the LED cathode must be connected to GND.
 - Config bits are set in order to user an RC oscillator. You need to fit a resistor between VCC and OSCIN pin, and a capacitor between GND and OSCIN pin. I used a 10K resistor and a 100pF ceramic capacitor.

### Notes
 - Since SDCC doesn't provide delay funcions for pic14 port, a simple and uncalibrated delay function is defined using a for loop.
 - `#include <stdint.h>` provides definitions for types like `uint8_t`, `uint16_t`, `int8_t`, etc.
