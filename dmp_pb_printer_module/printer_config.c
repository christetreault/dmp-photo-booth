#include "printer_config.h"

#define DMP_PM_GROUP "Printer Module"
#define DMP_PM_DPI_KEY "dpi"
#define DMP_PM_WIDTH_KEY "width"
#define DMP_PM_HEIGHT_KEY "height"
#define DMP_PM_PRINTER_NAME_KEY "printer name"
#define DMP_PM_PRINT_TO_FILE_KEY "print to file"

static GKeyFile * dmp_pm_config = NULL;

void dmp_pm_config_init()
{
	GError * working = NULL;
	GString * error_string;
	
	if (dmp_pm_config != NULL) return;
	
	dmp_pm_config = g_key_file_new();
	if (g_file_test(DMP_PM_CONFIG_FILE, G_FILE_TEST_EXISTS))
	{
		g_key_file_load_from_file(dmp_pm_config, 
				DMP_PM_CONFIG_FILE, 
				G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS,
				&working);
		if (working != NULL)
		{
			error_string = g_string_new(NULL);
			g_string_printf(error_string, "%s %d: %s\n", g_quark_to_string(working->domain),
				working->code, working->message);
			dmp_pm_console_write(error_string->str);
			g_string_free(error_string, TRUE);
			g_clear_error(&working);
		}
	}
}

void dmp_pm_config_finalize()
{
	g_key_file_free(dmp_pm_config);
}

gboolean dmp_pm_config_is_initialized()
{
	return (dmp_pm_config != NULL);
}

gsize dmp_pm_config_get_dpi()
{
	static gboolean warned = FALSE;
	g_assert(dmp_pm_config_is_initialized());
	GError * error = NULL;
	gsize return_value = g_key_file_get_integer(dmp_pm_config, DMP_PM_GROUP, DMP_PM_DPI_KEY, &error);
	
	if (error != NULL)
	{
		g_clear_error(&error);
		if (!warned)
		{
			dmp_pm_console_write("DPI not set in configuration! Defaulting to 72.\n");
			warned = TRUE;
		}
		return 72;
	}
	
	return return_value;
}

gsize dmp_pm_config_get_canvas_height()
{
	static gboolean warned = FALSE;
	g_assert(dmp_pm_config_is_initialized());
	GError * error = NULL;
	gsize return_value = g_key_file_get_integer(dmp_pm_config, DMP_PM_GROUP, DMP_PM_HEIGHT_KEY, &error);
	
	if (error != NULL)
	{
		g_clear_error(&error);
		if (!warned)
		{
			dmp_pm_console_write("Canvas Height not set in configuration! Defaulting to 6 Inches.\n");
			warned = TRUE;
		}
		return 6;
	}
	
	return return_value;
}

gsize dmp_pm_config_get_canvas_width()
{
	static gboolean warned = FALSE;
	g_assert(dmp_pm_config_is_initialized());
	GError * error = NULL;
	gsize return_value = g_key_file_get_integer(dmp_pm_config, DMP_PM_GROUP, DMP_PM_WIDTH_KEY, &error);
	
	if (error != NULL)
	{
		g_clear_error(&error);
		if (!warned)
		{
			dmp_pm_console_write("Canvas Width not set in configuration! Defaulting to 6 Inches.\n");
			warned = TRUE;
		}
		return 6;
	}
	
	return return_value;
}

gchar * dmp_pm_config_get_printer_name()
{
	static gboolean warned = FALSE;
	g_assert(dmp_pm_config_is_initialized());
	GError * error = NULL;
	GString * error_builder;
	gchar * return_value = g_key_file_get_string(dmp_pm_config, DMP_PM_GROUP, DMP_PM_PRINTER_NAME_KEY, &error);
	
	if (error != NULL)
	{
		g_clear_error(&error);
		return_value = g_strdup(cupsGetDefault());
		if (!warned)
		{
			error_builder = g_string_new(NULL);
			g_string_printf(error_builder, 
							"Printer Name not set in configuration! Defaulting to \"%s\".\n",
							return_value);
			dmp_pm_console_write(error_builder->str);
			warned = TRUE;
			g_string_free(error_builder, TRUE);
		}
		return return_value;
	}
	
	return return_value;
}

gboolean dmp_pm_config_print_to_file()
{
	g_assert(dmp_pm_config_is_initialized());
	GError * error = NULL;
	gboolean return_value = g_key_file_get_boolean(dmp_pm_config, DMP_PM_GROUP, DMP_PM_PRINT_TO_FILE_KEY, &error);
	
	if (error != NULL)
	{
		g_clear_error(&error);
		return FALSE;
	}
	
	return return_value;
}