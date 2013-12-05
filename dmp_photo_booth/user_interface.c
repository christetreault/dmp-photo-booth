#include "user_interface.h"
#include "configuration.h"
#include "photo_strip.h"

G_DEFINE_QUARK(DMP_PB_UI_ERROR, dmp_pb_ui_error)

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
#define DMP_PB_PHOTO_STRIP_POSITION_1_FLAG 1 << 0
#define DMP_PB_PHOTO_STRIP_POSITION_2 "dmp_pb_photo_strip_position_2"
#define DMP_PB_PHOTO_STRIP_POSITION_2_FLAG 1 << 1
#define DMP_PB_PHOTO_STRIP_POSITION_3 "dmp_pb_photo_strip_position_3"
#define DMP_PB_PHOTO_STRIP_POSITION_3_FLAG 1 << 2
#define DMP_PB_PHOTO_STRIP_POSITION_4 "dmp_pb_photo_strip_position_4"
#define DMP_PB_PHOTO_STRIP_POSITION_4_FLAG 1 << 3
#define DMP_PB_PHOTO_STRIP_POSITION_5 "dmp_pb_photo_strip_position_5"
#define DMP_PB_PHOTO_STRIP_POSITION_5_FLAG 1 << 4

#define DMP_PB_INDIVIDUAL_ASPECT_RATIO_COMBO_BOX "dmp_pb_individual_aspect_ratio_combo_box"
#define DMP_PB_INDIVIDUAL_IMAGE_WIDTH_SPIN_BOX "dmp_pb_individual_image_width_spin_box"

#define DMP_PB_IMAGE_HISTORY_VIEW "dmp_pb_image_history_view"

/* Status Icons */

#define DMP_PB_PRINTER_MODULE_STATUS_ICON "dmp_pb_printer_module_status_icon"
#define DMP_PB_TRIGGER_MODULE_STATUS_ICON "dmp_pb_trigger_module_status_icon"
#define DMP_PB_CAMERA_MODULE_STATUS_ICON "dmp_pb_camera_module_status_icon"

static GHashTable * dmp_pb_user_data;
static dmp_pb_ui_status_icons * status_icons = NULL;
static GtkTextBuffer * dmp_pb_console_buffer = NULL;

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
	
	dmp_pb_ui_register_user_data_key(builder, DMP_PB_INDIVIDUAL_ASPECT_RATIO_COMBO_BOX, &working_error);
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

	status_icons = malloc(sizeof (dmp_pb_ui_status_icons));
	status_icons->printer_module_staus_icon = GTK_IMAGE(gtk_builder_get_object(builder, DMP_PB_PRINTER_MODULE_STATUS_ICON));
	if (status_icons->printer_module_staus_icon == NULL)
	{
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
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
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		gtk_widget_destroy(GTK_WIDGET(status_icons->printer_module_staus_icon));
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
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		gtk_widget_destroy(GTK_WIDGET(status_icons->printer_module_staus_icon));
		gtk_widget_destroy(GTK_WIDGET(status_icons->trigger_module_staus_icon));
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
		g_hash_table_destroy(dmp_pb_user_data);
		dmp_pb_user_data = NULL;
		gtk_widget_destroy(GTK_WIDGET(status_icons->printer_module_staus_icon));
		gtk_widget_destroy(GTK_WIDGET(status_icons->trigger_module_staus_icon));
		gtk_widget_destroy(GTK_WIDGET(status_icons->camera_module_staus_icon));
		free(status_icons);
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

/**
 * parses an aspect ratio, doing the correct configuration
 * @param to_test the aspect ratio in question
 */
static void dmp_pb_ui_parse_aspect_ratio(gdouble to_test)
{
	if (to_test == DMP_PB_ASPECT_RATIO_4_3)
	{
		gtk_combo_box_set_active
				(
					GTK_COMBO_BOX(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_ASPECT_RATIO_COMBO_BOX)),
					0
				);
	}
	if (to_test == DMP_PB_ASPECT_RATIO_3_2)
	{
		gtk_combo_box_set_active
				(
					GTK_COMBO_BOX(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_ASPECT_RATIO_COMBO_BOX)),
					1
				);
		return;
	}
	else	/* Default action */ 
	{
		gtk_combo_box_set_active
				(
					GTK_COMBO_BOX(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_ASPECT_RATIO_COMBO_BOX)),
					0
				);
		return;
	}
}

/**
 * Initializes the options dialog with values read from the config
 */
static void dmp_pb_ui_initialize_options_fields()
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
	
	dmp_pb_ui_parse_aspect_ratio(dmp_pb_config_read_double(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_ASPECT_RATIO));
	
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
static gint dmp_pb_ui_get_image_positions()
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
 * Reads the aspect ratio combo box selection id and converts it to an aspect
 * ratio
 * @return said aspect ratio
 */
static gdouble dmp_pb_ui_get_aspect_ratio()
{
	gint working = gtk_combo_box_get_active(GTK_COMBO_BOX(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_INDIVIDUAL_ASPECT_RATIO_COMBO_BOX)));
	switch (working)
	{
		case 0:
			return DMP_PB_ASPECT_RATIO_4_3;
		case 1:
			return DMP_PB_ASPECT_RATIO_3_2;
		default:
			return DMP_PB_ASPECT_RATIO_4_3;
	}
}

/**
 * Commits the options fields to the config
 */
