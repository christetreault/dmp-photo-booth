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
#include "global_defines.h"
	
	/**
	 * Initializes Lua
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_tm_lua_initialize();

	/**
	 * Finalizes Lua
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_tm_lua_finalize();
	
	/**
	 * Sets the count in the trigger
     * @param count the count to set
     * @return SUCCESS or FAILURE
     */
	gint dmp_tm_lua_set_countdown(gint count);
	
	/**
	 * pushes an error code to the trigger
     * @param value the code to push
     * @return SUCCESS or FAILURE
     */
	gint dmp_tm_lua_show_error(gint value);


#ifdef	__cplusplus
}
#endif

#endif	/* LUA_INTERFACE_H */

