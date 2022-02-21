#include <SyncLED.h>

// Create an LED instance on pin 13 (with default settings - no PWM)
SyncLED StatusLight(13);

void setup() {

	// Blink the LED 5 times with a duration of 200ms between blinks
	// and an automatic 4 x Duration (800 milliseconds) delay between series of blinks
	//
	// This is useful for setting a simple blink status code
	// If you do not want the pause between series, please see the
	// examples/blink_custom and examples/blink_basic2 to setup a repeating
	// custom pattern
	
	StatusLight.blinkPattern( (byte) 5, 200UL );
	
}

void loop() {
  StatusLight.update();
}