static void dmp_pb_ui_commit_options_fields()
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
				DMP_PB_CONFIG_INDIVIDUAL_IMAGE_ASPECT_RATIO,
				dmp_pb_ui_get_aspect_ratio()
			);
	
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
					1, g_string_free(working, FALSE),
					-1);
		if (error != NULL)
		{
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
		}
	}
	return G_SOURCE_CONTINUE;
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
	
	dmp_pb_ui_initialize_options_fields();
	
	/* 
	 * While this probably violates some best practice, I'm going to do it 
	 * anyways because I cannot be bothered to re-implement these functions and 
	 * they work 
	 */
	dmp_pb_ui_cb_load_camera_module_button_clicked(NULL, dmp_pb_user_data);
	dmp_pb_ui_cb_load_printer_module_button_clicked(NULL, dmp_pb_user_data);
	dmp_pb_ui_cb_load_trigger_module_button_clicked(NULL, dmp_pb_user_data);
	
	/*
	 * End of best practices violations
	 */
	
	g_object_unref(G_OBJECT(builder));

	dmp_pb_mwd_init(status_icons);
	g_idle_add(dmp_pb_mwd_handle_message, NULL);
	g_idle_add(dmp_pb_ui_check_for_strips, NULL);
	g_timeout_add_seconds(1, dmp_pb_console_queue_flush_queue, dmp_pb_console_buffer);

	gtk_widget_show(g_hash_table_lookup(dmp_pb_user_data, DMP_PB_MAIN_WINDOW));
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
	
	gtk_widget_destroy(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_MAIN_WINDOW));
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_start_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	dmp_pb_console_queue_push(g_string_new("Starting the Photo Booth...\n"));
	//TODO: Something else should happen now
	
	//TODO: For testing
	dmp_pb_photo_strip_request(g_string_new("/tmp/test.png"),
			g_string_new("/tmp/1.JPG"),
			g_string_new("/tmp/2.JPG"),
			//g_string_new("/tmp/3.JPG"),
			NULL,
			g_string_new("/tmp/4.JPG"),
			g_string_new("/tmp/5.JPG"));
}

G_MODULE_EXPORT void dmp_pb_ui_cb_file_submenu_stop_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	dmp_pb_console_queue_push(g_string_new("Stopping the Photo Booth...\n"));
	//TODO: Something else should happen now
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
	gtk_widget_show(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_OPTIONS_DIALOG));
}

/* ------------------- */
/* Help Menu callbacks */
/* ------------------- */

G_MODULE_EXPORT void dmp_pb_ui_cb_help_submenu_launch_help_activate(GtkMenuItem * menuitem, gpointer user_data)
{

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
	
	/* 
	 * While this probably violates some best practice, I'm going to do it 
	 * anyways because I cannot be bothered to re-implement these functions and 
	 * they work 
	 */
	dmp_pb_ui_cb_load_camera_module_button_clicked(NULL, dmp_pb_user_data);
	dmp_pb_ui_cb_load_printer_module_button_clicked(NULL, dmp_pb_user_data);
	dmp_pb_ui_cb_load_trigger_module_button_clicked(NULL, dmp_pb_user_data);
	
	/*
	 * End of best practices violations
	 */
	
	
	gtk_widget_hide(g_hash_table_lookup((GHashTable *)dmp_pb_user_data, DMP_PB_OPTIONS_DIALOG));
}

G_MODULE_EXPORT void dmp_pb_options_dialog_response(GtkDialog * about, gint response, gpointer user_data)
{
	if (response == 2) dmp_pb_options_dialog_cancel_button(user_data);
	else if (response == 1) dmp_pb_options_dialog_ok_button(user_data);
}

/**
 * Helper for load module callbacks. Loads the specified module type, based on 
 * the button pushed
 * @param user_data the user_data struct to use
 * @param type the module type to load
 * @throws DMP_PB_MODULE_ERROR::G_MODULE_LOAD_FAILURE
 */
static void dmp_pb_ui_load_module_button_helper(GHashTable * user_data, dmp_pb_module_type type, GError ** error)
{
	GString * path;
	GtkEntry * current;
	GError * working_error = NULL;
	
	switch (type)
	{
		case DMP_PB_CAMERA_MODULE:
			current = GTK_ENTRY(g_hash_table_lookup(user_data, DMP_PB_CAMERA_MODULE_PATH_TEXT_BOX));
			break;
		case DMP_PB_TRIGGER_MODULE:
			current = GTK_ENTRY(g_hash_table_lookup(user_data, DMP_PB_TRIGGER_MODULE_PATH_TEXT_BOX));
			break;
		case DMP_PB_PRINTER_MODULE:
			current = GTK_ENTRY(g_hash_table_lookup(user_data, DMP_PB_PRINTER_MODULE_PATH_TEXT_BOX));
			break;
	}
	path = g_string_new(gtk_entry_get_text(current));
	dmp_pb_swap_module(type, path, &working_error);

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
	dmp_pb_ui_load_module_button_helper((GHashTable *) user_data, DMP_PB_CAMERA_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_trigger_module_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_ui_load_module_button_helper((GHashTable *) user_data, DMP_PB_TRIGGER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}

G_MODULE_EXPORT void dmp_pb_ui_cb_load_printer_module_button_clicked(GtkButton * button, gpointer user_data)
{
	GError * error = NULL;
	dmp_pb_ui_load_module_button_helper((GHashTable *) user_data, DMP_PB_PRINTER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
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
	dmp_pb_edit_module_config_button_helper(DMP_PB_TRIGGER_MODULE, &error);
	if (error != NULL)
	{
		dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
		g_error_free(error);
	}
}