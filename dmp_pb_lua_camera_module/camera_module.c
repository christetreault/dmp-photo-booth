#include "dmp_camera_module.h"

int (*console_write)(char * message);
void (*status_handler)(gint status);

int dmp_cm_install_console(int (*c_cb)(char * message))
{
	g_assert(c_cb != NULL);
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_install_status_handler(void (*sh)(int status))
{
	g_assert(sh != NULL);
	
	status_handler = sh;
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_capture(char * location)
{
	return dmp_cm_lua_capture(location);
}

int dmp_cm_edit_config()
{
	return DMP_PB_SUCCESS;
}

int dmp_cm_load_config()
{
	return DMP_PB_SUCCESS;
}

int dmp_cm_initialize()
{
	return dmp_cm_lua_initialize();
}

int dmp_cm_finalize()
{
	return dmp_cm_lua_finalize();
}

int dmp_cm_console_write(gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}

void dmp_cm_set_status(gboolean status)
{
	g_assert(status_handler != NULL);
	(*status_handler)(status);
}