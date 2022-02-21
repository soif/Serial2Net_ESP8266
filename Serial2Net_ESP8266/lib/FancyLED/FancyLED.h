/*
	FancyLED.h - - FancyLED library for Wiring/Arduino - Version 0.1.1
	
	Original library 		(0.1) by Carlyn Maw.
	
 */

// ensure this library description is only included once
#ifndef FancyLED_h
#define FancyLED_h

// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// library interface description
class FancyLED {
 
  // user-accessible "public" interface
  public:
  // constructors:
    FancyLED(int myPin, bool myMode);
    FancyLED(int myBit, bool myMode, unsigned char *myRegister);
    
    //char* version(void);			// get the library version
    //unsigned char getRegisterValue(void);

    void setCurrentTime(unsigned long);
    void update(unsigned long);
    void update(void);
    
    int getState(void);
    
    void turnOn(void);	
    void turnOff(void);
    void toggle(void);
 
    void pulse(int); 
    void pulse(int, int, int);
	void pulse(void);	

    void fusedPulse(long myFuseLength, int myPulseTimes);
	
	int getDutyCycle(void);
	void setDutyCycle(int);
	
	long getFullPeriod(void);
	void setFullPeriod(long);

    


  // library-accessible "private" interface
  private:
    int _myPin;
    int _myBit;
    unsigned char *_myRegister;
    unsigned char _registerValue;
    bool _type;  //direct pin or shift register
    bool _mode;  //HIGH == pressed (1) or LOW == pressed (0)
    
    int _lastState;
    int _currentState;
    bool _pinState;
    
    long _fuseTimer;
    
    int _onPeriod;
    int _offPeriod;
    int _dutyCycle;
    long _fullPeriod;
   //long _longHolder;
        		
    bool _pulseFlag;    		

    unsigned long int _flipTime;	
	unsigned long int _currentTime;
	
	int _currentPulseCount;
	int _goalPulseCount;

	
	void updatePin(bool);
  
};

#endif

