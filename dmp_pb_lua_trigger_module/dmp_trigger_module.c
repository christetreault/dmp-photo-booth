#include "dmp_trigger_module.h"
#include "lua_interface.h"


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
	return dmp_tm_lua_show_error(value | (1 << 7));
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
	return dmp_tm_lua_set_countdown(current);
}

int dmp_tm_edit_config()
{
	return DMP_PB_SUCCESS;
}

int dmp_tm_initialize()
{
	return dmp_tm_lua_initialize();
}

int dmp_tm_finalize()
{
	return dmp_tm_lua_finalize();
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