#include "coordination.h"

gchar * dmp_pb_coordination_get_epoch_filename(const gchar * prefix, const gchar * extension)
{
	GString * working = g_string_new(NULL);
	
	#ifdef G_GINT64_MODIFIER
	g_string_printf(working, "%s_%" G_GINT64_MODIFIER "d.%s", prefix, g_get_real_time(), extension);
	#endif
	#ifndef G_GINT64_MODIFIER
	guint val = ABS(g_random_int());
	g_string_printf(working, "%s_%u.%s", prefix, val, extension);
	#endif

	return g_string_free(working, FALSE);
}