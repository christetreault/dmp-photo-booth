#include "user_interface.h"
#include "configuration.h"
#include "photo_strip.h"
#include "coordination.h"

G_DEFINE_QUARK(DMP_PB_UI_ERROR, dmp_pb_ui_error)

#define DMP_PB_WAIT_FOR_GTK do {gtk_main_iteration();} while (gtk_events_pending())

#define DMP_PB_DOCUMENTATION_URL "http://doingmyprogramming.com/category/projects/dmp-photo-booth/"

/* User data widgets */

#define DMP_PB_MAIN_WINDOW "dmp_pb_main_window"
#define DMP_PB_ABOUT_DIALOG "dmp_pb_about_dialog"
#define DMP_PB_OPTIONS_DIALOG "dmp_pb_options_dialog"

#define DMP_PB_CAMERA_MODULE_PATH_TEXT_BOX "dmp_pb_camera_module_path_text_box"
#define DMP_PB_TRIGGER_MODULE_PATH_TEXT_BOX "dmp_pb_trigger_module_path_text_box"
#define DMP_PB_PRINTER_MODULE_PATH_TEXT_BOX "dmp_pb_printer_module_path_text_box"

#define DMP_PB_BACKGROUND_PATH_TEXT_BOX "dmp_pb_background_path_text_box"
#define DMP_PB_INDIVIDUAL_IMAGE_PATH_TEXT_BOX "dmp_pb_individual_image_path_text_box"
#define DMP_PB_COMPLETE_STRIP_PATH_TEXT_BOX "dmp_pb_complete_strip_path_text_box"

#define DMP_PB_PHOTO_STRIP_POSITION_1 "dmp_pb_photo_strip_position_1"
#define DMP_PB_PHOTO_STRIP_POSITION_2 "dmp_pb_photo_strip_position_2"
#define DMP_PB_PHOTO_STRIP_POSITION_3 "dmp_pb_photo_strip_position_3"
#define DMP_PB_PHOTO_STRIP_POSITION_4 "dmp_pb_photo_strip_position_4"
#define DMP_PB_PHOTO_STRIP_POSITION_5 "dmp_pb_photo_strip_position_5"

#define DMP_PB_INDIVIDUAL_IMAGE_WIDTH_SPIN_BOX "dmp_pb_individual_image_width_spin_box"

#define DMP_PB_IMAGE_HISTORY_VIEW "dmp_pb_image_history_view"

#define DMP_PB_FILE_SUBMENU_STOP "dmp_pb_file_submenu_stop"
#define DMP_PB_FILE_SUBMENU_START "dmp_pb_file_submenu_start"

#define DMP_PB_TOOLBAR_START_BUTTON "dmp_pb_toolbar_start_button"
#define DMP_PB_TOOLBAR_STOP_BUTTON "dmp_pb_toolbar_stop_button"

#define DMP_PB_WORKING_SPLASH_SCREEN "dmp_pb_working_splash_screen"

/* Status Icons */

#define DMP_PB_PRINTER_MODULE_STATUS_ICON "dmp_pb_printer_module_status_icon"
#define DMP_PB_TRIGGER_MODULE_STATUS_ICON "dmp_pb_trigger_module_status_icon"
#define DMP_PB_CAMERA_MODULE_STATUS_ICON "dmp_pb_camera_module_status_icon"

static GHashTable * dmp_pb_user_data;
static dmp_pb_ui_status_icons * status_icons = NULL;
static GtkTextBuffer * dmp_pb_console_buffer = NULL;
static gboolean dmp_pb_started = FALSE;


gboolean dmp_pb_ui_is_started(void)
{
	return dmp_pb_started;
}
/* ----------------------------- */
/* Begin static helper functions */
/* ----------------------------- */

/**
 * installs an individual key into the user data hash
 * @param builder the builder to extract from
 * @param widget_name the key to extract and insert
 * @throws DMP_PB_UI_ERROR::UI_DEFINITION_CORRUPT
 */
static void dmp_pb_ui_register_user_data_key(GtkBuilder * builder, gchar * widget_name, GError ** error)
{
	g_hash_table_insert(dmp_pb_user_data, widget_name, GTK_WIDGET(gtk_builder_get_object(builder, widget_name)));
	if (g_hash_table_lookup(dmp_pb_user_data, widget_name) == NULL)
	{
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}
}

