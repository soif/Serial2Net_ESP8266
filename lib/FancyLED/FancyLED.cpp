/*
  FancyLED.h - FancyLED for Wiring/Arduino
  (cc) 2011 Carlyn Maw, Attribute, Share Alike
  
  Created 06 July 2011
  Version 0.1.1
*/


// include core Wiring API
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// include this library's description file
#include "FancyLED.h"

   
  
// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

//------------------------------------------------------ Using Arduino Pin Num
FancyLED::FancyLED(int myPin, bool myMode)
{
    // initialize this instance's variables
    this->_myPin = myPin;
    
    pinMode(this->_myPin, OUTPUT);
    
    this->_type = 0;
    this->_myBit = this->_myPin;
    this->_mode = myMode;
    
    _lastState = 0;
    _currentState = 0;
    _pinState= 0;
    
    _dutyCycle = 50;
    _fullPeriod = 1000;
    _fuseTimer = 0;
    
    _goalPulseCount = 1;
    _currentPulseCount = 0;
      
}

//----------------------------------------------------------------- Using Byte
FancyLED::FancyLED(int myBit, bool myMode, unsigned char *myRegister)
{
    // initialize this instance's variables
    this->_type = 1;
    this->_mode = myMode;
    this->_myBit = myBit;
    this->_myPin = this->_myBit;  
    this->_myRegister = myRegister;
    
    this->_registerValue = 255;
    
    _lastState = 0;
    _currentState = 0;
    _pinState= 0;
    
    _dutyCycle = 50;
    _fullPeriod = 1000;
    _fuseTimer = 0;
    
    _goalPulseCount = 1;
    _currentPulseCount = 0;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

//---------////////////////////MAIN LOOP / LISTENER ///////////--------------//

void FancyLED::update(void) {
           update(millis());
}



void FancyLED::update(unsigned long newCurrentTime) {
      _currentTime = newCurrentTime;
      
      
      if (_fuseTimer > _currentTime) {
        return;
      } 
      
      if (_goalPulseCount == 0 or _currentPulseCount < _goalPulseCount) {
         _pulseFlag = true;
      } else if (_currentPulseCount >= _goalPulseCount) {
      }
      
      
      
      // if (_pulseFlag && _countFlag) {
      if (_pulseFlag) {
        //_longHolder = (_fullPeriod * _dutyCycle) / 100;
        //_onPeriod = int(_longHolder);
         _onPeriod = (_fullPeriod * _dutyCycle) / 100;
        _offPeriod = (_fullPeriod - _onPeriod);
        //Serial.println(_onPeriod);

      //if my state is ready 
      if (_currentState == 0) {
        //get the time
        _flipTime = _currentTime;
        //set my state to firing
        _currentState = 1;
        //turn on the pin
        _pinState = _mode;
        updatePin(_pinState);
      } 
      //if my state is firing
      else if (_currentState == 1) { 
        if ((_onPeriod) < (_currentTime - _flipTime)) {
          //if it's time, turn me off
          _flipTime = _currentTime;
          _currentState = 2;
          _pinState = !_mode;
          updatePin(_pinState);
        }    
      } 
      //if my state is resting
      else if (_currentState == 2) {
        //keep me off
        _pinState = !_mode;
        updatePin(_pinState);
        //check the time and make me ready
        if ((_offPeriod) < (_currentTime - _flipTime)) {
          _currentState = 0;
          _currentPulseCount++;
          _pulseFlag = false;
          _flipTime = _currentTime;
        }
    
      }
    }
    
}


void FancyLED::setCurrentTime(unsigned long newCurrentTime) {
    _currentTime = newCurrentTime;
}

int FancyLED::getState(void){
    return _currentState;
}



void FancyLED::turnOn(void){
    _pinState = _mode;
    updatePin(_pinState);
    
	_lastState = _currentState;
	_currentState = 1; //firing
    _flipTime = _currentTime;

}

void FancyLED::turnOff(void){
    _pinState = !_mode;
    updatePin(_pinState);

	_lastState = _currentState;
	_currentState = 2; //waiting

}

void FancyLED::toggle(void) {
    if (_currentState == 1) {
        turnOff();       
    } else {
        turnOn();
    }
}

void FancyLED::pulse(int myPulseTimes) {

    _pulseFlag = true;
    _goalPulseCount = myPulseTimes;
    _currentPulseCount = 0;
}

void FancyLED::pulse(int myPulseTimes, int myPeriod, int myDutyCycle) {

    _dutyCycle = myDutyCycle;
    _fullPeriod = myPeriod;
    pulse(myPulseTimes);
    
}

void FancyLED::pulse(void) {

    pulse(0);
    
}

void FancyLED::fusedPulse(long myFuseLength, int myPulseTimes){
    _fuseTimer = myFuseLength + _currentTime;
    pulse(myPulseTimes);
}

int FancyLED::getDutyCycle(void){
    return _dutyCycle;
}

void FancyLED::setDutyCycle(int newDC) {
    _dutyCycle = newDC;
}

long FancyLED::getFullPeriod(void){
    return _fullPeriod;
}

void FancyLED::setFullPeriod(long newFP){
    _fullPeriod = newFP;
}



// Private Methods //////////////////////////////////////////////////////////////
// Functions available to the library only.


void FancyLED::updatePin(bool pinValue) {
    
    if(!_type) {
        digitalWrite(_myPin, pinValue);
    } else {
        _registerValue = *_myRegister;
        if (pinValue) {
            _registerValue |= (1 << _myBit);
        } else {
            _registerValue &=~ (1 << _myBit);
        }
      *_myRegister = _registerValue;  
    }

}
