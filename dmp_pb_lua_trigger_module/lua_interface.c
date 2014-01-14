#include "lua_interface.h"
#include "dmp_trigger_module.h"

#define DMP_TM_MODULE_SCRIPT "dmp_trigger_module.lua"
#define DMP_TM_NAMESPACE "dmp"
#define DMP_TM_MODULE "tm"

static lua_State * dmp_tm_state = NULL;

static gint dmp_tm_lua_trigger_callback(lua_State * L)
{
	dmp_tm_call_trigger_handler();
	return 0;
}

static gint dmp_tm_lua_console_write(lua_State * L)
{
	if (lua_isstring(L, -1))
	{
		const gchar * message = lua_tostring(L, -1);
		dmp_tm_console_write((gchar *) message);
	}
	
	return 0;
}

static gint dmp_tm_lua_set_status(lua_State * L)
{
	if (lua_isboolean(L, -1))
	{
		gboolean return_value = lua_toboolean(L, -1);
		dmp_tm_set_status(return_value);
	}
	
	return 0;
}

gint dmp_tm_lua_set_countdown(gint count)
{	
	lua_getglobal(dmp_tm_state, DMP_TM_NAMESPACE);
	lua_getfield(dmp_tm_state, -1, DMP_TM_MODULE);
	lua_getfield(dmp_tm_state, -1, "set_countdown");
	lua_pushnumber(dmp_tm_state, count);
	if (lua_pcall(dmp_tm_state, 1, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_tm_console_write((gchar *) error);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

gint dmp_tm_lua_set_countdown(gint count)
{	
	lua_getglobal(dmp_tm_state, DMP_TM_NAMESPACE);
	lua_getfield(dmp_tm_state, -1, DMP_TM_MODULE);
	lua_getfield(dmp_tm_state, -1, "set_countdown");
	lua_pushnumber(dmp_tm_state, count);
	if (lua_pcall(dmp_tm_state, 1, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_tm_console_write((gchar *) error);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

gint dmp_tm_lua_show_error(gint value)
{	
	lua_getglobal(dmp_tm_state, DMP_TM_NAMESPACE);
	lua_getfield(dmp_tm_state, -1, DMP_TM_MODULE);
	lua_getfield(dmp_tm_state, -1, "show_error");
	lua_pushnumber(dmp_tm_state, value);
	if (lua_pcall(dmp_tm_state, 1, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_tm_console_write((gchar *) error);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

/**
 * Registers the trigger_callback callback to dmp.tm.console_write
 * @param L the state to register into
 */
static void dmp_tm_lua_register_trigger_callback(lua_State * L)
{
	lua_getglobal(L, DMP_TM_NAMESPACE);
	lua_getfield(L, -1, DMP_TM_MODULE);
	lua_pushcfunction(L, dmp_tm_lua_trigger_callback);
	lua_setfield(L, -2, "trigger_callback");
}

/**
 * Registers the console_write callback to dmp.tm.console_write
 * @param L the state to register into
 */
static void dmp_tm_lua_register_console(lua_State * L)
{
	lua_getglobal(L, DMP_TM_NAMESPACE);
	lua_getfield(L, -1, DMP_TM_MODULE);
	lua_pushcfunction(L, dmp_tm_lua_console_write);
	lua_setfield(L, -2, "console_write");
}

/**
 * Registers the set_status callback to dmp.tm.set_status
 * @param L the state to register into
 */
static void dmp_tm_lua_register_set_status(lua_State * L)
{
	lua_getglobal(L, DMP_TM_NAMESPACE);
	lua_getfield(L, -1, DMP_TM_MODULE);
	lua_pushcfunction(L, dmp_tm_lua_set_status);
	lua_setfield(L, -2, "set_status");
}

gint dmp_tm_lua_initialize()
{
	dmp_tm_state = luaL_newstate();
	luaL_openlibs(dmp_tm_state);
	
	if (luaL_loadfile(dmp_tm_state, DMP_TM_MODULE_SCRIPT) != LUA_OK)
	{
		dmp_tm_console_write("Failed to load camera module script!\n");
		lua_close(dmp_tm_state);
		dmp_tm_state = NULL;
		dmp_tm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	
	if (lua_pcall(dmp_tm_state, 0, 1, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_tm_state, -1);
		dmp_tm_console_write((gchar *) error);
		lua_close(dmp_tm_state);
		dmp_tm_state = NULL;
		dmp_tm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	lua_setglobal(dmp_tm_state, DMP_TM_NAMESPACE);
	
	dmp_tm_lua_register_console(dmp_tm_state);
	
	dmp_tm_lua_register_set_status(dmp_tm_state);
	
	lua_getglobal(dmp_tm_state, DMP_TM_NAMESPACE);
	lua_getfield(dmp_tm_state, -1, DMP_TM_MODULE);
	lua_getfield(dmp_tm_state, -1, "initialize");
	if (lua_pcall(dmp_tm_state, 0, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_tm_state, -1);
		dmp_tm_console_write((gchar *) error);
		lua_close(dmp_tm_state);
		dmp_tm_state = NULL;
		dmp_tm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

gint dmp_tm_lua_finalize()
{
	lua_getglobal(dmp_tm_state, DMP_TM_NAMESPACE);
	lua_getfield(dmp_tm_state, -1, DMP_TM_MODULE);
	lua_getfield(dmp_tm_state, -1, "finalize");
	if (lua_pcall(dmp_tm_state, 0, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_tm_state, -1);
		dmp_tm_console_write((gchar *) error);
	}
	
	lua_close(dmp_tm_state);
	dmp_tm_state = NULL;
	return DMP_PB_SUCCESS;
}

