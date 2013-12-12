#include "dmp_trigger_module.h"
#include "lifecycle.h"

void (*trigger_handler)() = NULL;
int (*console_write)(char * message);

int dmp_tm_show_error(int value)
{
	printf("Called: dmp_cm_edit_config(%d)\n", value);
	return DMP_PB_SUCCESS;
}

int dmp_tm_install_console(int (*c_cb)(char * message))
{
	if (c_cb == NULL) return DMP_PB_FAILURE;
	
	console_write = c_cb;
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_add_trigger_handler(void (*th)())
{
	if (th == NULL) return DMP_PB_FAILURE;
	
	trigger_handler = th;
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_set_countdown(int current)
{
	if (trigger_handler == NULL) return DMP_PB_FAILURE;
	
	switch (current)
	{
		case 4:
			dmp_tm_io_write_byte(INPUT_COUNT_4);
			break;
		case 3:
			dmp_tm_io_write_byte(INPUT_COUNT_3);
			break;
		case 2:
			dmp_tm_io_write_byte(INPUT_COUNT_2);
			break;
		case 1:
			dmp_tm_io_write_byte(INPUT_COUNT_1);
			break;
		case 0:
			dmp_tm_io_write_byte(INPUT_COUNT_0);
			break;
		default:
			dmp_tm_io_write_byte(INPUT_COUNT_4);
			break;
	}
	
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
	return dmp_tm_lifecycle_initialize();
}

int dmp_tm_is_initialized()
{
	return dmp_tm_lifecycle_is_initialized();
}

int dmp_tm_finalize()
{
	return dmp_tm_lifecycle_finalize();
}

int dmp_tm_console_write(gchar * to_write)
{
	g_assert(console_write != NULL);
	return (*console_write)(to_write);
}

void dmp_tm_call_trigger_handler()
{
	g_assert(trigger_handler != NULL);
	(*trigger_handler)();
}