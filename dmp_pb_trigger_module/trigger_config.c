#include "trigger_config.h"

#define DMP_TM_GROUP "Trigger Module"
#define DMP_TM_DEVICE_NAME_KEY "device name"

static GKeyFile * dmp_tm_config = NULL;

void dmp_tm_config_init(void)
{
	GError * working = NULL;
	
	if (dmp_tm_config != NULL) return;
	
	dmp_tm_config = g_key_file_new();
	if (g_file_test(DMP_TM_CONFIG_FILE, G_FILE_TEST_EXISTS))
	{
		g_key_file_load_from_file(dmp_tm_config, 
				DMP_TM_CONFIG_FILE, 
				G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS,
				&working);
		if (working != NULL)
		{
			dmp_tm_lifecycle_push_error(working);
			g_clear_error(&working);
		}
	}
}

void dmp_tm_config_finalize(void)
{
	g_key_file_free(dmp_tm_config);
}

gboolean dmp_tm_config_is_initialized(void)
{
	return (dmp_tm_config != NULL);
}

gchar * dmp_tm_config_get_device_name(void)
{
	static gboolean warned = FALSE;
	g_assert(dmp_tm_config_is_initialized());
	GError * error = NULL;
	GString * error_builder;
	gchar * return_value = g_key_file_get_string(dmp_tm_config, DMP_TM_GROUP, DMP_TM_DEVICE_NAME_KEY, &error);
	
	if (error != NULL)
	{
		g_clear_error(&error);
		return_value = g_strdup("/dev/ttyACM0");
		if (!warned)
		{
			error_builder = g_string_new(NULL);
			g_string_printf(error_builder, 
							"Device Name not set in configuration! Defaulting to \"/dev/ttyACM0\".\n");
			dmp_tm_console_write(error_builder->str);
			warned = TRUE;
			g_string_free(error_builder, TRUE);
		}
	}
	return return_value;
}