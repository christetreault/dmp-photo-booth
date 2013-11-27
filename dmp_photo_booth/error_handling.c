#include "error_handling.h"

GString * dmp_pb_error_to_string(GError * error)
{
	g_assert(error != NULL);
	
	GString * message = g_string_new(NULL);
	g_string_printf(message, "%s %d: %s\n", g_quark_to_string(error->domain),
				error->code, error->message);
	return message;
}