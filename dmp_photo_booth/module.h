/* 
 * File:   module.h
 * Author: chris
 *
 * Created on October 9, 2013, 12:57 PM
 */


#ifndef MODULE_H
#define	MODULE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "global_defines.h"
#include <stdlib.h>
#include <gmodule.h>
#include "error_handling.h"

	/**
	 * Enumeration of DMP Photo Booth modules
	 */
	typedef enum {
		DMP_PB_PRINTER_MODULE, DMP_PB_CAMERA_MODULE, DMP_PB_TRIGGER_MODULE
	} dmp_pb_module_type;
	
	/**
	 * Enumeration of Module GError codes
	 */
	typedef enum {
		G_MODULE_LOAD_FAILURE
	} dmp_pb_module_error;
	
	/**
	 * Enumeration of Module states
	 */
	typedef enum {
		DMP_PB_MODULE_IN_INCONSISTENT_STATE, 
		DMP_PB_MODULE_NOT_LOADED, 
		DMP_PB_MODULE_NOT_INITIALIZED,
		DMP_PB_MODULE_READY
	} dmp_pb_module_state;

	/**
	 * loads a module
	 * @param to_load the type of module to be loaded
	 * @param module_location the location on the filesystem of the module
	 * @throws DMP_PB_MODULE_ERROR::G_MODULE_LOAD_FAILURE
	 */
	void dmp_pb_load_module(dmp_pb_module_type to_load, GString * module_location, GError ** error);
	/**
	 * unloads a module
	 * @param to_unload the module type to be unloaded
	 */
	void dmp_pb_unload_module(dmp_pb_module_type to_unload);
	/**
	 * replaces the currently loaded module with the new module to load
	 * @param to_swap the module type to swap out
	 * @param new_module_location the location of the new module to load
	 * @throws DMP_PB_MODULE_ERROR::G_MODULE_LOAD_FAILURE
	 */
	void dmp_pb_swap_module(dmp_pb_module_type to_swap, GString * new_module_location, GError ** error);

	/**
	 * state checking helper function. checks is_loaded and is_consistent
	 * @param to_check the module type to check
	 * @return DMP_PB_MODULE_READY, DMP_PB_MODULE_IN_INCONSISTENT_STATE if inconsistent,
	 * DMP_PB_MODULE_NOT_LOADED if not loaded, or DMP_PB_MODULE_NOT_INITIALIZED if
	 * the module is not initialized
	*/
	dmp_pb_module_state dmp_pb_check_module_state(dmp_pb_module_type to_check);

	/* -------------------------------------------------- */
	/* Convenience functions for module function pointers */
	/* -------------------------------------------------- */

	/**
	 * returns the location of the config file for the specified module
	 * @param type the module type to query
	 * @return a string containing the config location, or NULL on error.
	 */
	gchar * dmp_pb_get_module_config_location(gchar * to_fill, gsize size, dmp_pb_module_type type);

	/**
	 * signals a module to edit its config
	 * @param type the module type to call
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_edit_module_config(dmp_pb_module_type type);

	/**
	 * signals a module to load or reload its config
	 * @param type the module type to call
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_load_module_config(dmp_pb_module_type type);

	/**
	 * signals the camera to capture a picture
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_cm_capture();
	/**
	 * Downloads the latest taken picture to the passed in location
	 * @param location the location to download to
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_cm_download(gchar * location);
	/**
	 * prints the file at the passed in location
	 * @param to_print the file to print
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_pm_print(gchar * to_print);
	/**
	 * adds the trigger callback function. When the user pushes "the button",
	 * this function should be called
	 * @param th the callback function
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_tm_add_trigger_handler(void (*th)());
	/**
	 * Sets the value to be displayed by the countdown
	 * @param current the current time, in seconds
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_tm_set_countdown(int current);

#ifdef	__cplusplus
}
#endif

#endif	/* MODULE_H */

