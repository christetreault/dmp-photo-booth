/* 
 * File:   dmp_trigger_module.h
 * Author: chris
 *
 * Created on October 6, 2013, 9:34 PM
 */

#ifndef DMP_TRIGGER_MODULE_H
#define	DMP_TRIGGER_MODULE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include "global_defines.h"
#include <glib.h>
#include "serial_io.h"

	/**
	 * indicate to the user that something went wrong
     * @param value an error code
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_show_error(int value);
	
	/**
	 * adds the trigger callback function. When the user pushes "the button",
	 * this function should be called
     * @param th the callback function
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_add_trigger_handler(void (*th)());
	
	/**
	 * Sets the value to be displayed by the countdown
     * @param current the current time, in seconds
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_set_countdown(int current);
	
	/**
	 * Returns the location of the trigger module configuration file
     * @param to_fill an allocated char array to place the returned value in
	 * @param size the size of the char array
     * @return The location of the config file, or NULL if on error. If size
	 * isn't large enough to contain the path, it will be truncated
     */
	char * dmp_tm_get_config_location(char * to_fill, size_t size);
	
	/**
	 * Edit the the module configuration
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_edit_config();

	/**
	 * loads or reloads the configuration for this module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_load_config();
	
	/**
	 * Installs the function used to write to the Core Application's console
     * @param c_cb the callback function to add
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_install_console(int (*c_cb)(char * message));

	/**
	 * initializes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_initialize();
	
	/**
	 * checks to see if the module is initialized
     * @return 0 if not initialized, !0 if initialized
     */
	int dmp_tm_is_initialized();
	
	/**
	 * finalizes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_tm_finalize();
	
	/**
	 * wrapper around the console write callback
     * @param to_write the string to write
     * @return success, or failure
     */
	int dmp_tm_console_write(gchar * to_write);
	
	/**
	 * wrapper around the trigger handler function
     */
	void dmp_tm_call_trigger_handler();

#ifdef	__cplusplus
}
#endif

#endif	/* DMP_TRIGGER_MODULE_H */

