#include "user_interface.h"

G_DEFINE_QUARK(DMP_PB_UI_ERROR, dmp_pb_ui_error)

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
 * @throws DMP_PB_UI_ERROR::UI_DEFINITION_CORRUPT
 */
static void dmp_pb_ui_register_user_data(GtkBuilder * builder, GError ** error)
{
	g_assert(g_module_supported());
	
	cb_user_data = malloc(sizeof (struct dmp_pb_ui_cb_user_data));

	cb_user_data->main_window = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_main_window"));
	if (cb_user_data->main_window == NULL)
	{
		free(cb_user_data);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	cb_user_data->about_window = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_about_dialog"));
	if (cb_user_data->about_window == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	cb_user_data->config_window = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_options_dialog"));
	if (cb_user_data->config_window == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	cb_user_data->camera_entry = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_camera_module_path_text_box"));
	if (cb_user_data->camera_entry == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		gtk_widget_destroy(cb_user_data->config_window);
		free(cb_user_data);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	cb_user_data->trigger_entry = GTK_WIDGET(gtk_builder_get_object(builder, "dmp_pb_trigger_module_path_text_box"));
	if (cb_user_data->trigger_entry == NULL)
	{
		gtk_widget_destroy(cb_user_data->about_window);
		gtk_widget_destroy(cb_user_data->main_window);
		gtk_widget_destroy(cb_user_data->config_window);
		gtk_widget_destroy(cb_user_data->camera_entry);
		free(cb_user_data);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
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
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	status_icons = malloc(sizeof (dmp_pb_ui_status_icons));
	status_icons->printer_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_printer_module_status_icon"));
	if (status_icons->printer_module_staus_icon == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	status_icons->trigger_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_trigger_module_status_icon"));
	if (status_icons->trigger_module_staus_icon == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	status_icons->camera_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_camera_module_status_icon"));
	if (status_icons->camera_module_staus_icon == NULL)
	{
		gtk_widget_destroy(cb_user_data->main_window);
		free(cb_user_data);
		free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	dmp_pb_console_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "dmp_pb_console_buffer"));

	if (dmp_pb_console_buffer == NULL)
	{
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}
	
	gtk_builder_connect_signals(builder, (gpointer) cb_user_data);
}

/**
 * Creates a new GtkBuilder using the passed-in GtkBuilder .xml definition
 * @param builder_file the .xml definition's location
 * @throws GTK_BUILDER_ERROR, G_MARKUP_ERROR, G_FILE_ERROR
 * @return A pointer to a new GtkBuilder, or NULL
 */
static GtkBuilder * dmp_pb_ui_create_gtk_builder(gchar * builder_file, GError ** error)
{
	GtkBuilder * builder = gtk_builder_new();
	GError * working_error = NULL;

	if (!gtk_builder_add_from_file(builder, builder_file, &working_error))
	{
		g_object_unref(GTK_BUILDER(builder));
		g_propagate_error(error, working_error);
		return NULL;
	}
	return builder;
}

/* --------------------------- */
/* End static helper functions */
/* --------------------------- */

void dmp_pb_ui_launch(gchar * ui_file, GError ** error)
{
	GError * working_error = NULL;
	
	GtkBuilder * builder = dmp_pb_ui_create_gtk_builder(ui_file, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	dmp_pb_ui_register_user_data(builder, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	g_object_unref(G_OBJECT(builder));

	dmp_pb_mwd_init(status_icons);
	g_idle_add(dmp_pb_mwd_handle_message, NULL);
	g_timeout_add_seconds(1, dmp_pb_console_queue_flush_queue, dmp_pb_console_buffer);

	gtk_widget_show(cb_user_data->main_window);
	gtk_main();

	dmp_pb_mwd_finalize();
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
	gtk_widget_destroy(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *) user_data)->main_window));
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
	gtk_widget_show(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *) user_data)->config_window));
}

/* ------------------- */
/* Help Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_launch_help_activate(GtkMenuItem * menuitem, gpointer user_data)
{

}

G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_about_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *) user_data)->about_window));
}

/* ---------------------- */
/* About Dialog callbacks */
/* ---------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_about_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	gtk_widget_hide(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *) user_data)->about_window));
}

/* ----------------------- */
/* Config Dialog callbacks */
/* ----------------------- */

G_MODULE_EXPORT void dmp_pb_options_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	if (response == 2) gtk_widget_hide(GTK_WIDGET(((struct dmp_pb_ui_cb_user_data *) user_data)->config_window));
	else if (response == 1) return;
}

/**
 * Helper for load module callbacks. Loads the specified module type, based on 
 * the button pushed
 * @param user_data the user_data struct to use
 * @param type the module type to load
 * @throws DMP_PB_MODULE_ERROR::G_MODULE_LOAD_FAILURE
 */
static void dmp_pb_ui_load_module_button_helper(struct dmp_pb_ui_cb_user_data * user_data, dmp_pb_module_type type, GError ** error)
{
	GString * path;
	GtkEntry * current;
	GError * working_error = NULL;
	
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
	dmp_pb_load_module(type, path, &working_error);

	if (working_error != NULL)
	{
		g_string_free(path, TRUE);
		g_propagate_error(error, working_error);
		return;
	}

	g_string_free(path, TRUE);
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_camera_module_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_ui_load_module_button_helper((struct dmp_pb_ui_cb_user_data *) user_data, DMP_PB_CAMERA_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_trigger_module_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_ui_load_module_button_helper((struct dmp_pb_ui_cb_user_data *) user_data, DMP_PB_TRIGGER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_printer_module_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_ui_load_module_button_helper((struct dmp_pb_ui_cb_user_data *) user_data, DMP_PB_PRINTER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

/**
 * Calls edit module config on the passed-i type. Brings the GError
 * @param type
 * @throws DMP_PB_UI_ERROR::LIBRARY_CALL_FAILURE
 */
static void dmp_pb_edit_module_config_button_helper(dmp_pb_module_type type, GError ** error)
{
	if (dmp_pb_edit_module_config(type) != DMP_PB_SUCCESS)
	{
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				LIBRARY_CALL_FAILURE,
				"Call to edit module config failed");
		return;
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_camera_module_config_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_edit_module_config_button_helper(DMP_PB_CAMERA_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_trigger_module_config_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_edit_module_config_button_helper(DMP_PB_TRIGGER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_printer_module_config_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_edit_module_config_button_helper(DMP_PB_TRIGGER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}