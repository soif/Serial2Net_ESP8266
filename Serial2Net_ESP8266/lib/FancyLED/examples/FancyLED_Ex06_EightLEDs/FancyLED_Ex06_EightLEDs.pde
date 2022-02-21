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
 *           bytes. With multiple LEDs. Don't have to use the register-style
 *           instantiation to make multiples, btw.  
 *
 * Created 10 July 2011
 * by Carlyn Maw
 *
 */
 
#include <Button.h> 
#include <FancyLED.h> 

bool debugFlag = 0;
unsigned long currentTime = 0;

const byte numberOfblinkers = 8;
byte blinkPinArray[numberOfblinkers] = { 
  2, 3,4 ,5 ,6, 7, 8,9 } 
;

//the pit array of the pinstates for the blinkers
unsigned char blinkMe = 0;
//Instantiate Button on digital pin 2
//pressed = ground (pulled high with _external_ resistor)
Button helloButton = Button(2, LOW);

FancyLED solenoids[numberOfblinkers] = { 
  FancyLED(0, HIGH, &blinkMe), 
  FancyLED(1, HIGH, &blinkMe), 
  FancyLED(2, HIGH, &blinkMe),
  FancyLED(3, HIGH, &blinkMe), 
  FancyLED(4, HIGH, &blinkMe), 
  FancyLED(5, HIGH, &blinkMe),
  FancyLED(6, HIGH, &blinkMe), 
  FancyLED(7, HIGH, &blinkMe)
  };


  void setup()
  {
    Serial.begin(9600);


    //have to do outputs b/c using the solenoid register style
    for (byte i = 0; i < numberOfblinkers; i ++) {
      pinMode(blinkPinArray[i], OUTPUT);
    }


  }

void loop()
{

  helloButton.listen();  

  currentTime = millis();

  for (byte i = 0; i < numberOfblinkers; i ++) {
    solenoids[i].update(currentTime);
  }


  if (helloButton.onPress()) {
    
    for (byte i = 0; i < numberOfblinkers; i ++) {
      solenoids[i].pulse(5);
    }
    
  }

  blinkOut(blinkMe);
}


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
    Serial.println(myDataOut, BIN);
  }
}


