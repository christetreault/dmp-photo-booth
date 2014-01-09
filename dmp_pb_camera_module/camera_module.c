#include "dmp_camera_module.h"
#include "camera_logic.h"

int (*console_write)(char * message);

int dmp_cm_install_console(int (*c_cb)(char * message))
{
	g_assert(c_cb != NULL);
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_capture(char * location)
{
	//printf("Called: dmp_cm_capture(%s)\n", location);
	
	dmp_cm_camera_capture(location);
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_edit_config()
{
	printf("Called: dmp_cm_edit_config()\n");
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_load_config()
{
	printf("Called: dmp_cm_load_config()\n");
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_initialize()
{
	return dmp_cm_camera_init();
}

int dmp_cm_is_initialized()
{
	return dmp_cm_camera_is_initialized();
}

int dmp_cm_finalize()
{
	return dmp_cm_camera_finalize();
}

int dmp_cm_console_write(gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}