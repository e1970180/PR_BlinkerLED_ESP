#pragma once

#include <arduino.h>
#include <stdint.h>             //used for Limits of integer types

#ifdef ARDUINO_ARCH_ESP8266

	#include "Ticker.h"				// the new version MUST be used from GitHub https://github.com/esp8266/Arduino/tree/master/libraries/Ticker


	class BlinkingLed {
		public:
			BlinkingLed(const uint8_t pin, const bool onState);
			~BlinkingLed();
			
			void	blinkStart_T(const uint32_t onTimeMS, const uint32_t offTimeMS);
			void	blinkStart_F(const float freq, const float duty);
			void	blinkStop();											//switch to continuous mode
			
			void	on();
			void	off();
			void	toggle();
			void	set(const bool value);
			
			bool	getVal();
			bool	getBlink();

		protected:
			Ticker			_ticker;
		
			uint8_t			_pin;
			bool			_offValue;		// the digitalWrite()'s value to off the led. 
			
			bool			_valueOut;		// flag if led on/off
			bool			_valueBlnk;		// flag if blink mode is on/off
			
			volatile bool	_currentBlinkState;	
					
			uint32_t	_onTimeMS;			// [ms] period "on" in blinking mode
			uint32_t 	_offTimeMS;
		
		protected:	
			void		_write();
			void		_callback();
	};

	BlinkingLed::BlinkingLed(const uint8_t pin, const bool onValue) : _pin(pin), _offValue(!onValue),
																	  _valueOut(false), _valueBlnk(false),
																	  _currentBlinkState(true)				{
		pinMode(_pin, OUTPUT);
		set(false);
	}

	BlinkingLed::~BlinkingLed() {
		pinMode(_pin, INPUT);
	}

	void	BlinkingLed::blinkStart_T(const uint32_t onTimeMS, const uint32_t offTimeMS) {
		_onTimeMS  = (onTimeMS) ? onTimeMS : 1; //if (onTimeMS==0) then _onTimeMS = 1;
		_offTimeMS = (offTimeMS) ? offTimeMS : 1;
		
		_valueBlnk = true;	
		_currentBlinkState = true;	//blinking will start from "on" phase
		
		_callback();
	}

	void 	BlinkingLed::blinkStart_F(const float freq, const float dutyPercentage) {
		
		//onTimeMS  = (uint32_t)( 1000/freq * (dutyPercentage/100%) );
		//offTimeMS = (uint32_t)( 1000/freq * (100% - dutyPercentage)/100% );
		
		float duty = constrain(dutyPercentage, 0, 100);

		float onTimeMS =  10.0/freq * duty;	
		float offTimeMS = 10.0/freq * (100 - duty);
		
		onTimeMS =  constrain(onTimeMS, 1, UINT32_MAX);
		offTimeMS = constrain(offTimeMS, 1, UINT32_MAX);
		
		blinkStart_T( (uint32_t)onTimeMS, (uint32_t)offTimeMS );
	}
		
	void	BlinkingLed::blinkStop() {
		_ticker.detach();
		_valueBlnk = false;
		set(_valueOut);
	}

	void	BlinkingLed::on() 		{ set(true);	}
	void	BlinkingLed::off()  	{ set(false);	}
	
	void	BlinkingLed::toggle() {
		_valueOut = !_valueOut;
		set(_valueOut);
	}

	void	BlinkingLed::set(const bool value) {
		_valueOut = value;
		_write();
	}

	bool	BlinkingLed::getVal() 		{ return _valueOut;  }
	bool	BlinkingLed::getBlink() 	{ return _valueBlnk; }

//// protected functions
	
	void	BlinkingLed::_write() {
		digitalWrite(_pin, (_offValue ^ (_valueOut && !(!_currentBlinkState && _valueBlnk)) ) );
	}

	void	BlinkingLed::_callback() {
		
		_ticker.once_ms( (_currentBlinkState)?_onTimeMS:_offTimeMS, std::bind(&BlinkingLed::_callback, this) );	
		
		_write(); 
		_currentBlinkState = !_currentBlinkState;
	}
	
#else
	#error "Wrong target platform. This lib for ESP8266 only! " 
#endif