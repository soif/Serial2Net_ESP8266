#include <SyncLED.h>

// Create an LED instance on pin 13 (with default settings - no PWM)
SyncLED StatusLight(13);

void setup() {

	// Blink SOS Pattern
	// This pattern length is the maximum of 32 bits - we cannot go longer with an unsigned long
	StatusLight.blinkPattern( 0B000000101010001110111011100010101UL, 150, 32 );
}

void loop() {
  StatusLight.update();
}