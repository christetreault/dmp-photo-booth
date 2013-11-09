#include "dmp_printer_module.h"

int (*console_write)(char * message);

int dmp_pm_install_console(int (*c_cb)(char * message))
{
	if (c_cb == NULL) return DMP_PB_NULL_POINTER;
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_pm_print(char * to_print)
{
	if (to_print == NULL) return DMP_PB_NULL_POINTER;
	
	printf("Called: dmp_pm_print(\"%s\")\n", to_print);
	
	return DMP_PB_SUCCESS;
}

char * dmp_pm_get_config_location(char * to_fill, size_t size)
{
	if (to_fill == NULL) return NULL;
	
	strncpy(to_fill, "./pm.lua", size);
	if (size > 0) to_fill[size - 1] = '\0';
	
	return to_fill;
}

int dmp_pm_edit_config()
{
	printf("Called: dmp_pm_edit_config()\n");
	
	return DMP_PB_SUCCESS;
}

int dmp_pm_load_config()
{
	printf("Called: dmp_pm_load_config()\n");
	
	return DMP_PB_SUCCESS;
}