/**
 * Fills the user_data struct, and passes it into the GtkBuilder by way of
 * calling gtk_builder_connect_signals
 * @param builder The builder to connect signals to
 * @throws DMP_PB_UI_ERROR::UI_DEFINITION_CORRUPT
 */
static void dmp_pb_ui_register_user_data(GtkBuilder * builder, GError ** error)
{
	g_assert(g_module_supported());
	
	GError * working_error = NULL;
	dmp_pb_user_data = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (GDestroyNotify) gtk_widget_destroy);

	dmp_pb_ui_register_user_data_key(builder, DMP_PB_MAIN_WINDOW, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	dmp_pb_ui_register_user_data_key(builder, DMP_PB_ABOUT_DIALOG, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	dmp_pb_ui_register_user_data_key(builder, DMP_PB_OPTIONS_DIALOG, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	dmp_pb_ui_register_user_data_key(builder, DMP_PB_CAMERA_MODULE_PATH_TEXT_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	dmp_pb_ui_register_user_data_key(builder, DMP_PB_TRIGGER_MODULE_PATH_TEXT_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	dmp_pb_ui_register_user_data_key(builder, DMP_PB_PRINTER_MODULE_PATH_TEXT_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_BACKGROUND_PATH_TEXT_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_INDIVIDUAL_IMAGE_PATH_TEXT_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_COMPLETE_STRIP_PATH_TEXT_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_PHOTO_STRIP_POSITION_1, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_PHOTO_STRIP_POSITION_2, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_PHOTO_STRIP_POSITION_3, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_PHOTO_STRIP_POSITION_4, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_PHOTO_STRIP_POSITION_5, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_INDIVIDUAL_IMAGE_WIDTH_SPIN_BOX, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_IMAGE_HISTORY_VIEW, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_FILE_SUBMENU_START, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_FILE_SUBMENU_STOP, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_TOOLBAR_START_BUTTON, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_TOOLBAR_STOP_BUTTON, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_WORKING_SPLASH_SCREEN, &working_error);
	if (working_error != NULL)
	{
		g_propagate_error(error, working_error);
		return;
	}

	status_icons = g_malloc(sizeof (dmp_pb_ui_status_icons));
	status_icons->printer_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, DMP_PB_PRINTER_MODULE_STATUS_ICON));
	if (status_icons->printer_module_staus_icon == NULL)
	{
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		g_free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	status_icons->trigger_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_trigger_module_status_icon"));
	if (status_icons->trigger_module_staus_icon == NULL)
	{
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		gtk_widget_destroy(GTK_WIDGET(status_icons->printer_module_staus_icon));
		g_free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	status_icons->camera_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, "dmp_pb_camera_module_status_icon"));
	if (status_icons->camera_module_staus_icon == NULL)
	{
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		gtk_widget_destroy(GTK_WIDGET(status_icons->printer_module_staus_icon));
		gtk_widget_destroy(GTK_WIDGET(status_icons->trigger_module_staus_icon));
		g_free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}

	dmp_pb_console_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "dmp_pb_console_buffer"));

	if (dmp_pb_console_buffer == NULL)
	{
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		gtk_widget_destroy(GTK_WIDGET(status_icons->printer_module_staus_icon));
		gtk_widget_destroy(GTK_WIDGET(status_icons->trigger_module_staus_icon));
		gtk_widget_destroy(GTK_WIDGET(status_icons->camera_module_staus_icon));
		g_free(status_icons);
		g_set_error(error,
				dmp_pb_ui_error_quark(),
				UI_DEFINITION_CORRUPT,
				"Failed to load widget from UI definition");
		return;
	}
	
	gtk_builder_connect_signals(builder, (gpointer) dmp_pb_user_data);
}

/**
 * Creates a new GtkBuilder using the passed-in GtkBuilder .xml definition
 * @param builder_file the .xml definition's location
 * @throws GTK_BUILDER_ERROR, G_MARKUP_ERROR, G_FILE_ERROR
 * @return A pointer to a new GtkBuilder, or NULL
 */
