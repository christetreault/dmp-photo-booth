/* 
 * File:   dmp_trigger_module.h
 * Author: chris
 *
 * Created on October 6, 2013, 9:34 PM
 */

#ifndef DMP_TRIGGER_MODULE_H
#define	DMP_TRIGGER_MODULE_H

#define DMP_TM_CONFIG_FILE "dmp_trigger_module.rc"

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include "global_defines.h"
#include <glib.h>
#include "serial_io.h"
#include "lifecycle.h"
#include "trigger_config.h"

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
	
	/**
	 * Sets the status monitor handler callback
     * @param sh the callback to set
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	int dmp_tm_install_status_handler(void (*sh)(int status));
	
	/**
	 * Sets the status of the module
     * @param status Is it true that the module is good?
     */
	void dmp_tm_set_status(gboolean status);

#ifdef	__cplusplus
}
#endif

#endif	/* DMP_TRIGGER_MODULE_H */

