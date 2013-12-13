#include "switch.h"

static int last_state = LOW;

void dmp::trigger::handle_trigger_button()
{
	int current;
	if ((current = digitalRead(TRIGGER_BUTTON)) == LOW)
	{
		if (last_state == HIGH) write(OUTPUT_BUTTON_PRESS);
	}
	last_state = current;
}