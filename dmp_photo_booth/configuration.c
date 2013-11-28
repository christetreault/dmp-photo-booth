#include "configuration.h"

static GKeyFile * dmp_pb_config = NULL;
G_LOCK_DEFINE(dmp_pb_config);

void dmp_pb_config_initialize(GString * path, GError ** error)
{
	GError * working = NULL;
	
	if (dmp_pb_config != NULL) return;
	
	G_LOCK(dmp_pb_config);
	dmp_pb_config = g_key_file_new();
	g_key_file_load_from_file(dmp_pb_config, 
			path->str, 
			G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS,
			&working);
	if (working != NULL)
	{
		g_propagate_error(error, working);
		G_UNLOCK(dmp_pb_config);
		return;
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

void dmp_pb_config_write(GString * path, GError ** error)
{
	g_assert(dmp_pb_config != NULL);
	
	GError * working = NULL;
	
	G_LOCK(dmp_pb_config);
	
	G_UNLOCK(dmp_pb_config);
}

GString * dmp_pb_config_read_string(const gchar * group, const gchar * key)
{
	return g_string_new(g_key_file_get_string(dmp_pb_config, group, key, NULL));
}

void dmp_pb_config_write_string(const gchar * group, const gchar * key, GString * value)
{
	g_key_file_set_string(dmp_pb_config, group, key, value->str);
}