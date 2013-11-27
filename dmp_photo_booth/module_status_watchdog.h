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

#define DMP_PB_MWD_CAMERA_UP 1
#define DMP_PB_MWD_CAMERA_DOWN 2
#define DMP_PB_MWD_PRINTER_UP 3
#define DMP_PB_MWD_PRINTER_DOWN 4
#define DMP_PB_MWD_TRIGGER_UP 5
#define DMP_PB_MWD_TRIGGER_DOWN 6

	typedef struct _dmp_pb_ui_status_icons dmp_pb_ui_status_icons;
	/**
	 * Initializes the Module Status Watchdog
	 * @param in_status_icons The status icons struct for use by the watchdog
	 */
	void dmp_pb_mwd_init(dmp_pb_ui_status_icons * in_status_icons);

	/**
	 * Finalizes the Module Status Watchdog, freeing the queue and signaling
	 * the thread to die
	 */
	void dmp_pb_mwd_finalize();

	/**
	 * Handles 1 message in the queue. 
	 * @param user_data. This does nothing, and should be set to NULL
	 * @return G_SOURCE_CONTINUE. This function should not stop until gtk_main_quit
	 * has been called
	 */
	gboolean dmp_pb_mwd_handle_message(gpointer user_data);




#ifdef	__cplusplus
}
#endif

#endif	/* MODULE_STATUS_WATCHDOG_H */

