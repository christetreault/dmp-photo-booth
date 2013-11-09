#include "module_callbacks.h"

void dmp_pb_trigger_handler()
{
	g_printf("dmp_pb_trigger_handler() called!\n");
}

int dmp_pb_console_write_callback(char * message)
{
	GString * to_write = g_string_new(message);
	return dmp_pb_console_queue_push(to_write);
}