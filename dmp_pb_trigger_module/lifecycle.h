/* 
 * File:   lifecycle.h
 * Author: chris
 *
 * Created on December 10, 2013, 10:22 AM
 */

#ifndef LIFECYCLE_H
#define	LIFECYCLE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include "global_defines.h"
#include "serial_io.h"
#include "dmp_trigger_module.h"
	
	/**
	 * Pushes a GError into the console queue
     * @param error the error to push
     */
	void dmp_tm_lifecycle_push_error(GError * error);
	
	/**
	 * initializes the trigger module
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	gint dmp_tm_lifecycle_initialize();
	
	/**
	 * finalizes the trigger module
     * @return DMP_PB_SUCCESS, or DMP_PB_FAILURE
     */
	gint dmp_tm_lifecycle_finalize();
	
	/**
	 * tests to see if the trigger module is initialized
     * @return TRUE if it is, FALSE if not
     */
	gboolean dmp_tm_lifecycle_is_initialized();
	


#ifdef	__cplusplus
}
#endif

#endif	/* LIFECYCLE_H */

