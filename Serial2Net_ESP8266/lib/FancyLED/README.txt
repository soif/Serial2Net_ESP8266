This is an example C++ library for Arduino 0018+, by Carlyn Maw

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
~/Documents/Arduino/libraries folder for mac My Documents\Arduino\libraries\ for a
PC.

When installed, this library should look like:

Arduino/libraries/FancyLED              (this library's folder)
Arduino/libraries/FancyLED/FancyLED.cpp     (the library implementation file)
Arduino/libraries/FancyLED/FancyLED.h       (the library description file)
Arduino/libraries/FancyLED/keywords.txt (the syntax coloring file)
Arduino/libraries/FancyLED/examples     (the examples in the "open" menu)
Arduino/libraries/FancyLED/readme.txt   (this file)

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select dLED.  This will add a corresponding line to the top of your sketch:
#include <FancyLED.h>

To stop using this library, delete that line from your sketch.

Geeky information:
After a successful build of this library, a new file named "FancyLED.o" will appear
in "Arduino/libraries/FancyLED". This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "FancyLED.cpp" or "FancyLED.h"),
then you must first 'unbuild' this library by deleting the "FancyLED.o" file. The
new "FancyLED.o" with your code will appear after the next press of "verify"

Public Functions
--------------------------------------------------------------------------------

FancyLED myInstance = FancyLED(int myPin, bool myMode);

FancyLED myInstance = FancyLED(int pinNumber, bool pressedState);

	myInstance = some name
	myPin = some int representing what pin the LED is attached to
	myMode = is the pin meant to be HIGH or LOW when to turn the LED on (power source or power sink?)
	
	BEHAVIOR: creates instance of a FancyLED
	
FancyLED myInstance = FancyLED(int pinNumber, bool pressedState, unsigned char *myRegister);

	myInstance = some name
	myBit = some int representing in what bit the LED information is stored
	myMode = is the pin meant to be HIGH or LOW when to turn the LED on (power source or power sink?)
	*myRegister = the byte containing the button information passed by reference ( &byteName ) 
	
	BEHAVIOR: creates instance of a FancyLED
    
myInstance.update();
myInstance.update(unsigned long newCurrentTime);

    newCurrentTime = optional value if you want all the LEDs have same preicisesly the same update
    time. Calling this function without providing a specific time causes it to call millis() itself. 

    BEHAVIOR: Does all the work of the function, actually tracks the behavior. Should be called
    at the top of the main loop.
    
    
myInstance.setCurrentTime(unsigned long newCurrentTime);

    newCurrentTime = forces a refresh of the private current time variable. Shouldn't be necessary.

    BEHAVIOR: returns true if the debounced value for button is unpressed.
    
myInstance.getState();
    
    BEHAVIOR: returns true a value
        0 - off, available to get new messages
        1 - on 
        2 - off, but in a forced resting mode (the off part of a duty cycle, for example)

    
myInstance.turnOn();
    
    BEHAVIOR: Turns it on. (state goes to 1, led turns on)

myInstance.turnOff();
    
    BEHAVIOR: Turns it off. (state goes to 2, led turns off - state will still wait go to 0 until 
    the off period determined my the period length and duty cycle is over)
    
myInstance.toggle();

    BEHAVIOR: If it is on, it goes off. If it is either off or resting it goes on. 
        
myInstance.pulse(char myPulseTimes);
myInstance.pulse(char myPulseTimes, int myPeriod, int myDutyCycle)
    
    myPulseTimes = the number of times you want it to blink
    myPeriod = the length of the full cycle
    myDutyCycle = the percent of that period that the LED will be on (at the start)
    
    BEHAVIOR: LED will blink the number of times asked of it, at the default/current settings unless
    they are also updated. 
    
myInstance.pulse();
    
    BEHAVIOR: This function will pulse the LED forever (if myInstance.update() is called)

myInstance.fusedPulse(long myFuseLength, int myPulseTimes);

    myFuseLength = how long will it wait until it starts the pulse routine
    myPulesTimes = the number of times it will pulse

    BEHAVIOR: pulses the number of time indicated by myPulseTimes, but delays start for the number
    of milliseconds indicated by myFuseLength
    
myInstance.getDutyCycle();

    BEHAVIOR: returns current value of the duty cycle
    
myInstance.setDutyCycle(int newDC);

    newDC = new value for duty cycle (percentage of period, value 0-100)

    BEHAVIOR: sets the value for the duty cycle. default is 50%

myInstance.getFullPeriod();

    BEHAVIOR: returns the value of the full period (how long on + how long off)

myInstance.setFullPeriod(long newFP);

    newFP = value in milliseconds that the full period should be. 
    
    BEHAVIOR: sets the value for how long it takes the LED to complete a on cycle and an off cycle. 
    Default is 1000 (1 second).
