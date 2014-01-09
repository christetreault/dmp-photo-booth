#include "module.h"

G_DEFINE_QUARK(DMP_PB_MODULE_ERROR, dmp_pb_module_error)

/**
 * The printer module
 */
static GModule * dmp_pb_printer_module = NULL;
static int (*dmp_pm_print)(char * to_print) = NULL;
static int (*dmp_pm_edit_config)() = NULL;
static int (*dmp_pm_load_config)() = NULL;
static int (*dmp_pm_initialize)() = NULL;
static int (*dmp_pm_is_initialized)() = NULL;
static int (*dmp_pm_finalize)() = NULL;
static int (*dmp_pm_install_console)(int (*c_cb)(char * message)) = NULL;
static gboolean dmp_pb_printer_module_consistent = TRUE;

/**
 * The trigger module
 */
static GModule * dmp_pb_trigger_module = NULL;
static int (*dmp_tm_add_trigger_handler)(void (*th)()) = NULL;
static int (*dmp_tm_set_countdown)(int current) = NULL;
static int (*dmp_tm_edit_config)() = NULL;
static int (*dmp_tm_load_config)() = NULL;
static int (*dmp_tm_initialize)() = NULL;
static int (*dmp_tm_is_initialized)() = NULL;
static int (*dmp_tm_finalize)() = NULL;
static int (*dmp_tm_show_error)(int value) = NULL;
static int (*dmp_tm_install_console)(int (*c_cb)(char * message)) = NULL;
static gboolean dmp_pb_trigger_module_consistent = TRUE;

/**
 * The camera module
 */
static GModule * dmp_pb_camera_module = NULL;
static int (*dmp_cm_capture)(char * location) = NULL;
static int (*dmp_cm_edit_config)() = NULL;
static int (*dmp_cm_load_config)() = NULL;
static int (*dmp_cm_initialize)() = NULL;
static int (*dmp_cm_is_initialized)() = NULL;
static int (*dmp_cm_finalize)() = NULL;
static int (*dmp_cm_install_console)(int (*c_cb)(char * message)) = NULL;
static gboolean dmp_pb_camera_module_consistent = TRUE;

/**
 * checks to see if a module is initialized
 * @param type the module type
 * @return FALSE if not initialized, TRUE if initialized
 */
static gboolean dmp_pb_is_initialized(dmp_pb_module_type to_check)
{
	switch (to_check)
	{
		case DMP_PB_PRINTER_MODULE:
			if (dmp_pm_is_initialized == NULL) return FALSE;
			return (*dmp_pm_is_initialized)();
		case DMP_PB_TRIGGER_MODULE:
			if (dmp_tm_is_initialized == NULL) return FALSE;
			return (*dmp_tm_is_initialized)();
		case DMP_PB_CAMERA_MODULE:
			if (dmp_cm_is_initialized == NULL) return FALSE;
			return (*dmp_cm_is_initialized)();
		default:
			g_assert_not_reached();
	}
}

/**
 * tests to see if the passed-in module type is consistent
 * @param to_check the module type to check
 * @return the value of the module's consistency flag
 */
static gboolean dmp_pb_is_consistent(dmp_pb_module_type to_check)
{
	g_assert(g_module_supported());

	if (to_check == DMP_PB_CAMERA_MODULE) return dmp_pb_camera_module_consistent;
	else if (to_check == DMP_PB_TRIGGER_MODULE) return dmp_pb_trigger_module_consistent;
	else if (to_check == DMP_PB_PRINTER_MODULE) return dmp_pb_printer_module_consistent;
	else return FALSE;
}

/**
 * tests to see if the passed-in module type is loaded
 * @param to_check the module type to test
 * @return true if loaded, false if not loaded or inconsistent
 */
