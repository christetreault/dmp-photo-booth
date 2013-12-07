/* 
 * File:   user_interface.h
 * Author: chris
 *
 * Created on October 23, 2013, 2:10 PM
 */

#ifndef USER_INTERFACE_H
#define	USER_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>
#include "console_queue.h"
#include "global_defines.h"
#include "error_handling.h"
#include "module_status_watchdog.h"
#include "configuration.h"

	/**
	 * For whatever reason, gtk.h does not typedef this
	 */
	typedef struct _GtkTextBuffer GtkTextBuffer;

	/**
	 * Enumeration of UI GError codes
	 */
	typedef enum {
		UI_DEFINITION_CORRUPT, LIBRARY_CALL_FAILURE
	} dmp_pb_ui_error;
	
	typedef struct _dmp_pb_ui_status_icons {
		GtkImage * printer_module_staus_icon;
		GtkImage * trigger_module_staus_icon;
		GtkImage * camera_module_staus_icon;
	} dmp_pb_ui_status_icons;

	/**
	 * @return a pointer to the dmp_pb_ui_status_icons struct. Caller should not
	 * attept to free this pointer
	 */
	dmp_pb_ui_status_icons * dmp_pb_ui_get_status_icons();
	
	/**
	 * Launches the UI, and calls gtk_main
	 * @param ui_file The glade UI file to use
	 * @throws GTK_BUILDER_ERROR, G_MARKUP_ERROR, G_FILE_ERROR,
	 * DMP_PB_UI_ERROR::UI_DEFINITION_CORRUPT
	 */
	void dmp_pb_ui_launch(gchar * ui_file, GError ** error);
	
	/**
	 * Tests to see if the Photo Booth is started
     * @return TRUE if started, FALSE if not
     */
	gboolean dmp_pb_ui_is_started();

	/* --------------- */
	/* Begin Callbacks */
	/* --------------- */

	/* ------------------- */
	/* File Menu callbacks */
	/* ------------------- */

	/**
	 * GTK3 can be kind of dumb; If you call gtk_main_quit() in response
	 * to say, a file->exit, or basically anything other than clicking the
	 * X, it will decrement the main_loops count <b>THEN</b> destroy the top
	 * level window, which will try to decrement the main_loops count. This
	 * causes an assertion failure which prints an ugly error to the console.
	 * 
	 * In order to not get said ugly error, this callback exists. It calls
	 * gtk_widget_destroy on the pointer to the top level window contained
	 * in the user_data pointer 
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_quit_activate(GtkMenuItem * menuitem, gpointer user_data);

	/**
	 * callback for file->start. Activates the photo booth
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_start_activate(GtkMenuItem * menuitem, gpointer user_data);

	/**
	 * callback for file->stop. Stops the photo booth.
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_stop_activate(GtkMenuItem * menuitem, gpointer user_data);

	/**
	 * callback for file->print. Pauses the photo booth. Allows operator to resume current session
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_print_activate(GtkMenuItem * menuitem, gpointer user_data);

	/**
	 * callback for file->save output. Allows the operator to save the output of the console
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_save_output_activate(GtkMenuItem * menuitem, gpointer user_data);

	/* ------------------- */
	/* Edit Menu callbacks */
	/* ------------------- */

	/**
	 * callback for edit->preferences. Invokes the preferences dialog
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_edit_submenu_preferences_activate(GtkMenuItem * menuitem, gpointer user_data);
	
		/**
	 * callback for the config dialog's ok and cancel buttons
	 * @param button
	 * @param response
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_options_dialog_response(GtkDialog * about, gint response, gpointer user_data);

	/**
	 * callback for the load camera module button
	 * @param button
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_load_camera_module_button_clicked(GtkButton * button, gpointer user_data);

	/**
	 * callback for the load trigger module button
	 * @param button
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_load_trigger_module_button_clicked(GtkButton * button, gpointer user_data);

	/**
	 * callback for the load printer module button
	 * @param button
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_load_printer_module_button_clicked(GtkButton * button, gpointer user_data);

	/**
	 * callback for the edit camera module config button
	 * @param button
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_edit_camera_module_config_button_clicked(GtkButton * button, gpointer user_data);

	/**
	 * callback for the edit trigger module config button
	 * @param button
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_edit_trigger_module_config_button_clicked(GtkButton * button, gpointer user_data);

	/**
	 * callback for the edit printer module config button
	 * @param button
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_edit_printer_module_config_button_clicked(GtkButton * button, gpointer user_data);

	/* ------------------- */
	/* Help Menu callbacks */
	/* ------------------- */

	/**
	 * callback for help->Launch Help. Invokes the help system
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_launch_help_activate(GtkMenuItem * menuitem, gpointer user_data);

	/**
	 * callback for help->about. Invokes the about dialog
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_about_activate(GtkMenuItem * menuitem, gpointer user_data);

	/**
	 * callback the about dialog's close button. hides the window
	 * @param menuitem
	 * @param user_data
	 */
	G_MODULE_EXPORT void dmp_pb_ui_cb_about_dialog_response(GtkDialog * about, gint response, gpointer user_data);



#ifdef	__cplusplus
}
#endif

#endif	/* USER_INTERFACE_H */

