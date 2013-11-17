#include "configuration.h"
#include "error_handling.h"

static GKeyFile * dmp_pb_config = NULL;

gint dmp_pb_config_load()
{
	GError * error = NULL;
	if (dmp_pb_config == NULL) return dmp_pb_set_error_code_return(DMP_PB_NULL_POINTER);
		
	dmp_pb_config = g_key_file_new();
	if (!g_key_file_load_from_file(dmp_pb_config, DMP_PB_CONFIG_FILE, G_KEY_FILE_NONE, &error))
	{
		
	}
	
	

}