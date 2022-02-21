/*
 * FancyLED Library Example 1
 * ------------
 *
 * Circuit: 
 *          - Button, pulled up externally with 10k resitor on pin 2
 *          - Extra LED on pin 9
 *
 * Behavior: 
 *          Two LED's when one is on, the other is off - based on button
 *          state. 
 *
 * Created 4 April 2010
 * Updated 17 August 2011
 * by Carlyn Maw
 *
 */
 
#include <Button.h> 
#include <FancyLED.h> 

  //Instantiate Button on digital pin 2
  //pressed = ground (i.e. pulled high with external resistor)
  Button helloButton = Button(2, LOW);
  
  FancyLED externalLED = FancyLED(9, HIGH);
  FancyLED boardLED = FancyLED(13, HIGH); 

void setup()
{

  
}

void loop()
{

  helloButton.listen();  


  if (helloButton.isPressed()) {
    externalLED.turnOn();
    boardLED.turnOff();
  }


  if (helloButton.isReleased()) {
    externalLED.turnOff();
    boardLED.turnOn();

  }

}