static gboolean dmp_pb_is_loaded(dmp_pb_module_type to_check)	//TODO: this
{
	g_assert(g_module_supported());

	if (to_check == DMP_PB_CAMERA_MODULE) return (dmp_pb_camera_module != NULL
			&& dmp_cm_capture != NULL);
	else if (to_check == DMP_PB_TRIGGER_MODULE) return (dmp_pb_trigger_module != NULL
			&& dmp_tm_add_trigger_handler != NULL && dmp_tm_set_countdown != NULL);
	else if (to_check == DMP_PB_PRINTER_MODULE) return (dmp_pb_printer_module != NULL
			&& dmp_pm_print != NULL);
	else return FALSE;
}

dmp_pb_module_state dmp_pb_check_module_state(dmp_pb_module_type to_check)
{
	if (!dmp_pb_is_consistent(to_check)) return DMP_PB_MODULE_IN_INCONSISTENT_STATE;
	if (!dmp_pb_is_loaded(to_check)) return DMP_PB_MODULE_NOT_LOADED;
	if (!dmp_pb_is_initialized(to_check)) return DMP_PB_MODULE_NOT_INITIALIZED;
	return DMP_PB_MODULE_READY;
}

/**
 * initializes a module
 * @param type the module type
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_initialize(dmp_pb_module_type type)
{
	gint result = dmp_pb_check_module_state(type);
	if (result == DMP_PB_MODULE_IN_INCONSISTENT_STATE) return result;
	if (result == DMP_PB_MODULE_NOT_LOADED) return result;
	
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			(*dmp_pm_install_console)(dmp_pb_console_write_callback);
			return (*dmp_pm_initialize)();
		case DMP_PB_TRIGGER_MODULE:
			(*dmp_tm_install_console)(dmp_pb_console_write_callback);
			(*dmp_tm_add_trigger_handler)(dmp_pb_trigger_handler);
			return (*dmp_tm_initialize)();
		case DMP_PB_CAMERA_MODULE:
			(*dmp_cm_install_console)(dmp_pb_console_write_callback);
			return (*dmp_cm_initialize)();
		default:
			g_assert_not_reached();
	}
}

/**
 * finalizes a module
 * @param type the module type
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_finalize(dmp_pb_module_type type)
{
	gint result;
	if (result = dmp_pb_check_module_state(type) != DMP_PB_MODULE_READY) return result;
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			return (*dmp_pm_finalize)();
		case DMP_PB_TRIGGER_MODULE:
			return (*dmp_tm_finalize)();
		case DMP_PB_CAMERA_MODULE:
			return (*dmp_cm_finalize)();
		default:
			g_assert_not_reached();
	}
}

/* dmp_pb_load_module helper functions */

/**
 * Loads the printer module
 * @param module_location the location of the printer module
 * @return DMP_PB_SUCCESS, or an error code
 */
static void dmp_pb_load_printer_module(GString * module_location, GError ** error)
{
	g_assert(dmp_pb_printer_module_consistent);
	g_assert(g_module_supported());
	
	if (dmp_pb_printer_module != NULL) return;
	
	dmp_pb_printer_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_printer_module == NULL)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}

	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_print", (gpointer *) & dmp_pm_print))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_edit_config", (gpointer *) & dmp_pm_edit_config))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_load_config", (gpointer *) & dmp_pm_load_config))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_initialize", (gpointer *) & dmp_pm_initialize))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_is_initialized", (gpointer *) & dmp_pm_is_initialized))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_finalize", (gpointer *) & dmp_pm_finalize))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_install_console", (gpointer *) & dmp_pm_install_console))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}

	dmp_pb_printer_module_consistent = TRUE;
	
	if (dmp_pb_initialize(DMP_PB_PRINTER_MODULE) != DMP_PB_SUCCESS)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	
	return;
}

/**
 * Loads the trigger module
 * @param module_location the location of the trigger module
 * @return DMP_PB_SUCCESS, or an error code
 */
