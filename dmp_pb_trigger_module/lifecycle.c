#include "lifecycle.h"

static gboolean is_initialized = FALSE;

void dmp_tm_lifecycle_push_error(GError * error)
{
	g_assert(error != NULL);
	
	GString * message = g_string_new(NULL);
	g_string_printf(message, "%s %d: %s\n", g_quark_to_string(error->domain),
				error->code, error->message);
	dmp_tm_console_write(message->str);
	g_string_free(message, TRUE);
}

gint dmp_tm_lifecycle_initialize()
{
	if (is_initialized) return DMP_PB_SUCCESS;
	
	dmp_tm_io_start_serial();
	
	is_initialized = TRUE;
	return DMP_PB_SUCCESS;
}

gint dmp_tm_lifecycle_finalize()
{
	dmp_tm_io_stop_serial();
	
	is_initialized = FALSE;
	return DMP_PB_SUCCESS;
}