#include "dmp_trigger_module.h"

void (*trigger_handler)();
int (*console_write)(char * message);

int dmp_tm_install_console(int (*c_cb)(char * message))
{
	if (c_cb == NULL) return DMP_PB_NULL_POINTER;
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_add_trigger_handler(void (*th)())
{
	if (th == NULL) return DMP_PB_NULL_POINTER;
	
	trigger_handler = th;
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_set_countdown(int current)
{
	if (trigger_handler == NULL) return DMP_PB_NULL_POINTER;
	
	printf("Called: dmp_tm_set_countdown(%d)\n", current);
	printf("Calling the trigger handler:\n");
	(*trigger_handler)();
	
	return DMP_PB_SUCCESS;
}

char * dmp_tm_get_config_location(char * to_fill, size_t size)
{
	if (to_fill == NULL) return NULL;
	
	strncpy(to_fill, "./tm.lua", size);
	if (size > 0) to_fill[size - 1] = '\0';
	
	return to_fill;
}

int dmp_tm_edit_config()
{
	printf("Called: dmp_cm_edit_config()\n");
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_load_config()
{
	printf("Called: dmp_cm_load_config()\n");
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_initialize()
{
	return DMP_PB_SUCCESS;
}

int dmp_tm_is_initialized()
{
	return !0;
}

int dmp_tm_finalize()
{
	return DMP_PB_SUCCESS;
}