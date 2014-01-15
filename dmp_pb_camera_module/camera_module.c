#include "dmp_camera_module.h"
#include "camera_logic.h"

int (*console_write)(const char * message);
void (*status_handler)(gint status);

int dmp_cm_install_console(int (*c_cb)(const char * message))
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

int dmp_cm_capture(const char * location)
{
	dmp_cm_camera_capture(location);
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_edit_config(void)
{
	return DMP_PB_SUCCESS;
}

int dmp_cm_initialize(void)
{
	return dmp_cm_camera_init();
}

int dmp_cm_finalize(void)
{
	return dmp_cm_camera_finalize();
}

int dmp_cm_console_write(const gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}

void dmp_cm_set_status(gboolean status)
{
	g_assert(status_handler != NULL);
	(*status_handler)(status);
}