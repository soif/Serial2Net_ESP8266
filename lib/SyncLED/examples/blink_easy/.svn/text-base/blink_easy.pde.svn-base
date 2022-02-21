#include <SyncLED.h>

// Create an LED instance on pin 13 (with default setting- no PWM)
SyncLED StatusLight(13);

void setup() {

	// Blink the LED 5 times with a default duration of 1000ms between blinks and
	// a delay of 4 x duration (4 seconds) between series of blinks (a pause)
	// Note the type of "byte" being used to differentiate from the unsigned long
	// used in the more advanced examples
	StatusLight.blinkPattern( (byte) 5);
	
}

void loop() {
  StatusLight.update();
}