static GtkBuilder * dmp_pb_ui_create_gtk_builder(const gchar * builder_file, GError ** error)
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

/**
 * parses the image positions bit field
 * @param to_test the value to parse
 */
static void dmp_pb_ui_parse_image_positions(gint to_test)
{
	if (to_test & DMP_PB_PHOTO_STRIP_POSITION_1_FLAG)
	{
		gtk_toggle_button_set_active
			(
				GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_1)),
				TRUE
			);
	}
	
	if (to_test & DMP_PB_PHOTO_STRIP_POSITION_2_FLAG)
	{
		gtk_toggle_button_set_active
			(
				GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_2)),
				TRUE
			);
	}
	
	if (to_test & DMP_PB_PHOTO_STRIP_POSITION_3_FLAG)
	{
		gtk_toggle_button_set_active
			(
				GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_3)),
				TRUE
			);
	}
	
	if (to_test & DMP_PB_PHOTO_STRIP_POSITION_4_FLAG)
	{
		gtk_toggle_button_set_active
			(
				GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_4)),
				TRUE
			);
	}
	
	if (to_test & DMP_PB_PHOTO_STRIP_POSITION_5_FLAG)
	{
		gtk_toggle_button_set_active
			(
				GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_5)),
				TRUE
			);
	}
}

static void dmp_pb_ui_show_working_screen(void)
{
	gtk_widget_show(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_WORKING_SPLASH_SCREEN));
}

static void dmp_pb_ui_hide_working_screen(void)
{
	gtk_widget_hide(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_WORKING_SPLASH_SCREEN));
}

/**
 * Initializes the options dialog with values read from the config
 */
static void dmp_pb_ui_initialize_options_fields(void)
{
	GString * working = NULL;
	
	dmp_pb_ui_parse_image_positions(dmp_pb_config_read_int(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_POSITION_TOGGLE));
	
	working = dmp_pb_config_read_string(DMP_PB_CONFIG_MODULE_GROUP, DMP_PB_CONFIG_CAMERA_MODULE_PATH);
	gtk_entry_set_text
			(
				GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_CAMERA_MODULE_PATH_TEXT_BOX)),
				working->str
			);
	g_string_free(working, TRUE);
	
	working = dmp_pb_config_read_string(DMP_PB_CONFIG_MODULE_GROUP, DMP_PB_CONFIG_PRINTER_MODULE_PATH);
	gtk_entry_set_text
			(
				GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PRINTER_MODULE_PATH_TEXT_BOX)),
				working->str
			);
	g_string_free(working, TRUE);
	
	working = dmp_pb_config_read_string(DMP_PB_CONFIG_MODULE_GROUP, DMP_PB_CONFIG_TRIGGER_MODULE_PATH);
	gtk_entry_set_text
			(
				GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_TRIGGER_MODULE_PATH_TEXT_BOX)),
				working->str
			);
	g_string_free(working, TRUE);
	
	working = dmp_pb_config_read_string(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_BACKGROUND_PATH);
	gtk_entry_set_text
			(
				GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_BACKGROUND_PATH_TEXT_BOX)),
				working->str
			);
	g_string_free(working, TRUE);
	
	working = dmp_pb_config_read_string(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_PATH);
	gtk_entry_set_text
			(
				GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_IMAGE_PATH_TEXT_BOX)),
				working->str
			);
	g_string_free(working, TRUE);
	
	working = dmp_pb_config_read_string(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_COMPLETED_STRIP_PATH);
	gtk_entry_set_text
			(
				GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_COMPLETE_STRIP_PATH_TEXT_BOX)),
				working->str
			);
	g_string_free(working, TRUE);
	
	gtk_spin_button_set_value
			(
			GTK_SPIN_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_IMAGE_WIDTH_SPIN_BOX)),
			dmp_pb_config_read_double(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_WIDTH)
			);
}

/**
 * Reads the image positions toggles and converts the result to an integer
 * @return said integer
 */
