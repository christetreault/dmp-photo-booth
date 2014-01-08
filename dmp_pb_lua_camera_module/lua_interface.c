#include "lua_interface.h"
#include "dmp_camera_module.h"
#include "error_handling.h"

#define DMP_CM_MODULE_SCRIPT "dmp_camera_module.lua"
#define DMP_CM_NAMESPACE "dmp"
#define DMP_CM_MODULE "cm"

static lua_State * dmp_cm_state = NULL;
static gboolean is_initialized = FALSE;

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

gint dmp_cm_lua_capture(gchar * location)
{
	g_assert(dmp_cm_lua_is_initialized());
	
	lua_getglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	lua_getfield(dmp_cm_state, -1, DMP_CM_MODULE);
	lua_getfield(dmp_cm_state, -1, "capture");
	lua_pushstring(dmp_cm_state, location);
	if (lua_pcall(dmp_cm_state, 1, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write((gchar *) error);
		return DMP_PB_FAILURE;
	}
	return DMP_PB_SUCCESS;
}

/**
 * Registers the console_write callback to dmp.cm.console_write
 * @param L the state to register into
 * @throws Nothing, at the moment. Seems like the sort of function that should,
 * so this is here for potential future errors
 */
static void dmp_cm_lua_register_console(lua_State * L, GError ** error)
{
	lua_getglobal(L, DMP_CM_NAMESPACE);
	lua_getfield(L, -1, DMP_CM_MODULE);
	lua_pushcfunction(L, dmp_cm_lua_console_write);
	lua_setfield(L, -2, "console_write");
}

gint dmp_cm_lua_initialize()
{
	g_assert(!dmp_cm_lua_is_initialized());
	
	GError * error = NULL;
	
	dmp_cm_state = luaL_newstate();
	luaL_openlibs(dmp_cm_state);
	
	if (luaL_loadfile(dmp_cm_state, DMP_CM_MODULE_SCRIPT) != LUA_OK)
	{
		dmp_cm_console_write("Failed to load camera module script!\n");
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		return DMP_PB_FAILURE;
	}
	
	if (lua_pcall(dmp_cm_state, 0, 1, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write((gchar *) error);
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		return DMP_PB_FAILURE;
	}
	lua_setglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	
	dmp_cm_lua_register_console(dmp_cm_state, &error);
	
	if (error != NULL)
	{
		dmp_cm_error_console_write(error);
		g_clear_error(&error);
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		return DMP_PB_FAILURE;
	}
	
	lua_getglobal(dmp_cm_state, DMP_CM_NAMESPACE);
	lua_getfield(dmp_cm_state, -1, DMP_CM_MODULE);
	lua_getfield(dmp_cm_state, -1, "initialize");
	if (lua_pcall(dmp_cm_state, 0, 0, 0) != LUA_OK)
	{
		const gchar * error = lua_tostring(dmp_cm_state, -1);
		dmp_cm_console_write((gchar *) error);
		lua_close(dmp_cm_state);
		dmp_cm_state = NULL;
		return DMP_PB_FAILURE;
	}
	
	is_initialized = TRUE;
	return DMP_PB_SUCCESS;
}

gboolean dmp_cm_lua_is_initialized()
{
	gboolean return_value;
	if (dmp_cm_state != NULL && dmp_cm_lua_is_initialized)
	{
		lua_getglobal(dmp_cm_state, DMP_CM_NAMESPACE);
		lua_getfield(dmp_cm_state, -1, DMP_CM_MODULE);
		lua_getfield(dmp_cm_state, -1, "is_initialized");
		if (lua_pcall(dmp_cm_state, 0, 1, 0) != LUA_OK)
		{
			const gchar * error = lua_tostring(dmp_cm_state, -1);
			dmp_cm_console_write((gchar *) error);
			return FALSE;
		}
		return_value = lua_toboolean(dmp_cm_state, -1);
		return return_value;
	}
	return FALSE;
}

gint dmp_cm_lua_finalize()
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
	is_initialized = FALSE;
	return DMP_PB_SUCCESS;
}

