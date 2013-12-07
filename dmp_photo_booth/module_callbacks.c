#include "module_callbacks.h"

void dmp_pb_trigger_handler()
{
	dmp_pb_handle_photo_request();
}

int dmp_pb_console_write_callback(char * message)
{
	dmp_pb_console_queue_push(g_string_new(message));
	return DMP_PB_SUCCESS;
}