static gint dmp_pb_ui_get_image_positions(void)
{
	gint return_value = 0;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_1))))
	{
		return_value = return_value | DMP_PB_PHOTO_STRIP_POSITION_1_FLAG;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_2))))
	{
		return_value = return_value | DMP_PB_PHOTO_STRIP_POSITION_2_FLAG;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_3))))
	{
		return_value = return_value | DMP_PB_PHOTO_STRIP_POSITION_3_FLAG;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_4))))
	{
		return_value = return_value | DMP_PB_PHOTO_STRIP_POSITION_4_FLAG;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PHOTO_STRIP_POSITION_5))))
	{
		return_value = return_value | DMP_PB_PHOTO_STRIP_POSITION_5_FLAG;
	}
	
	return return_value;
}

/**
 * Commits the options fields to the config
 */
static void dmp_pb_ui_commit_options_fields(void)
{
	GString * working;
	dmp_pb_config_write_int
			(
				DMP_PB_CONFIG_CORE_GROUP, 
				DMP_PB_CONFIG_POSITION_TOGGLE, 
				dmp_pb_ui_get_image_positions()
			);
	
	working = g_string_new(gtk_entry_get_text(GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_BACKGROUND_PATH_TEXT_BOX))));
	dmp_pb_config_write_string
			(
				DMP_PB_CONFIG_CORE_GROUP,
				DMP_PB_CONFIG_BACKGROUND_PATH,
				working
			);
	
	g_string_assign(working, gtk_entry_get_text(GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_IMAGE_PATH_TEXT_BOX))));
	dmp_pb_config_write_string
			(
				DMP_PB_CONFIG_CORE_GROUP,
				DMP_PB_CONFIG_INDIVIDUAL_IMAGE_PATH,
				working
			);
	
	g_string_assign(working, gtk_entry_get_text(GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_COMPLETE_STRIP_PATH_TEXT_BOX))));
	dmp_pb_config_write_string
			(
				DMP_PB_CONFIG_CORE_GROUP,
				DMP_PB_CONFIG_COMPLETED_STRIP_PATH,
				working
			);
	
	g_string_assign(working, gtk_entry_get_text(GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_CAMERA_MODULE_PATH_TEXT_BOX))));
	dmp_pb_config_write_string
			(
				DMP_PB_CONFIG_MODULE_GROUP,
				DMP_PB_CONFIG_CAMERA_MODULE_PATH,
				working
			);
	
	g_string_assign(working, gtk_entry_get_text(GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_TRIGGER_MODULE_PATH_TEXT_BOX))));
	dmp_pb_config_write_string
			(
				DMP_PB_CONFIG_MODULE_GROUP,
				DMP_PB_CONFIG_TRIGGER_MODULE_PATH,
				working
			);
	
	g_string_assign(working, gtk_entry_get_text(GTK_ENTRY(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_PRINTER_MODULE_PATH_TEXT_BOX))));
	dmp_pb_config_write_string
			(
				DMP_PB_CONFIG_MODULE_GROUP,
				DMP_PB_CONFIG_PRINTER_MODULE_PATH,
				working
			);
	
	g_string_free(working, TRUE);
	
	dmp_pb_config_write_double
			(
				DMP_PB_CONFIG_CORE_GROUP,
				DMP_PB_CONFIG_INDIVIDUAL_IMAGE_WIDTH,
				gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_IMAGE_WIDTH_SPIN_BOX)))
			);
}

/**
 * checks for new strips, displays them in the UI if found
 * @param user_data
 * @return 
 */
static gboolean dmp_pb_ui_check_for_strips(gpointer user_data)
{
	GError * error = NULL;
	GString * working = NULL;
	GdkPixbuf * working_pixbuf = dmp_pb_photo_strip_get_result(&working, &error);
	
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_clear_error(&error);
	}
	else if (working_pixbuf == NULL) return G_SOURCE_CONTINUE;
	else if (working != NULL)
	{
		GtkTreeIter iter;
		GtkIconView * icon_view = GTK_ICON_VIEW(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_IMAGE_HISTORY_VIEW));
		GtkListStore * icon_view_store = GTK_LIST_STORE(gtk_icon_view_get_model(icon_view));
		
		gtk_list_store_append(icon_view_store, &iter);
		gtk_list_store_set
				(
					icon_view_store, 
					&iter, 
					0, working_pixbuf,
					1, working->str,
					-1);
		g_string_free(working, TRUE);
		g_object_unref(working_pixbuf);
	}
	return G_SOURCE_CONTINUE;
}
/* --------------------------- */
/* End static helper functions */
/* --------------------------- */

