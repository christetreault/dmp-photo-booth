/* 
 * File:   strip_processing.h
 * Author: chris
 *
 * Created on December 16, 2013, 4:14 PM
 */

#ifndef STRIP_PROCESSING_H
#define	STRIP_PROCESSING_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include <cups/cups.h>
#include <glib/gstdio.h>
#include <wand/MagickWand.h>
#include "dmp_printer_module.h"
	
	/**
	 * Prepares a strip for printing
     * @param to_print the strip to print
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_pm_process_print(gchar * to_print);


#ifdef	__cplusplus
}
#endif

#endif	/* STRIP_PROCESSING_H */

