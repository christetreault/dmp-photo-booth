/* 
 * File:   printer_config.h
 * Author: chris
 *
 * Created on January 2, 2014, 3:40 PM
 */

#ifndef PRINTER_CONFIG_H
#define	PRINTER_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include "dmp_printer_module.h"
#include <cups/cups.h>
	
	/**
	 * initializes the configuration
     */
	void dmp_pm_config_init(void);
	
	/**
	 * Finalizes the configuration. Does not save changes.
     */
	void dmp_pm_config_finalize(void);
	
	/**
	 * Reads the DPI of the image to be printed from config
     * @return the DPI, in pixels
     */
	gsize dmp_pm_config_get_dpi(void);
	
	/**
	 * Reads the height of the print canvas from config
     * @return the height, in inches
     */
	gsize dmp_pm_config_get_canvas_height(void);
	
	/**
	 * Reads the width of the print canvas from config
     * @return the width, in inches
     */
	gsize dmp_pm_config_get_canvas_width(void);
	
	/**
	 * Reads the name of the printer to print to from config
     * @return the name of the printer. The caller is responsible for memory
	 * management of this pointer!
     */
	gchar * dmp_pm_config_get_printer_name(void);
	
	/**
	 * Reads if the print job should go to the printer, of if an image should be
	 * dumped to /tmp. This is mainly for debugging purposes. If this property is
	 * undefined, the function will return FALSE
     * @return TRUE if the job should dump a file. False if not.
     */
	gboolean dmp_pm_config_print_to_file(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PRINTER_CONFIG_H */

