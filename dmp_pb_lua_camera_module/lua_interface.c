#include "lua_interface.h"
#include "dmp_camera_module.h"

#define DMP_CM_MODULE_SCRIPT "dmp_camera_module.lua"
#define DMP_CM_NAMESPACE "dmp"
#define DMP_CM_MODULE "cm"

static lua_State * dmp_cm_state = NULL;

G_DEFINE_QUARK(DMP_CM_LUA_ERROR, dmp_cm_lua_error)

static gint dmp_cm_lua_console_write(lua_State * L)
{
	if (lua_isstring(L, -1))
	{
		const gchar * message = lua_tostring(L, -1);
		dmp_cm_console_write((gchar *) message);
	}
	
	return 0;
}

static gint dmp_cm_lua_set_status(lua_State * L)
{
	if (lua_isboolean(L, -1))
	{
		gboolean return_value = lua_toboolean(L, -1);
		dmp_cm_set_status(return_value);
	}
	
	return 0;
}

gint dmp_cm_lua_capture(const gchar * location)
{	
	lua_getglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	lua_getfield(dmp_cm_state, -1, DMP_CM_MODULE);
	lua_getfield(dmp_cm_state, -1, "capture");
	lua_pushstring(dmp_cm_state, location);
	if (lua_pcall(dmp_cm_state, 1, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write(error);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

/**
 * Registers the console_write callback to dmp.cm.console_write
 * @param L the state to register into
 */
static void dmp_cm_lua_register_console(lua_State * L)
{
	lua_getglobal(L, DMP_CM_NAMESPACE);
	lua_getfield(L, -1, DMP_CM_MODULE);
	lua_pushcfunction(L, dmp_cm_lua_console_write);
	lua_setfield(L, -2, "console_write");
}

/**
 * Registers the set_status callback to dmp.cm.set_status
 * @param L the state to register into
 */
static void dmp_cm_lua_register_set_status(lua_State * L)
{
	lua_getglobal(L, DMP_CM_NAMESPACE);
	lua_getfield(L, -1, DMP_CM_MODULE);
	lua_pushcfunction(L, dmp_cm_lua_set_status);
	lua_setfield(L, -2, "set_status");
}

gint dmp_cm_lua_initialize(void)
{
	dmp_cm_state = luaL_newstate();
	luaL_openlibs(dmp_cm_state);
	
	if (luaL_loadfile(dmp_cm_state, DMP_CM_MODULE_SCRIPT) != LUA_OK)
	{
		dmp_cm_console_write("Failed to load camera module script!\n");
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		dmp_cm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	
	if (lua_pcall(dmp_cm_state, 0, 1, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write((gchar *) error);
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		dmp_cm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	lua_setglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	
	dmp_cm_lua_register_console(dmp_cm_state);
	dmp_cm_lua_register_set_status(dmp_cm_state);
	
	lua_getglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	lua_getfield(dmp_cm_state, -1, DMP_CM_MODULE);
	lua_getfield(dmp_cm_state, -1, "initialize");
	if (lua_pcall(dmp_cm_state, 0, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write((gchar *) error);
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		dmp_cm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

gint dmp_cm_lua_finalize(void)
{
	lua_getglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	lua_getfield(dmp_cm_state, -1, DMP_CM_MODULE);
	lua_getfield(dmp_cm_state, -1, "finalize");
	if (lua_pcall(dmp_cm_state, 0, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write((gchar *) error);
	}
	
	lua_close(dmp_cm_state);
	dmp_cm_state = NULL;
	return DMP_PB_SUCCESS;
}

