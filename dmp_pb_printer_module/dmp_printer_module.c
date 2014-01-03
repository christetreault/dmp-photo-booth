#include "dmp_printer_module.h"

int (*console_write)(char * message);

int dmp_pm_install_console(int (*c_cb)(char * message))
{
	g_assert(c_cb != NULL);
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_pm_print(char * to_print)
{
	g_assert(to_print != NULL);
	
	dmp_pm_process_print(to_print);
	
	return DMP_PB_SUCCESS;
}

char * dmp_pm_get_config_location(char * to_fill, size_t size)
{
	if (to_fill == NULL) return NULL;
	
	strncpy(to_fill, "dmp_printer_module.rc", size);
	if (size > 0) to_fill[size - 1] = '\0';
	
	return to_fill;
}

int dmp_pm_edit_config()
{
	dmp_pm_console_write("\n\n---------------------------------------------------------------");
	dmp_pm_console_write("\n\nTo edit the configuration, edit the file \""
							DMP_PM_CONFIG_FILE
							"\" with your favorite text editor.");
	dmp_pm_console_write("\n\n---------------------------------------------------------------");
	dmp_pm_console_write("\n\n");
	return DMP_PB_SUCCESS;
}

int dmp_pm_load_config()
{
	return DMP_PB_SUCCESS;
}


int dmp_pm_initialize()
{
	dmp_pm_config_init();
	return DMP_PB_SUCCESS;
}

int dmp_pm_is_initialized()
{
	return dmp_pm_config_is_initialized();
}

int dmp_pm_finalize()
{
	dmp_pm_config_finalize();
	return DMP_PB_SUCCESS;
}

int dmp_pm_console_write(gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}