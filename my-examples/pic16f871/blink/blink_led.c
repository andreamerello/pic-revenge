// Modified by Andrea Merello,
// Based on pic14 example
// Copyright (C) 2014 Diego Herranz

//#define NO_BIT_DEFINES
#include <pic14regs.h>
#include <stdint.h>
//#define __16F871

// Oscillator RC.
// disable watchdog,
// and disable low voltage programming.
// disable code protect
// enable brown out reset
// The rest of fuses are left as default.
__code uint16_t __at (_CONFIG) __configword = _RC_OSC & _WDT_OFF & _LVP_OFF & _BOREN_ON & _CP_OFF;


// Uncalibrated delay, just waits a number of for-loop iterations
void delay(uint16_t iterations)
{
	uint16_t i;
	for (i = 0; i < iterations; i++) {
		// Prevent this loop from being optimized away.
		__asm nop __endasm;
	}
}

void main(void)
{
	TRISC = 0xfe; // Pin as output
	PORTC = 0;

	while (1) {
		PORTC = 1; // LED On
		delay(10000); // ~500ms @ 4MHz
		PORTC = 0; // LED Off
		delay(10000); // ~500ms @ 4MHz
	}
}
