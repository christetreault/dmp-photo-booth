/* 
 * File:   module_status_watchdog.h
 * Author: chris
 *
 * Created on November 4, 2013, 8:42 PM
 */

#ifndef MODULE_STATUS_WATCHDOG_H
#define	MODULE_STATUS_WATCHDOG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include <gtk/gtk.h>
#include "module.h"
#include "user_interface.h"

	typedef struct _dmp_pb_ui_status_icons dmp_pb_ui_status_icons;
	/**
	 * Initializes the Module Status Watchdog
	 * @param in_status_icons The status icons struct for use by the watchdog
	 */
	void dmp_pb_mwd_init(dmp_pb_ui_status_icons * in_status_icons);

	/**
	 * Polls the status of the modules, updates the module status indicators
	 * @param user_data NULL
	 * @return G_SOURCE_CONTINUE
	 */
	gboolean dmp_pb_mwd_poll_modules(gpointer user_data);




#ifdef	__cplusplus
}
#endif

#endif	/* MODULE_STATUS_WATCHDOG_H */

