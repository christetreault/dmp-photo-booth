#include "dmp_camera_module.h"

int (*console_write)(char * message);

int dmp_cm_install_console(int (*c_cb)(char * message))
{
	if (c_cb == NULL) return DMP_PB_NULL_POINTER;
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_capture()
{
	printf("Called: dmp_cm_capture()\n");
	
	return DMP_PB_SUCCESS;
}

int dmp_cm_download(char * location)
{
	if (location == NULL) return DMP_PB_NULL_POINTER;
	
	printf("Called: dmp_cm_download(\"%s\")\n", location);
	
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