#include <SyncLED.h>

// Create an LED instance on pin 3 (pwm)
// 	Initial state is LOW (off)
// 	Use PWM is true (LED will fade between states - works only on PWMable pins)
//  fade time is 200ms - it will take that long to fully fade in or out
SyncLED StatusLight(3, LOW, true, 200UL);

void setup() {

	// Blink the LED 5 times with a duration of 200ms between blinks
	// and an automatic 4 x Duration (800 milliseconds) delay between series of blinks
	//
	// Note that 500ms rate allows for 200ms of PWM fade time and 300ms of on or off time
	// If it were less than 200ms, the LED would not reach full brightness before jumping
	// to full brightness to fade out from 100% since fadeout begins at 100% duty
	// 
	StatusLight.blinkPattern( (byte) 5, 500UL );
	
}

void loop() {
  StatusLight.update();
}