void dmp_pb_ui_launch(const gchar * ui_file, GError ** error)
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
	
	dmp_pb_ui_initialize_options_fields();
	
	g_object_unref(G_OBJECT(builder));

	dmp_pb_mwd_init(status_icons);
	g_timeout_add_seconds(1, dmp_pb_mwd_handle_message, NULL); 
	g_timeout_add_seconds(1, dmp_pb_ui_check_for_strips, NULL);
	g_timeout_add_seconds(1, dmp_pb_console_queue_flush_queue, dmp_pb_console_buffer);

	gtk_widget_show(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_MAIN_WINDOW));
	
	gtk_main();

	dmp_pb_mwd_finalize();
}

dmp_pb_ui_status_icons * dmp_pb_ui_get_status_icons(void)
{
	return status_icons;
}

static void dmp_pb_stop_photo_booth(void)
{	
	if (dmp_pb_coordination_is_processing())
	{
		dmp_pb_console_queue_push(g_string_new("Cannot stop the Photo Booth while a session is in progress!\n"));
		return;
	}
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_unload_module(DMP_PB_TRIGGER_MODULE);
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_unload_module(DMP_PB_PRINTER_MODULE);
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_unload_module(DMP_PB_CAMERA_MODULE);
	
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_FILE_SUBMENU_STOP), FALSE);
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_FILE_SUBMENU_START), TRUE);
	
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_TOOLBAR_START_BUTTON), TRUE);
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_TOOLBAR_STOP_BUTTON), FALSE);
	dmp_pb_started = FALSE;
}

static void dmp_pb_start_photo_booth(void)
{
	GError * error = NULL;	
	GString * location = dmp_pb_config_read_string(DMP_PB_CONFIG_MODULE_GROUP, DMP_PB_CONFIG_PRINTER_MODULE_PATH);
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_load_module(DMP_PB_PRINTER_MODULE, location, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_clear_error(&error);
		dmp_pb_stop_photo_booth();
		g_string_free(location, TRUE);
		return;
	}
	
	DMP_PB_WAIT_FOR_GTK;
	
	g_string_free(location, TRUE);
	location = dmp_pb_config_read_string(DMP_PB_CONFIG_MODULE_GROUP, DMP_PB_CONFIG_CAMERA_MODULE_PATH);
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_load_module(DMP_PB_CAMERA_MODULE, location, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_clear_error(&error);
		dmp_pb_stop_photo_booth();
		g_string_free(location, TRUE);
		return;
	}
	
	g_string_free(location, TRUE);
	location = dmp_pb_config_read_string(DMP_PB_CONFIG_MODULE_GROUP, DMP_PB_CONFIG_TRIGGER_MODULE_PATH);
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_load_module(DMP_PB_TRIGGER_MODULE, location, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_clear_error(&error);
		dmp_pb_stop_photo_booth();
		g_string_free(location, TRUE);
		return;
	}
	
	g_string_free(location, TRUE);
	
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_FILE_SUBMENU_STOP), TRUE);
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_FILE_SUBMENU_START), FALSE);
	
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_TOOLBAR_START_BUTTON), FALSE);
	gtk_widget_set_sensitive(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_TOOLBAR_STOP_BUTTON), TRUE);
	dmp_pb_started = TRUE;
}

/* --------------- */
/* Begin Callbacks */
/* --------------- */

