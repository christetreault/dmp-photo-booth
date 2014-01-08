#include "error_handling.h"

gint dmp_cm_error_console_write(GError * error)
{
	g_assert(error != NULL);
	
	GString * message = g_string_new(NULL);
	g_string_printf(message, "%s %d: %s\n", g_quark_to_string(error->domain),
				error->code, error->message);
	dmp_cm_console_write(message->str);
	g_string_free(message, TRUE);
	return DMP_PB_SUCCESS;
}