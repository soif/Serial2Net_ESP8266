#include <SyncLED.h>

// Create an LED instance on pin 13 (with default settings - no PWM)
SyncLED StatusLight(13);

void setup() {

	// Blink a custom pattern indicated by a binary number
	// Bits a read from right to left
	// 0B0101UL indicates On, Off, On, Off (in that order)
	// Each bit represents 200ms of time
	// The pattern length is 4 bits
	//
	// Note the UL on the pattern to cast it as a unsigned long
	// This is required because of a similar call which takes a byte argument
	StatusLight.blinkPattern( 0B0101UL, 200, 4 );
}

void loop() {
  StatusLight.update();
}