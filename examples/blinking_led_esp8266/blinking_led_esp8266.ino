
#include "PR_BlinkerLED_ESP.hpp"		//ver 0.3.0

BlinkingLed		led(LED_BUILTIN, HIGH);          
            

void setup() {

    Serial.begin(9600);

	Serial.println("start delay 5s");
	delay(5000);

	Serial.println("continuous on 5s");
    led.on();
	delay(5000);
	
	Serial.println("continuous off 3s");
	led.off();
	delay(3000);
	
	Serial.println("blink mode f=1Hz, duty= 50%, still off");
	led.blinkStart_F(1 , 50);
	
	Serial.println("on 15s");
	delay(15000);
	
	Serial.println("off by toggle() 5s");
	led.toggle();
	delay(5000);
	
	Serial.println("on by set() 10s");
	led.set(true);
	delay(10000);
	
	Serial.println("blink mode f=10Hz, duty= 50%, still on");
	led.blinkStart_F(10 , 50);
	
	Serial.println("blink mode 300ms+100ms, still on");
	led.blinkStart_T(300, 100);
	
	Serial.println("continuous mode on 5s");
	led.blinkStop();
	delay(5000);
	
	Serial.println("blink mode f=0.5Hz, duty= 10%, on");
	led.blinkStart_F(0.5 , 10);
    led.on();
	
	Serial.println("finish still blink on");

}


void loop() {
		
  	
}
