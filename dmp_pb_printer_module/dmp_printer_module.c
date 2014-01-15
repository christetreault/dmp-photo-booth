#include "dmp_printer_module.h"

int (*console_write)(const char * message);
void (*status_handler)(gint status);

int dmp_pm_install_status_handler(void (*sh)(int status))
{
	g_assert(sh != NULL);
	
	status_handler = sh;
	
	return DMP_PB_SUCCESS;
}

int dmp_pm_install_console(int (*c_cb)(const char * message))
{
	g_assert(c_cb != NULL);
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_pm_print(const char * to_print)
{
	g_assert(to_print != NULL);
	
	return dmp_pm_process_print(to_print);
}

int dmp_pm_edit_config(void)
{
	dmp_pm_console_write("\n\n---------------------------------------------------------------");
	dmp_pm_console_write("\n\nTo edit the configuration, edit the file \""
							DMP_PM_CONFIG_FILE
							"\" with your favorite text editor.");
	dmp_pm_console_write("\n\n---------------------------------------------------------------");
	dmp_pm_console_write("\n\n");
	return DMP_PB_SUCCESS;
}

int dmp_pm_initialize(void)
{
	dmp_pm_config_init();
	return DMP_PB_SUCCESS;
}

int dmp_pm_finalize(void)
{
	dmp_pm_config_finalize();
	return DMP_PB_SUCCESS;
}

int dmp_pm_console_write(const gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}

void dmp_pm_set_status(gboolean status)
{
	g_assert(status_handler != NULL);
	(*status_handler)(status);
}