#include "user_interface.h"



struct dmp_pb_ui_cb_user_data
{
	GtkWidget * main_window;
	GtkWidget * about_window;
	GtkWidget * config_window;
	
	/* Text Boxes for the module configuration page */
	
	GtkWidget * camera_entry;
	GtkWidget * trigger_entry;
	GtkWidget * printer_entry;
	
};

static struct dmp_pb_ui_cb_user_data * cb_user_data;
static dmp_pb_ui_status_icons * status_icons = NULL;
static GtkTextBuffer * dmp_pb_console_buffer = NULL;

/* ----------------------------- */
/* Begin static helper functions */
/* ----------------------------- */

/**
 * Fills the user_data struct, and passes it into the GtkBuilder by way of
 * calling gtk_builder_connect_signals
 * @param builder The builder to connect signals to
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_ui_register_user_data(GtkBuilder * builder)
{
	cb_user_data = malloc(sizeof(struct dmp_pb_ui_cb_user_data));
	
	cb_user_data->main_window = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_main_window"));
	if (cb_user_data->main_window == NULL)
	{
		free(cb_user_data);
		return dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	cb_user_data->about_window = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_about_dialog"));
	if (cb_user_data->about_window == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		return dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	cb_user_data->config_window = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_options_dialog"));
	if (cb_user_data->config_window == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		return dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	cb_user_data->camera_entry = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_camera_module_path_text_box"));
	if (cb_user_data->camera_entry == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		gtk_widget_destroy(cb_user_data->config_window);
		free(cb_user_data);
		return dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	cb_user_data->trigger_entry = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_trigger_module_path_text_box"));
	if (cb_user_data->trigger_entry == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		gtk_widget_destroy(cb_user_data->config_window);
		gtk_widget_destroy(cb_user_data->camera_entry);
		free(cb_user_data);
		return dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	cb_user_data->printer_entry = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_printer_module_path_text_box"));
	if (cb_user_data->printer_entry == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		gtk_widget_destroy(cb_user_data->config_window);
		gtk_widget_destroy(cb_user_data->camera_entry);
		gtk_widget_destroy(cb_user_data->printer_entry);
		free(cb_user_data);
		return dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	status_icons = malloc(sizeof(dmp_pb_ui_status_icons));
	status_icons->printer_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_printer_module_status_icon"));
	if (status_icons->printer_module_staus_icon == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		free(status_icons);
		dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	status_icons->trigger_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_trigger_module_status_icon"));
	if (status_icons->trigger_module_staus_icon == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		free(status_icons);
		dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	status_icons->camera_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_camera_module_status_icon"));
	if (status_icons->camera_module_staus_icon == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		free(status_icons);
		dmp_pb_set_error_code_return(DMP_PB_GTK_CALL_FAILED);
	}
	
	dmp_pb_console_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "dmp_pb_console_buffer"));
	
	gtk_builder_connect_signals(builder, (gpointer)cb_user_data);
	return DMP_PB_SUCCESS;
}

/**
 * Creates a new GtkBuilder using the passed-in GtkBuilder .xml definition
 * @param builder_file the .xml definition's location
 * @return A pointer to a new GtkBuilder, or NULL
 */
static GtkBuilder * dmp_pb_ui_create_gtk_builder(gchar * builder_file)
{
	GtkBuilder * builder = gtk_builder_new();
	GError * error = NULL;
	
	if (!gtk_builder_add_from_file(builder, builder_file, &error))
	{
		GString * error_message = g_string_new(NULL);
		g_string_printf(error_message, "Error %d: %s\n", DMP_PB_GTK_CALL_FAILED, error->message);
		dmp_pb_console_queue_push(error_message);
		g_error_free(error);
		g_object_unref(GTK_BUILDER(builder));
		dmp_pb_set_error_code(DMP_PB_GTK_CALL_FAILED);
		return NULL;
	}
	return builder;
}

/* --------------------------- */
/* End static helper functions */
/* --------------------------- */

gint dmp_pb_ui_launch(gchar * ui_file)
{
	GtkBuilder * builder = dmp_pb_ui_create_gtk_builder(ui_file);
	if (builder == NULL) return dmp_pb_get_last_error_code();
	
	if (dmp_pb_ui_register_user_data(builder) != DMP_PB_SUCCESS) return dmp_pb_get_last_error_code();
	g_object_unref(G_OBJECT(builder));
	
	dmp_pb_mwd_init(status_icons);
	g_idle_add(dmp_pb_mwd_handle_message, NULL);
	g_timeout_add_seconds(1, dmp_pb_console_queue_flush_queue, dmp_pb_console_buffer);
	
	gtk_widget_show(cb_user_data->main_window);
	gtk_main();
	
	dmp_pb_mwd_finalize();
	
	return DMP_PB_SUCCESS;
}

