/* 
 * File:   dmp_printer_module.h
 * Author: chris
 *
 * Created on October 6, 2013, 9:21 PM
 */

#ifndef DMP_PRINTER_MODULE_H
#define	DMP_PRINTER_MODULE_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#define DMP_PM_CONFIG_FILE "dmp_printer_module.rc"
	
#include "global_defines.h"
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <cups/cups.h>
#include <wand/MagickWand.h>
#include "strip_processing.h"
#include "printer_config.h"

	/**
	 * prints the file at the passed in location
     * @param to_print the file to print
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_print(char * to_print);

	/**
	 * Edit the the module configuration
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_edit_config();

	/**
	 * loads or reloads the configuration for this module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_load_config();
	
	/**
	 * Installs the function used to write to the Core Application's console
     * @param c_cb the callback function to add
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_install_console(int (*c_cb)(char * message));
	
		/**
	 * initializes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_initialize();
	
	/**
	 * checks to see if the module is initialized
     * @return 0 if not initialized, !0 if initialized
     */
	int dmp_pm_is_initialized();
	
	/**
	 * finalizes the module
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_finalize();
	
	/**
	 * wrapper around console write function pointer
     * @param to_write the string to write
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	int dmp_pm_console_write(gchar * to_write);
	
	/**
	 * Sets the status monitor handler callback
     * @param sh the callback to set
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	int dmp_pm_install_status_handler(void (*sh)(int status));
	
	/**
	 * Sets the status of the module
     * @param status Is it true that the module is good?
     */
	void dmp_pm_set_status(gboolean status);

#ifdef	__cplusplus
}
#endif

#endif	/* DMP_PRINTER_MODULE_H */

