/*
 * FancyLED Library Example 2
 * ------------
 *
 * Circuit: 
 *          - Button, pulled up externally with 10k resitor on pin 2
 *          - Extra LED on pin 9
 *
 * Behavior: 
 *
 * Created 4 April 2010
 * Updated 29 October 2010
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
    externalLED.turnOn();
    boardLED.turnOff();
  
}

void loop()
{

  helloButton.listen();  


  if (helloButton.onPress()) {
    externalLED.toggle();
    boardLED.toggle();
  }

}

