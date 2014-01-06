#include "dmp_camera_module.h"

int (*console_write)(char * message);

int dmp_cm_install_console(int (*c_cb)(char * message))
{
	g_assert(c_cb != NULL);
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_capture(char * location)
{
	
	
	return DMP_PB_SUCCESS;
}

char * dmp_cm_get_config_location(char * to_fill, size_t size)
{
	if (to_fill == NULL) return NULL;
	
	strncpy(to_fill, "./cm.lua", size);
	if (size > 0) to_fill[size - 1] = '\0';
	
	return to_fill;
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
	return DMP_PB_SUCCESS;
}

int dmp_cm_is_initialized()
{
	return !0;
}

int dmp_cm_finalize()
{
	return DMP_PB_SUCCESS;
}

int dmp_cm_console_write(gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}