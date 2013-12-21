#include "configuration.h"

static GKeyFile * dmp_pb_config = NULL;
G_LOCK_DEFINE(dmp_pb_config);

void dmp_pb_config_initialize(GError ** error)
{
	GError * working = NULL;
	
	if (dmp_pb_config != NULL) return;
	
	G_LOCK(dmp_pb_config);
	dmp_pb_config = g_key_file_new();
	if (g_file_test(DMP_PB_DEFAULT_CONFIGURATION, G_FILE_TEST_EXISTS))
	{
		g_key_file_load_from_file(dmp_pb_config, 
				DMP_PB_DEFAULT_CONFIGURATION, 
				G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS,
				&working);
		if (working != NULL)
		{
			g_propagate_error(error, working);
			G_UNLOCK(dmp_pb_config);
			return;
		}
	}
	G_UNLOCK(dmp_pb_config);
}

void dmp_pb_config_finalize()
{
	if (dmp_pb_config == NULL) return;
	
	G_LOCK(dmp_pb_config);
	g_key_file_free(dmp_pb_config);
	G_UNLOCK(dmp_pb_config);
}

void dmp_pb_config_write(GError ** error)
{
	g_assert(dmp_pb_config != NULL);
	
	GError * working = NULL;
	
	G_LOCK(dmp_pb_config);
	
	g_file_set_contents
			(
				DMP_PB_DEFAULT_CONFIGURATION, 
				g_key_file_to_data(dmp_pb_config, NULL, NULL),
				-1,
				&working
			); //TODO: Am I evil? "...on Windows there is a race condition..."
	
	G_UNLOCK(dmp_pb_config);
	
	if (working != NULL) g_propagate_error(error, working);
	
	return;
}

GString * dmp_pb_config_read_string(const gchar * group, const gchar * key)
{
	G_LOCK(dmp_pb_config);
	gchar * working = g_key_file_get_string(dmp_pb_config, group, key, NULL);
	G_UNLOCK(dmp_pb_config);
	GString * return_value = g_string_new(working);
	g_free(working);
	return return_value;
}

void dmp_pb_config_write_string(const gchar * group, const gchar * key, GString * value)
{
	G_LOCK(dmp_pb_config);
	g_key_file_set_string(dmp_pb_config, group, key, value->str);
	G_UNLOCK(dmp_pb_config);
}

gint dmp_pb_config_read_int(const gchar * group, const gchar * key)
{
	G_LOCK(dmp_pb_config);
	gint return_value = g_key_file_get_integer(dmp_pb_config, group, key, NULL);
	G_UNLOCK(dmp_pb_config);
	return return_value;
}

void dmp_pb_config_write_int(const gchar * group, const gchar * key, gint value)
{
	G_LOCK(dmp_pb_config);
	g_key_file_set_integer(dmp_pb_config, group, key, value);
	G_UNLOCK(dmp_pb_config);
}

gdouble dmp_pb_config_read_double(const gchar * group, const gchar * key)
{
	G_LOCK(dmp_pb_config);
	gdouble return_value = g_key_file_get_double(dmp_pb_config, group, key, NULL);
	G_UNLOCK(dmp_pb_config);
	return return_value;
}

void dmp_pb_config_write_double(const gchar * group, const gchar * key, gdouble value)
{
	G_LOCK(dmp_pb_config);
	g_key_file_set_double(dmp_pb_config, group, key, value);
	G_UNLOCK(dmp_pb_config);
}