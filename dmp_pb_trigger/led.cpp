#include "led.h"


void dmp::trigger::show_value(led_pin value)
{
	switch (value)
	{
		case COUNT_NULL_LED:
			break;
		case COUNT_0_LED:
			digitalWrite(COUNT_0_LED, HIGH);
			digitalWrite(COUNT_1_LED, LOW);
			digitalWrite(COUNT_2_LED, LOW);
			digitalWrite(COUNT_3_LED, LOW);
			digitalWrite(COUNT_4_LED, LOW);
			break;
		case COUNT_1_LED:
			digitalWrite(COUNT_0_LED, HIGH);
			digitalWrite(COUNT_1_LED, HIGH);
			digitalWrite(COUNT_2_LED, LOW);
			digitalWrite(COUNT_3_LED, LOW);
			digitalWrite(COUNT_4_LED, LOW);
			break;
		case COUNT_2_LED:
			digitalWrite(COUNT_0_LED, HIGH);
			digitalWrite(COUNT_1_LED, HIGH);
			digitalWrite(COUNT_2_LED, HIGH);
			digitalWrite(COUNT_3_LED, LOW);
			digitalWrite(COUNT_4_LED, LOW);
			break;
		case COUNT_3_LED:
			digitalWrite(COUNT_0_LED, HIGH);
			digitalWrite(COUNT_1_LED, HIGH);
			digitalWrite(COUNT_2_LED, HIGH);
			digitalWrite(COUNT_3_LED, HIGH);
			digitalWrite(COUNT_4_LED, LOW);
			break;
		case COUNT_4_LED:
			digitalWrite(COUNT_0_LED, HIGH);
			digitalWrite(COUNT_1_LED, HIGH);
			digitalWrite(COUNT_2_LED, HIGH);
			digitalWrite(COUNT_3_LED, HIGH);
			digitalWrite(COUNT_4_LED, HIGH);
			break;
	}
	delay(1000);
}

void dmp::trigger::clear()
{
	digitalWrite(COUNT_0_LED, LOW);
	digitalWrite(COUNT_1_LED, LOW);
	digitalWrite(COUNT_2_LED, LOW);
	digitalWrite(COUNT_3_LED, LOW);
	digitalWrite(COUNT_4_LED, LOW);
}

void dmp::trigger::show_error(unsigned code)
{
	digitalWrite(COUNT_0_LED, LOW);
	digitalWrite(COUNT_1_LED, LOW);
	digitalWrite(COUNT_2_LED, LOW);
	digitalWrite(COUNT_3_LED, LOW);
	digitalWrite(COUNT_4_LED, LOW);
	
	delay(250);
	
	if (code & 1 << 0) digitalWrite(COUNT_0_LED, HIGH);
	if (code & 1 << 1) digitalWrite(COUNT_1_LED, HIGH);
	if (code & 1 << 2) digitalWrite(COUNT_2_LED, HIGH);
	if (code & 1 << 3) digitalWrite(COUNT_3_LED, HIGH);
	if (code & 1 << 4) digitalWrite(COUNT_4_LED, HIGH);
	
	delay(250);
	
	digitalWrite(COUNT_0_LED, LOW);
	digitalWrite(COUNT_1_LED, LOW);
	digitalWrite(COUNT_2_LED, LOW);
	digitalWrite(COUNT_3_LED, LOW);
	digitalWrite(COUNT_4_LED, LOW);
	
	delay (250);
	
	if (code & 1 << 0) digitalWrite(COUNT_0_LED, HIGH);
	if (code & 1 << 1) digitalWrite(COUNT_1_LED, HIGH);
	if (code & 1 << 2) digitalWrite(COUNT_2_LED, HIGH);
	if (code & 1 << 3) digitalWrite(COUNT_3_LED, HIGH);
	if (code & 1 << 4) digitalWrite(COUNT_4_LED, HIGH);
	
	delay(250);
	
	clear();
}