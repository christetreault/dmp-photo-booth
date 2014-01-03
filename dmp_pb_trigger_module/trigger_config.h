/* 
 * File:   trigger_config.h
 * Author: chris
 *
 * Created on January 3, 2014, 2:34 PM
 */

#ifndef TRIGGER_CONFIG_H
#define	TRIGGER_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include "dmp_trigger_module.h"
	
	/**
	 * initializes the configuration
     */
	void dmp_tm_config_init();
	
	/**
	 * Finalizes the configuration. Does not save changes.
     */
	void dmp_tm_config_finalize();
	
	/**
     * @return TRUE if the configuration is initialized, FALSE if not
     */
	gboolean dmp_tm_config_is_initialized();
	
	/**
     * @return Return the device name of the trigger. Caller is responsible for the
	 * memory management of the returned string.
     */
	gchar * dmp_tm_config_get_device_name();


#ifdef	__cplusplus
}
#endif

#endif	/* TRIGGER_CONFIG_H */

