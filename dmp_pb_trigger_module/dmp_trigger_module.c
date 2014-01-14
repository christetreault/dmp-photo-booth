#include "dmp_trigger_module.h"


void (*trigger_handler)() = NULL;
int (*console_write)(char * message);
void (*status_handler)(gint status);

int dmp_tm_install_status_handler(void (*sh)(int status))
{
	g_assert(sh != NULL);
	
	status_handler = sh;
	
	return DMP_PB_SUCCESS;
}

int dmp_tm_show_error(int value)
{
	dmp_tm_io_write_byte(value | IS_ERROR);
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

int dmp_tm_edit_config()
{
	dmp_tm_console_write("\n\n---------------------------------------------------------------");
	dmp_tm_console_write("\n\nTo edit the configuration, edit the file \""
							DMP_TM_CONFIG_FILE
							"\" with your favorite text editor.");
	dmp_tm_console_write("\n\n---------------------------------------------------------------");
	dmp_tm_console_write("\n\n");
	return DMP_PB_SUCCESS;
}

int dmp_tm_initialize()
{
	dmp_tm_config_init();
	return dmp_tm_lifecycle_initialize();
}

int dmp_tm_finalize()
{
	dmp_tm_config_finalize();
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

void dmp_tm_set_status(gboolean status)
{
	g_assert(status_handler != NULL);
	(*status_handler)(status);
}