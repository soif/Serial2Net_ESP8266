/*
 * FancyLED Library Example 5
 * ------------
 *
 * Circuit: 
 *          - Button, pulled up externally with 10k resitor on pin 2
 *          - LEDs on Pins 2 through 9 (only the one on 9 is really used)
 *
 * Behavior: 
 *           Showing the potential for this lbrary to work with register
 *           bytes. Acts just like example 3. 
 *           Hook up all the LEDs and Change the value of blinkMe.
 *           See what changes.
 *
 * Created 10 July 2011
 * by Carlyn Maw
 *
 */

#include <Button.h> 
#include <FancyLED.h> 

bool debugFlag = 0;

const byte numberOfblinkers = 8;
byte blinkPinArray[numberOfblinkers] = { 
  2,3,4,5,6, 7, 8,9 } 
;

//the byte that acts as an array of the pinstates for 
//the blinkers, could be a byte to be shifted out to shift 
//register, etc. 
unsigned char blinkMe = 0;

//Instantiate Button on digital pin 2
//pressed = ground (pulled high with _external_ resistor)
Button helloButton = Button(2, LOW);

//Instantiate FancyLED on digital pin 9
//active = HIGH (could need to be low if inverting IC is being used...)  
//since blinkPinArray[3] is PIN 5 in this instantiation style
//we use 3 ("third bit in register")
FancyLED testLED = FancyLED(7, HIGH, &blinkMe);


void setup()
{
  Serial.begin(9600);
  testLED.setFullPeriod(1000);
  testLED.setDutyCycle(40);

  //have to do outputs b/c using the solenoid register style
  for (byte i = 0; i <= numberOfblinkers; i ++) {
    pinMode(blinkPinArray[i], OUTPUT);
  }


}

void loop()
{

  helloButton.listen();  
  testLED.update();

  if (helloButton.onPress()) {
    //will pulse once at current duty cycle and period settings
    // testLED.pulse();

    //will pulse 5 times at current duty cycle and period settings
    testLED.pulse(5);

    //will pulse once and update duty cycle and period settings
    //testLED.pulse(2000, 10);

    //will pulse 5 times and update duty cycle and period settings
    //testLED.pulse(5, 2000, 10);


    Serial.println(testLED.getFullPeriod());
    Serial.println(testLED.getDutyCycle());
  } 

  //normally you'd be shifting out if you were
  //using this version of the FancyLED or something. 
  blinkOut(blinkMe);

}

//normally you'd be shifting out if you were
//using this version of the FancyLED
void blinkOut(byte myDataOut) {
  int pinState;
  for (int t=7; t>=0; t--)  {
    if ( myDataOut & (1<<t) ) {
      pinState= 1;
    }
    else {	
      pinState= 0;
    }
    digitalWrite(blinkPinArray[t], pinState);
  }
  if (debugFlag) {
    Serial.println(myDataOut, DEC);
  }
}
