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
#include <stdio.h>
#include <string.h>
#include <glib.h>
	/**
	 * signals the camera to capture a picture. Downloads the taken picture to
	 * the passed-in location
	 * @param location the location to download to
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_capture(char * location);
	
	/**
	 * Edit the the module configuration
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_edit_config();

	/**
	 * loads or reloads the configuration for this module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_load_config();
	
	/**
	 * Installs the function used to write to the Core Application's console
     * @param c_cb the callback function to add
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_install_console(int (*c_cb)(char * message));
	
	/**
	 * initializes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_initialize();
	
	/**
	 * checks to see if the module is initialized
     * @return 0 if not initialized, !0 if initialized
     */
	int dmp_cm_is_initialized();
	
	/**
	 * finalizes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_finalize();
	
	/**
	 * wrapper around console write function pointer
     * @param to_write the string to write
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	int dmp_cm_console_write(gchar * to_write);


#ifdef	__cplusplus
}
#endif

#endif	/* CAMERA_MODULE_H */

