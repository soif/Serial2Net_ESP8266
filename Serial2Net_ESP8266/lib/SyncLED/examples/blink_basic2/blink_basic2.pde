#include <SyncLED.h>

// Create an LED instance on pin 13 (with default settings)
SyncLED StatusLight(13);

void setup() {
	// Define a pattern here
	// This pattern is the equivilent of examples/blink_basic (400ms on, 200ms off)
	StatusLight.blinkPattern( 0B001111UL, 100, 6);
}

void loop() {
  StatusLight.update();
}