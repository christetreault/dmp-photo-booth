/* 
 * File:   camera_module.h
 * Author: chris
 *
 * Created on October 6, 2013, 8:53 PM
 */

#ifndef CAMERA_MODULE_H
#define	CAMERA_MODULE_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include "global_defines.h"
#include <glib.h>
#include "lua_interface.h"
	/**
	 * signals the camera to capture a picture. Downloads the taken picture to
	 * the passed-in location
	 * @param location the location to download to
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_capture(const char * location);
	
	/**
	 * Edit the the module configuration
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_edit_config(void);
	
	/**
	 * Installs the function used to write to the Core Application's console
     * @param c_cb the callback function to add
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_install_console(int (*c_cb)(const char * message));
	
	/**
	 * initializes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_initialize(void);
	
	/**
	 * finalizes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_finalize(void);
	
	/**
	 * wrapper around console write function pointer
     * @param to_write the string to write
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	int dmp_cm_console_write(const gchar * to_write);
	
	/**
	 * Sets the status monitor handler callback
     * @param sh the callback to set
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	int dmp_cm_install_status_handler(void (*sh)(int status));
	
	/**
	 * Sets the status of the module
     * @param status Is it true that the module is good?
     */
	void dmp_cm_set_status(gboolean status);


#ifdef	__cplusplus
}
#endif

#endif	/* CAMERA_MODULE_H */

