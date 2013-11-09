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
	
#include "../DMP_Photo_Booth/global_defines.h"
	
#include <stdio.h>
#include <string.h>

	/**
	 * prints the file at the passed in location
     * @param to_print the file to print
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pm_print(char * to_print);

	/**
	 * Returns the location of the printer module configuration file
     * @param to_fill an allocated char array to place the returned value in
	 * @param size the size of the char array
     * @return The location of the config file, or NULL if on error. If size
	 * isn't large enough to contain the path, it will be truncated
     */
	char * dmp_pm_get_config_location(char * to_fill, size_t size);
	
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

#ifdef	__cplusplus
}
#endif

#endif	/* DMP_PRINTER_MODULE_H */

