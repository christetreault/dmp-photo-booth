#include "switch.h"

void dmp::trigger::handle_trigger_button()
{
	if (digitalRead(TRIGGER_BUTTON) == HIGH)
	{
		write(OUTPUT_BUTTON_PRESS);
	}
}