/* ------------------- */
/* File Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_quit_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_destroy(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_MAIN_WINDOW));
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_start_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	g_assert(dmp_pb_ui_is_started() == FALSE);
	
	dmp_pb_ui_show_working_screen();
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_console_queue_push(g_string_new("Starting the Photo Booth...\n"));
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_start_photo_booth();
	
	dmp_pb_ui_hide_working_screen();
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_stop_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	g_assert(dmp_pb_ui_is_started());
	
	dmp_pb_ui_show_working_screen();

	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_console_queue_push(g_string_new("Stopping the Photo Booth...\n"));
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_stop_photo_booth();
	
	dmp_pb_ui_hide_working_screen();
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_print_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	GtkIconView * working_view = g_hash_table_lookup(dmp_pb_user_data, DMP_PB_IMAGE_HISTORY_VIEW);
	GList * working = gtk_icon_view_get_selected_items(working_view);
	if (working == NULL) return;	// Nothing selected
	
	dmp_pb_ui_show_working_screen();
	
	DMP_PB_WAIT_FOR_GTK;
	
	GtkTreeModel * working_tree_model = gtk_icon_view_get_model(working_view);
	GtkTreeIter  iter;
	
	gtk_tree_model_get_iter(working_tree_model,	&iter, working->data);
	
	gchar * path;
	
	gtk_tree_model_get(	working_tree_model,	&iter, 1, &path, -1);
	
	DMP_PB_WAIT_FOR_GTK;
	
	dmp_pb_pm_print(path);
	g_list_foreach(working, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(working);
	
	g_free(path);
	dmp_pb_ui_hide_working_screen();
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_save_output_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	g_assert(dmp_pb_console_buffer != NULL);
	GError * error = NULL;
	
	GtkWidget * file_save = gtk_file_chooser_dialog_new("Save Console output",
			GTK_WINDOW(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_MAIN_WINDOW)),
			GTK_FILE_CHOOSER_ACTION_SAVE,
			"gtk-cancel", GTK_RESPONSE_CANCEL,
			"gtk-save", GTK_RESPONSE_ACCEPT,
			NULL);
	
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_save), TRUE);
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_save), 
					dmp_pb_coordination_get_epoch_filename("console_output", "txt"));
	
	if (gtk_dialog_run(GTK_DIALOG(file_save)) == GTK_RESPONSE_ACCEPT)
	{
		gchar * file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_save));
		GtkTextIter start_iter;
		gtk_text_buffer_get_iter_at_offset(dmp_pb_console_buffer, &start_iter, 0);
		GtkTextIter end_iter;
		gtk_text_buffer_get_iter_at_offset(dmp_pb_console_buffer, &end_iter, -1);
		gchar * console_contents = gtk_text_buffer_get_text(dmp_pb_console_buffer,
													&start_iter, &end_iter, TRUE);
		
		g_file_set_contents(file_name,
				console_contents,
				-1,
				&error);
		
		if (error != NULL)
		{
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
		}
		
		g_free(file_name);
		g_free(console_contents);
	}
	
	gtk_widget_destroy(file_save);
}

/* ------------------- */
/* Edit Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_edit_submenu_preferences_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_OPTIONS_DIALOG));
}

/* ------------------- */
/* Help Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_launch_help_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	GError * error = NULL;
	//TODO: At some point, this will launch actual documentation. Probably
	//		still a webpage somewhere though...
	
	gtk_show_uri(NULL, 
				DMP_PB_DOCUMENTATION_URL,
				GDK_CURRENT_TIME,
				&error);
	
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_clear_error(&error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_about_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	gtk_widget_show(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_ABOUT_DIALOG));
}

/* ---------------------- */
/* About Dialog callbacks */
/* ---------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_about_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	gtk_widget_hide(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_ABOUT_DIALOG));
}

/* ----------------------- */
/* Config Dialog callbacks */
/* ----------------------- */

static void dmp_pb_options_dialog_cancel_button(gpointer user_data)
{
	dmp_pb_ui_initialize_options_fields();
	
	gtk_widget_hide(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_OPTIONS_DIALOG));
}

static void dmp_pb_options_dialog_ok_button(gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_ui_commit_options_fields();
	dmp_pb_config_write(&error);
	
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_clear_error(&error);
	}
	
	dmp_pb_stop_photo_booth();	
	
	gtk_widget_hide(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_OPTIONS_DIALOG));
}

G_MODULE_EXPORT void dmp_pb_options_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	if (response == 2) dmp_pb_options_dialog_cancel_button(user_data);
	else if (response == 1) dmp_pb_options_dialog_ok_button(user_data);
}

/**
 * Calls edit module config on the passed-in type. Brings the GError
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
	dmp_pb_edit_module_config_button_helper(DMP_PB_PRINTER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}