static void dmp_pb_load_trigger_module(GString * module_location, GError ** error)
{
	g_assert(dmp_pb_trigger_module_consistent);
	g_assert(g_module_supported());

	if (dmp_pb_trigger_module != NULL) return;
	dmp_pb_trigger_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_trigger_module == NULL)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}

	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_add_trigger_handler", (gpointer *) & dmp_tm_add_trigger_handler))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_set_countdown", (gpointer *) & dmp_tm_set_countdown))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_edit_config", (gpointer *) & dmp_tm_edit_config))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_load_config", (gpointer *) & dmp_tm_load_config))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}	
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_initialize", (gpointer *) & dmp_tm_initialize))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_is_initialized", (gpointer *) & dmp_tm_is_initialized))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_finalize", (gpointer *) & dmp_tm_finalize))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_show_error", (gpointer *) & dmp_tm_show_error))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_install_console", (gpointer *) & dmp_tm_install_console))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	

	dmp_pb_trigger_module_consistent = TRUE;
	
	if (dmp_pb_initialize(DMP_PB_TRIGGER_MODULE) != DMP_PB_SUCCESS)
	{
			g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	
	return;
}

/**
 * Loads the camera module
 * @param module_location the location of the camera module
 * @return DMP_PB_SUCCESS, or an error code
 */
static void dmp_pb_load_camera_module(GString * module_location, GError ** error)
{
	g_assert(dmp_pb_camera_module_consistent);
	g_assert(g_module_supported());

	if (dmp_pb_camera_module != NULL) return;
	dmp_pb_camera_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_camera_module == NULL)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}

	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_capture", (gpointer *) & dmp_cm_capture))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_edit_config", (gpointer *) & dmp_cm_edit_config))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_load_config", (gpointer *) & dmp_cm_load_config))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_initialize", (gpointer *) & dmp_cm_initialize))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_is_initialized", (gpointer *) & dmp_cm_is_initialized))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_finalize", (gpointer *) & dmp_cm_finalize))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_install_console", (gpointer *) & dmp_cm_install_console))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}

	dmp_pb_camera_module_consistent = TRUE;
	
	if (dmp_pb_initialize(DMP_PB_CAMERA_MODULE) != DMP_PB_SUCCESS)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		return;
	}
	
	return;
}

/* end of dmp_pb_load_module helper functions */

void dmp_pb_load_module(dmp_pb_module_type to_load, GString * module_location, GError ** error)
{
	GError * working_error = NULL;
	switch (to_load)
	{
		case DMP_PB_PRINTER_MODULE:
			dmp_pb_load_printer_module(module_location, &working_error);
			break;
		case DMP_PB_TRIGGER_MODULE:
			dmp_pb_load_trigger_module(module_location, &working_error);
			break;
		case DMP_PB_CAMERA_MODULE:
			dmp_pb_load_camera_module(module_location, &working_error);
			break;
		default:
			g_assert_not_reached();
	}
	if (working_error != NULL) g_propagate_error(error, working_error);
}

/* dmp_pb_unload_module helper functions */

/**
 * unloads the printer module
 */
static void dmp_pb_unload_printer_module()
{
	g_assert(g_module_supported());
	if (dmp_pm_finalize != NULL) dmp_pb_finalize(DMP_PB_PRINTER_MODULE);
	if (dmp_pb_printer_module != NULL) g_module_close(dmp_pb_printer_module);

	dmp_pb_printer_module = NULL;
	dmp_pm_print = NULL;
	dmp_pm_edit_config = NULL;
	dmp_pm_load_config = NULL;
	dmp_pm_finalize = NULL;
	dmp_pm_initialize = NULL;
	dmp_pm_is_initialized = NULL;

	dmp_pb_printer_module_consistent = TRUE;

}

/**
 * unloads the trigger module
 */
