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
#include "../dmp_photo_booth/global_defines.h"
	
#include <stdio.h>
#include <string.h>
	/**
	 * signals the camera to capture a picture
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_capture();
	
	/**
	 * Downloads the latest taken picture to the passed in location
     * @param location the location to download to
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_cm_download(char * location);
	
	/**
	 * Returns the location of the camera module configuration file
     * @param to_fill an allocated char array to place the returned value in
	 * @param size the size of the char array
     * @return The location of the config file, or NULL if on error. If size
	 * isn't large enough to contain the path, it will be truncated
     */
	char * dmp_cm_get_config_location(char * to_fill, size_t size);
	
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


#ifdef	__cplusplus
}
#endif

#endif	/* CAMERA_MODULE_H */

