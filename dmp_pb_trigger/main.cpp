#include <Arduino.h>
#include "serial.h"
#include "led.h"
#include "switch.h"

extern HardwareSerial Serial;

void setup() {
  Serial.begin(9600);
  pinMode(dmp::trigger::COUNT_0_LED, OUTPUT);
  pinMode(dmp::trigger::COUNT_1_LED, OUTPUT);
  pinMode(dmp::trigger::COUNT_2_LED, OUTPUT);
  pinMode(dmp::trigger::COUNT_3_LED, OUTPUT);
  pinMode(dmp::trigger::COUNT_4_LED, OUTPUT);
  pinMode(dmp::trigger::TRIGGER_BUTTON, INPUT);
}

void loop() {
	dmp::trigger::handle_trigger_button();
	int input = dmp::trigger::read();
	dmp::trigger::clear();
	
	if (input == -1) return;
	
	if (dmp::trigger::is_error(input))
	{
		dmp::trigger::show_error(dmp::trigger::resolve_error(input));
	}
	else
	{
		dmp::trigger::show_value(dmp::trigger::resolve_pin(input));
	}
	
		
}