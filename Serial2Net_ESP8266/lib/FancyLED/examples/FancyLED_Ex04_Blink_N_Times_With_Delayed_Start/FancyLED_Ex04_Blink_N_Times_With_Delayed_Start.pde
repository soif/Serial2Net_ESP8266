/*
 * FancyLED Library Example 4
 * ------------
 *
 * Circuit: 
 *          - Button, pulled up externally with 10k resitor on pin 2
 *          - Extra LED on pin 9
 *
 * Behavior: 
 *         External LED blinks twice. LED on the board blinks twice 5
 *         seconds later.
 *
 * Created 17 August 2011
 * by Carlyn Maw
 *
 */

#include <Button.h> 
#include <FancyLED.h> 

unsigned long  loopCurrentTime = 0;

  //Instantiate Button on digital pin 2
  //pressed = ground (pulled high with _external_ resistor)
  Button helloButton = Button(2, LOW);
     
  //Instantiate FancySolenoid on digital pin 5
  //active = HIGH (could need to be low if inverting IC is being used...)  
  FancyLED externalLED = FancyLED(9, LOW);
  FancyLED boardLED = FancyLED(13, LOW);


void setup()
{
  Serial.begin(19200);
  externalLED.setFullPeriod(1000);
  externalLED.setDutyCycle(50);
  externalLED.turnOff();
  
  boardLED.setFullPeriod(1000);
  boardLED.setDutyCycle(50);
  boardLED.turnOff();
  
}

void loop()
{

  helloButton.listen();  
  externalLED.update();
  boardLED.update();
 
  
  if (helloButton.onPress()) {

    externalLED.pulse(1);
    boardLED.fusedPulse(5000,1);

  } 

}

