/*
 * FancyLED Library Example 3
 * ------------
 *
 * Circuit: 
 *          - Button, pulled up externally with 10k resitor on pin 2
 *          - Extra LED on pin 9
 *
 * Behavior: 
 *          LED blinks 5 times after button is pressed. Can set and get 
 *          timing variables.
 *
 * Created 4 April 2010
 * Updated 17 August 2011
 * by Carlyn Maw
 *
 */

#include <Button.h> 
#include <FancyLED.h> 

unsigned long  loopCurrentTime = 0;

  //Instantiate Button on digital pin 2
  //pressed = ground (pulled high with _external_ resistor)
  Button helloButton = Button(2, LOW);
     
  //Instantiate FancyLED on digital pin 9
  //active = HIGH (could need to be low if inverting IC is being used...)  
  FancyLED externalLED = FancyLED(9, HIGH);


void setup()
{
  Serial.begin(9600);
  externalLED.setFullPeriod(1000);
  externalLED.setDutyCycle(10);
  
}

void loop()
{

  helloButton.listen();  
  externalLED.update();

  if (helloButton.onPress()) {
    //will blink the LED once at current period length and duty cycle
    // externalLED.pulse();
    
    //will blink the LED 5 times at current period length and duty cycle
    externalLED.pulse(5);
   
    //will pulse 5 times and update period lenth and duty settings
    //externalLED.pulse(5, 2000, 10);
    
    //can set period length and duty cycle seperately
    //externalLED.setFullPeriod(2000);
    //externalLED.setDutyCycle(10);
    
    Serial.println(externalLED.getFullPeriod());
    Serial.println(externalLED.getDutyCycle());
  } 

}

