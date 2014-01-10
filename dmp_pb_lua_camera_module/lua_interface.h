/* 
 * File:   lua_interface.h
 * Author: chris
 *
 * Created on January 6, 2014, 2:09 PM
 */

#ifndef LUA_INTERFACE_H
#define	LUA_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <glib.h>
	
	typedef enum
	{
		CONSOLE_CALLBACK_REGISTRATION_FAILURE
	}dmp_cm_lua_error;
	
	/**
	 * Calls into lua's capture function
     * @param location the location to download to
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_cm_lua_capture(gchar * location);

	/**
	 * Initializes Lua
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_cm_lua_initialize();

	/**
	 * Finalizes Lua
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_cm_lua_finalize();


#ifdef	__cplusplus
}
#endif

#endif	/* LUA_INTERFACE_H */

