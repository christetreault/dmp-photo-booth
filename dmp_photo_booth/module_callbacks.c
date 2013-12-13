#include "module_callbacks.h"


void dmp_pb_trigger_handler()
{
	if (dmp_pb_ui_is_started()) dmp_pb_handle_photo_request();
	else
	{
		dmp_pb_tm_show_error(PHOTO_BOOTH_CORE_ERROR);
	}
}

int dmp_pb_console_write_callback(char * message)
{
	dmp_pb_console_queue_push(g_string_new(message));
	return DMP_PB_SUCCESS;
}