#include "serial.h"

int dmp::trigger::read()
{
	return Serial.read();
}

void dmp::trigger::write(char to_write)
{
	Serial.write(to_write);
}

unsigned dmp::trigger::resolve_error(int byte)
{
	return (unsigned) (byte ^ ((1 << 5) | (1 << 6) | (1 << 7)));
}

dmp::trigger::led_pin dmp::trigger::resolve_pin(int byte)
{
	if (byte & INPUT_COUNT_4) return COUNT_4_LED;
	else if (byte & INPUT_COUNT_3) return COUNT_3_LED;
	else if (byte & INPUT_COUNT_2) return COUNT_2_LED;
	else if (byte & INPUT_COUNT_1) return COUNT_1_LED;
	else if (byte & INPUT_COUNT_0) return COUNT_0_LED;
	else return COUNT_NULL_LED;
}

bool dmp::trigger::is_error(int byte)
{
	Serial.print(byte & IS_ERROR);
	return byte & IS_ERROR;
}