static void dmp_pb_unload_trigger_module()
{
	g_assert(g_module_supported());
	if (dmp_tm_finalize != NULL) dmp_pb_finalize(DMP_PB_TRIGGER_MODULE);
	if (dmp_pb_trigger_module != NULL) g_module_close(dmp_pb_trigger_module);

	dmp_pb_trigger_module = NULL;
	dmp_tm_add_trigger_handler = NULL;
	dmp_tm_set_countdown = NULL;
	dmp_tm_edit_config = NULL;
	dmp_tm_load_config = NULL;
	dmp_tm_finalize = NULL;
	dmp_tm_show_error = NULL;
	dmp_tm_initialize = NULL;
	dmp_tm_is_initialized = NULL;

	dmp_pb_trigger_module_consistent = TRUE;
}

/**
 * unloads the camera module
 * @return DMP_PB_SUCCESS, or an error code
 */
static void dmp_pb_unload_camera_module()
{
	g_assert(g_module_supported());
	if (dmp_cm_finalize != NULL) dmp_pb_finalize(DMP_PB_CAMERA_MODULE);
	if (dmp_pb_camera_module != NULL) g_module_close(dmp_pb_camera_module);

	dmp_pb_camera_module = NULL;
	dmp_cm_capture = NULL;
	dmp_cm_edit_config = NULL;
	dmp_cm_load_config = NULL;
	dmp_cm_finalize = NULL;
	dmp_cm_initialize = NULL;
	dmp_cm_is_initialized = NULL;

	dmp_pb_camera_module_consistent = TRUE;
}

/* end of dmp_pb_unload_module helper functions */

void dmp_pb_unload_module(dmp_pb_module_type to_unload)
{
	switch (to_unload)
	{
		case DMP_PB_PRINTER_MODULE:
			dmp_pb_unload_printer_module();
			break;
		case DMP_PB_TRIGGER_MODULE:
			dmp_pb_unload_trigger_module();
			break;
		case DMP_PB_CAMERA_MODULE:
			dmp_pb_unload_camera_module();
			break;
		default:
			g_assert_not_reached();
	}
	
}

gint dmp_pb_edit_module_config(dmp_pb_module_type type)
{
	g_assert(dmp_pb_check_module_state(type) == DMP_PB_MODULE_READY);
	
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			return (*dmp_pm_edit_config)();
		case DMP_PB_TRIGGER_MODULE:
			return (*dmp_tm_edit_config)();
		case DMP_PB_CAMERA_MODULE:
			return (*dmp_cm_edit_config)();
		default:
			g_assert_not_reached();
	}
}

gint dmp_pb_load_module_config(dmp_pb_module_type type)
{
	g_assert(dmp_pb_check_module_state(type) == DMP_PB_MODULE_READY);

	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			return (*dmp_pm_load_config)();
		case DMP_PB_TRIGGER_MODULE:
			return (*dmp_tm_load_config)();
		case DMP_PB_CAMERA_MODULE:
			return (*dmp_cm_load_config)();
		default:
			g_assert_not_reached();
	}
}

void dmp_pb_swap_module(dmp_pb_module_type to_swap, GString * new_module_location, GError ** error)
{
	GError * working_error = NULL;
	
	dmp_pb_unload_module(to_swap);
	dmp_pb_load_module(to_swap, new_module_location, &working_error);
	
	if (working_error != NULL) g_propagate_error(error, working_error);
}



gint dmp_pb_cm_capture(gchar * location)
{
	g_assert(dmp_pb_check_module_state(DMP_PB_CAMERA_MODULE) == DMP_PB_MODULE_READY);
	return (*dmp_cm_capture)(location);
}

gint dmp_pb_pm_print(gchar * to_print)
{
	g_assert(dmp_pb_check_module_state(DMP_PB_PRINTER_MODULE) == DMP_PB_MODULE_READY);
	return (*dmp_pm_print)(to_print);
}

gint dmp_pb_tm_set_countdown(int current)
{
	g_assert(dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE) == DMP_PB_MODULE_READY);
	return (*dmp_tm_set_countdown)(current);
}

gint dmp_pb_tm_show_error(gint value)
{
	g_assert(dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE) == DMP_PB_MODULE_READY);
	return (*dmp_tm_show_error)(value);
}