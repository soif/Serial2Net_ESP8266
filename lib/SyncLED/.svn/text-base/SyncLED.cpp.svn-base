#include "SyncLED.h"

SyncLED::SyncLED(byte pin, byte initialState, boolean usePWM, unsigned long pwmMS):m_pin(pin),m_state(initialState),
	m_inPattern(false),m_lastChange(0L), m_usePWM(usePWM), m_pwmFadeMS(pwmMS)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, initialState);
	m_state = initialState;
}

void SyncLED::blinkPattern(unsigned long pattern, unsigned long delay, byte pattern_length)
{
	setRate(delay);
	setPattern(pattern, pattern_length);
}

void SyncLED::setRate(unsigned long delay)
{
	m_delay = delay;
}

void SyncLED::setPattern(unsigned long pattern, byte pattern_length)
{
	m_pattern = pattern;
	m_patternLength = pattern_length;
	m_inPattern = true;
	m_patternPosition = 0;
}

//
// Blink the LED x times
// with a delay between blinks and a 4x off period between
// series of blinks
//
void SyncLED::blinkPattern(uint8_t blinks, unsigned long delay)
{
	m_pattern = 0;
	for(int i=0; i < blinks;i++) {
		m_pattern |= (0b10000 << (i*2));
	}
	m_patternLength = (blinks*2) + 4;
	m_delay = delay;
	m_inPattern = true;
	m_patternPosition = 3;
}


void SyncLED::On()
{
	m_state = HIGH;
	digitalWrite(m_pin, m_state);
	m_inPattern = false;
	m_lastChange = millis();
}

void SyncLED::Off()
{
	m_state = LOW;
	digitalWrite(m_pin, m_state);
	m_inPattern = false;
	m_lastChange = millis();
}

void SyncLED::resumePattern(boolean reset)
{
	if(reset) m_patternPosition = 0;
	m_inPattern = true;
}

boolean SyncLED::isOn()
{
	return( m_state == HIGH );
}

void SyncLED::update()
{
	update(millis());
}

void SyncLED::advanceState(unsigned long time_ms)
{
		m_patternPosition++;
		m_patternPosition %= m_patternLength;
		m_lastState = m_state;
		m_state = bitRead(m_pattern, m_patternPosition);
		m_lastChange = time_ms;
}

void SyncLED::update(unsigned long time_ms)
{
  if(!m_inPattern) return;
  
  unsigned long surpassed = time_ms - m_lastChange;
  
  if(!m_usePWM) { // Simple state changes...
	if(surpassed >= m_delay) {
		// Time to change LED state
		// Get the next bit out of the pattern
		advanceState(time_ms);
		
		// m_state ^= 1UL;
		digitalWrite( m_pin, m_state );
	}
  } else { // Use PWM
  	if(surpassed >= m_delay) {
  		advanceState(time_ms);
  	}
  	
    // Progressive change
    // Map our current value and write it out - no
    if(m_lastState != m_state) 
    	analogWrite(m_pin, constrain( map( surpassed, 0, m_pwmFadeMS, (m_state?0:255), (m_state?255:0)), 0, 255));
  }
  
}