dmp_pb_ui_status_icons * dmp_pb_ui_get_status_icons()
{
	return status_icons;
}

/* --------------- */
/* Begin Callbacks */
/* --------------- */

/* ------------------- */
/* File Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_quit_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	 gtk_widget_destroy(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *)user_data)->main_window));
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_start_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_stop_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_pause_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_save_output_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	
}

/* ------------------- */
/* Edit Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_submenu_copy_activate(GtkMenuItem * menuitem, gpointer user_data)
{
		
}
	
G_MODULE_EXPORT void dmp_pb_ui_cb_edit_submenu_preferences_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *)user_data)->config_window));
}

/* ------------------- */
/* Help Menu callbacks */
/* ------------------- */
	
G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_launch_help_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	
}
	
G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_about_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *)user_data)->about_window));
}

/* ---------------------- */
/* About Dialog callbacks */
/* ---------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_about_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	gtk_widget_hide(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *)user_data)->about_window));
}

/* ----------------------- */
/* Config Dialog callbacks */
/* ----------------------- */

G_MODULE_EXPORT void dmp_pb_options_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	if (response == 2) gtk_widget_hide(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *)user_data)->config_window));
	else if (response == 1) return;
}

/**
 * Helper for load module callbacks. Loads the specified module type, based on 
 * the button pushed
 * @param user_data the user_data struct to use
 * @param type the module type to load
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_ui_load_module_button_helper(struct dmp_pb_ui_cb_user_data * user_data, dmp_pb_module_type type)
{
	GString * path;
	GString * message;
	gint result;
	GtkEntry * current;
	switch (type)
	{
		case DMP_PB_CAMERA_MODULE:
			current = GTK_ENTRY(user_data->camera_entry);
			break;
		case DMP_PB_TRIGGER_MODULE:
			current = GTK_ENTRY(user_data->trigger_entry);
			break;
		case DMP_PB_PRINTER_MODULE:
			current = GTK_ENTRY(user_data->printer_entry);
			break;
	}
	path = g_string_new(gtk_entry_get_text(current));
	result = dmp_pb_load_module(type, path);
	
	if (result != DMP_PB_SUCCESS)
	{
		message = g_string_new(NULL);
		g_string_printf(message, "Return value %d: Failed to load module: \"%s\"\n", result, path->str);
		dmp_pb_console_queue_push(message);
		dmp_pb_unload_module(type);
	}
	
	g_string_free(path, TRUE);
	return result;
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_camera_module_button_clicked(GtkButton * button, gpointer user_data)
{
	dmp_pb_ui_load_module_button_helper((struct dmp_pb_ui_cb_user_data *)user_data, DMP_PB_CAMERA_MODULE);
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_trigger_module_button_clicked(GtkButton * button, gpointer user_data)
{
	dmp_pb_ui_load_module_button_helper((struct dmp_pb_ui_cb_user_data *)user_data, DMP_PB_TRIGGER_MODULE);
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_printer_module_button_clicked(GtkButton * button, gpointer user_data)
{
	dmp_pb_ui_load_module_button_helper((struct dmp_pb_ui_cb_user_data *)user_data, DMP_PB_PRINTER_MODULE);
}

static gint dmp_pb_edit_module_config_button_helper(dmp_pb_module_type type)
{
	gint result = dmp_pb_edit_module_config(type);
	GString * message;
	
	if (result != DMP_PB_SUCCESS)
	{
		message = g_string_new(NULL);
		g_string_printf(message, "Return value %d: Call to edit module config failed.\n", result);
		dmp_pb_console_queue_push(message);
	}
	
	return result;
}

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_camera_module_config_button_clicked(GtkButton * button, gpointer user_data)
{
	dmp_pb_edit_module_config_button_helper(DMP_PB_CAMERA_MODULE);
}

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_trigger_module_config_button_clicked(GtkButton * button, gpointer user_data)
{
	dmp_pb_edit_module_config_button_helper(DMP_PB_TRIGGER_MODULE);
}

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_printer_module_config_button_clicked(GtkButton * button, gpointer user_data)
{
	dmp_pb_edit_module_config_button_helper(DMP_PB_TRIGGER_MODULE);
}