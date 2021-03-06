#include "module.h"

G_DEFINE_QUARK(DMP_PB_MODULE_ERROR, dmp_pb_module_error)

/**
 * The printer module
 */
G_LOCK_DEFINE(printer_module);
static GModule * dmp_pb_printer_module = NULL;
static int (*dmp_pm_print)(const char * to_print) = NULL;
static int (*dmp_pm_edit_config)(void) = NULL;
static int (*dmp_pm_initialize)(void) = NULL;
static int (*dmp_pm_finalize)(void) = NULL;
static int (*dmp_pm_install_console)(int (*c_cb)(const char * message)) = NULL;
static int (*dmp_pm_install_status_handler)(void (*sh)(int status)) = NULL;
static gboolean dmp_pb_printer_module_consistent = TRUE;
G_LOCK_DEFINE(pm_status);
static gboolean dmp_pb_printer_module_status_ok = FALSE;

/**
 * The trigger module
 */
G_LOCK_DEFINE(trigger_module);
static GModule * dmp_pb_trigger_module = NULL;
static int (*dmp_tm_add_trigger_handler)(void (*th)(void)) = NULL;
static int (*dmp_tm_set_countdown)(int current) = NULL;
static int (*dmp_tm_edit_config)(void) = NULL;
static int (*dmp_tm_initialize)(void) = NULL;
static int (*dmp_tm_finalize)(void) = NULL;
static int (*dmp_tm_show_error)(int value) = NULL;
static int (*dmp_tm_install_console)(int (*c_cb)(const char * message)) = NULL;
static int (*dmp_tm_install_status_handler)(void (*sh)(int status)) = NULL;
static gboolean dmp_pb_trigger_module_consistent = TRUE;
G_LOCK_DEFINE(tm_status);
static gboolean dmp_pb_trigger_module_status_ok = FALSE;

/**
 * The camera module
 */
G_LOCK_DEFINE(camera_module);
static GModule * dmp_pb_camera_module = NULL;
static int (*dmp_cm_capture)(const char * location) = NULL;
static int (*dmp_cm_edit_config)(void) = NULL;
static int (*dmp_cm_initialize)(void) = NULL;
static int (*dmp_cm_finalize)(void) = NULL;
static int (*dmp_cm_install_console)(int (*c_cb)(const char * message)) = NULL;
static int (*dmp_cm_install_status_handler)(void (*sh)(int status)) = NULL;
static gboolean dmp_pb_camera_module_consistent = TRUE;
G_LOCK_DEFINE(cm_status);
static gboolean dmp_pb_camera_module_status_ok = FALSE;

static void dmp_pb_set_printer_module_status(gint state)
{
	G_LOCK(pm_status);
	dmp_pb_printer_module_status_ok = !(state == 0);
	G_UNLOCK(pm_status);
}

static void dmp_pb_set_trigger_module_status(gint state)
{
	G_LOCK(tm_status);
	dmp_pb_trigger_module_status_ok = !(state == 0);
	G_UNLOCK(tm_status);
}

static void dmp_pb_set_camera_module_status(gint state)
{
	G_LOCK(cm_status);
	dmp_pb_camera_module_status_ok = !(state == 0);
	G_UNLOCK(cm_status);
}

/**
 * checks to see if a module is initialized
 * @param type the module type
 * @return FALSE if not initialized, TRUE if initialized
 */
static gboolean dmp_pb_status_is_ok(dmp_pb_module_type to_check)
{
	gboolean result = FALSE;
	switch (to_check)
	{
		case DMP_PB_PRINTER_MODULE:
			G_LOCK(pm_status);
			result = dmp_pb_printer_module_status_ok;
			G_UNLOCK(pm_status);
			return result;
		case DMP_PB_TRIGGER_MODULE:
			G_LOCK(tm_status);
			result = dmp_pb_trigger_module_status_ok;
			G_UNLOCK(tm_status);
			return result;
		case DMP_PB_CAMERA_MODULE:
			G_LOCK(cm_status);
			result = dmp_pb_camera_module_status_ok;
			G_UNLOCK(cm_status);
			return result;
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

	gboolean result;
	
	if (to_check == DMP_PB_CAMERA_MODULE)
	{
		G_LOCK(camera_module);
		result = dmp_pb_camera_module_consistent;
		G_UNLOCK(camera_module);
		return result;
	}
	else if (to_check == DMP_PB_TRIGGER_MODULE)
	{
		G_LOCK(trigger_module);
		result = dmp_pb_trigger_module_consistent;
		G_UNLOCK(trigger_module);
		return result;
	}
	else if (to_check == DMP_PB_PRINTER_MODULE) 
	{
		G_LOCK(printer_module);
		result = dmp_pb_printer_module_consistent;
		G_UNLOCK(printer_module);
		return result;
	}
	else return FALSE;
}

/**
 * tests to see if the passed-in module type is loaded
 * @param to_check the module type to test
 * @return true if loaded, false if not loaded or inconsistent
 */
static gboolean dmp_pb_is_loaded(dmp_pb_module_type to_check)
{
	g_assert(g_module_supported());

	gboolean result;
	
	if (to_check == DMP_PB_CAMERA_MODULE)
	{
		G_LOCK(camera_module);
		result = (dmp_pb_camera_module != NULL
			&& dmp_cm_capture != NULL && dmp_cm_edit_config != NULL
			&& dmp_cm_initialize != NULL && dmp_cm_finalize != NULL
			&& dmp_cm_install_console != NULL 
			&& dmp_cm_install_status_handler != NULL);
		G_UNLOCK(camera_module);
		return result;
	}
	
	else if (to_check == DMP_PB_TRIGGER_MODULE)
	{
		G_LOCK(trigger_module);
		result = (dmp_pb_trigger_module != NULL
			&& dmp_tm_add_trigger_handler != NULL && dmp_tm_set_countdown != NULL
			&& dmp_tm_edit_config != NULL && dmp_tm_initialize != NULL
			&& dmp_tm_finalize != NULL && dmp_tm_show_error != NULL
			&& dmp_tm_install_console != NULL
			&& dmp_tm_install_status_handler != NULL);
		G_UNLOCK(trigger_module);
		return result;
	}
	
	else if (to_check == DMP_PB_PRINTER_MODULE)
	{
		G_LOCK(printer_module);
		result = (dmp_pb_printer_module != NULL
			&& dmp_pm_print != NULL && dmp_pm_initialize != NULL
			&& dmp_pm_finalize != NULL && dmp_pm_install_console != NULL
			&& dmp_pm_install_status_handler != NULL);
		G_UNLOCK(printer_module);
		return result;
	}
	else return FALSE;
}

dmp_pb_module_state dmp_pb_check_module_state(dmp_pb_module_type to_check)
{
	if (!dmp_pb_is_consistent(to_check)) return DMP_PB_MODULE_IN_INCONSISTENT_STATE;
	if (!dmp_pb_is_loaded(to_check)) return DMP_PB_MODULE_NOT_LOADED;
	if (!dmp_pb_status_is_ok(to_check)) return DMP_PB_MODULE_BAD_STATUS;
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
			G_LOCK(printer_module);
			(*dmp_pm_install_console)(dmp_pb_console_write_callback);
			(*dmp_pm_install_status_handler)(dmp_pb_set_printer_module_status);
			result = (*dmp_pm_initialize)();
			G_UNLOCK(printer_module);
			return result;
		case DMP_PB_TRIGGER_MODULE:
			G_LOCK(trigger_module);
			(*dmp_tm_install_console)(dmp_pb_console_write_callback);
			(*dmp_tm_install_status_handler)(dmp_pb_set_trigger_module_status);
			(*dmp_tm_add_trigger_handler)(dmp_pb_trigger_handler);
			result = (*dmp_tm_initialize)();
			G_UNLOCK(trigger_module);
			return result;
		case DMP_PB_CAMERA_MODULE:
			G_LOCK(camera_module);
			(*dmp_cm_install_console)(dmp_pb_console_write_callback);
			(*dmp_cm_install_status_handler)(dmp_pb_set_camera_module_status);
			result = (*dmp_cm_initialize)();
			G_UNLOCK(camera_module);
			return result;
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
	gint result = dmp_pb_check_module_state(type);
	if (result == DMP_PB_MODULE_IN_INCONSISTENT_STATE || result == DMP_PB_MODULE_NOT_LOADED) return result;
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			if (dmp_pm_finalize == NULL) return DMP_PB_FAILURE;
			return (*dmp_pm_finalize)();
		case DMP_PB_TRIGGER_MODULE:
			if (dmp_tm_finalize == NULL) return DMP_PB_FAILURE;
			return (*dmp_tm_finalize)();
		case DMP_PB_CAMERA_MODULE:
			if (dmp_cm_finalize == NULL) return DMP_PB_FAILURE;
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
static void dmp_pb_load_printer_module(const GString * module_location, GError ** error)
{
	g_assert(dmp_pb_printer_module_consistent);
	g_assert(g_module_supported());
	
	G_LOCK(printer_module);
	
	if (dmp_pb_printer_module != NULL)
	{
		G_UNLOCK(printer_module);
		return;
	}
		
	dmp_pb_printer_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_printer_module == NULL)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s; %s", module_location->str,
				g_module_error());
		G_UNLOCK(printer_module);
		return;
	}

	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_print", (gpointer *) & dmp_pm_print))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(printer_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_edit_config", (gpointer *) & dmp_pm_edit_config))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(printer_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_initialize", (gpointer *) & dmp_pm_initialize))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(printer_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_install_status_handler", (gpointer *) & dmp_pm_install_status_handler))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(printer_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_finalize", (gpointer *) & dmp_pm_finalize))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(printer_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_install_console", (gpointer *) & dmp_pm_install_console))
	{
		dmp_pb_printer_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(printer_module);
		return;
	}

	dmp_pb_printer_module_consistent = TRUE;
	G_UNLOCK(printer_module);
	
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
static void dmp_pb_load_trigger_module(const GString * module_location, GError ** error)
{
	g_assert(dmp_pb_trigger_module_consistent);
	g_assert(g_module_supported());

	G_LOCK(trigger_module);
	
	if (dmp_pb_trigger_module != NULL)
	{
		G_UNLOCK(trigger_module);
		return;
	}
	
	dmp_pb_trigger_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_trigger_module == NULL)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}

	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_add_trigger_handler", (gpointer *) & dmp_tm_add_trigger_handler))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_set_countdown", (gpointer *) & dmp_tm_set_countdown))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_edit_config", (gpointer *) & dmp_tm_edit_config))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}	
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_initialize", (gpointer *) & dmp_tm_initialize))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_install_status_handler", (gpointer *) & dmp_tm_install_status_handler))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_finalize", (gpointer *) & dmp_tm_finalize))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}
	
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_show_error", (gpointer *) & dmp_tm_show_error))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_install_console", (gpointer *) & dmp_tm_install_console))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(trigger_module);
		return;
	}

	dmp_pb_trigger_module_consistent = TRUE;
	G_UNLOCK(trigger_module);
	
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
static void dmp_pb_load_camera_module(const GString * module_location, GError ** error)
{
	g_assert(dmp_pb_camera_module_consistent);
	g_assert(g_module_supported());

	if (dmp_pb_camera_module != NULL) 
	{
		G_UNLOCK(camera_module);
		return;
	}
	
	dmp_pb_camera_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_camera_module == NULL)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}

	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_capture", (gpointer *) & dmp_cm_capture))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_edit_config", (gpointer *) & dmp_cm_edit_config))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_initialize", (gpointer *) & dmp_cm_initialize))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_install_status_handler", (gpointer *) & dmp_cm_install_status_handler))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_finalize", (gpointer *) & dmp_cm_finalize))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_install_console", (gpointer *) & dmp_cm_install_console))
	{
		dmp_pb_camera_module_consistent = FALSE;
		g_set_error(error,
				dmp_pb_module_error_quark(),
				G_MODULE_LOAD_FAILURE,
				"Failed to load module at: %s", module_location->str);
		G_UNLOCK(camera_module);
		return;
	}

	dmp_pb_camera_module_consistent = TRUE;
	G_UNLOCK(camera_module);
	
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

void dmp_pb_load_module(dmp_pb_module_type to_load, const GString * module_location, GError ** error)
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
static void dmp_pb_unload_printer_module(void)
{
	g_assert(g_module_supported());
	G_LOCK(printer_module);
	if (dmp_pm_finalize != NULL)	// TODO: Stinky
	{
		G_UNLOCK(printer_module);
		dmp_pb_finalize(DMP_PB_PRINTER_MODULE);
		G_LOCK(printer_module);
	}
	if (dmp_pb_printer_module != NULL) g_module_close(dmp_pb_printer_module);

	dmp_pb_printer_module = NULL;
	dmp_pm_print = NULL;
	dmp_pm_edit_config = NULL;
	dmp_pm_finalize = NULL;
	dmp_pm_initialize = NULL;
	dmp_pm_install_status_handler = NULL;

	dmp_pb_printer_module_consistent = TRUE;
	G_UNLOCK(printer_module);
}

/**
 * unloads the trigger module
 */
static void dmp_pb_unload_trigger_module(void)
{
	g_assert(g_module_supported());
	G_LOCK(trigger_module);
	if (dmp_tm_finalize != NULL)	// TODO: stinky
	{
		G_UNLOCK(trigger_module);
		dmp_pb_finalize(DMP_PB_TRIGGER_MODULE);
		G_LOCK(trigger_module);
	}
	if (dmp_pb_trigger_module != NULL) g_module_close(dmp_pb_trigger_module);

	dmp_pb_trigger_module = NULL;
	dmp_tm_add_trigger_handler = NULL;
	dmp_tm_set_countdown = NULL;
	dmp_tm_edit_config = NULL;
	dmp_tm_finalize = NULL;
	dmp_tm_show_error = NULL;
	dmp_tm_initialize = NULL;
	dmp_tm_install_status_handler = NULL;

	dmp_pb_trigger_module_consistent = TRUE;
	G_UNLOCK(trigger_module);
}

/**
 * unloads the camera module
 * @return DMP_PB_SUCCESS, or an error code
 */
static void dmp_pb_unload_camera_module(void)
{
	g_assert(g_module_supported());
	G_LOCK(camera_module);
	if (dmp_cm_finalize != NULL)	// TODO: stinky
	{
		G_UNLOCK(camera_module);
		dmp_pb_finalize(DMP_PB_CAMERA_MODULE);
		G_LOCK(camera_module);
	}
	if (dmp_pb_camera_module != NULL) g_module_close(dmp_pb_camera_module);

	dmp_pb_camera_module = NULL;
	dmp_cm_capture = NULL;
	dmp_cm_edit_config = NULL;
	dmp_cm_finalize = NULL;
	dmp_cm_initialize = NULL;
	dmp_cm_install_status_handler = NULL;

	dmp_pb_camera_module_consistent = TRUE;
	G_UNLOCK(camera_module);
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

gint dmp_pb_edit_module_config(dmp_pb_module_type type, GError ** error)
{
	dmp_pb_module_type module_type = type;
	dmp_pb_module_state state = dmp_pb_check_module_state(module_type);
	dmp_pb_module_error error_type;
	gint result;
	
	if (state != DMP_PB_MODULE_READY)
	{
		if (module_type == DMP_PB_CAMERA_MODULE) error_type = CAMERA_MODULE_FUNCTION_ERROR;
		else if (module_type == DMP_PB_TRIGGER_MODULE) error_type = TRIGGER_MODULE_FUNCTION_ERROR;
		else if (module_type == DMP_PB_PRINTER_MODULE) error_type = PRINTER_MODULE_FUNCTION_ERROR;
		else g_assert_not_reached();
		
		g_set_error(error,
				dmp_pb_module_error_quark(),
				error_type,
				"Module State is: %d, where it should be %d", state, 
				DMP_PB_MODULE_READY);
		return DMP_PB_FAILURE;
	}
	
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			G_LOCK(printer_module);
			result = (*dmp_pm_edit_config)();
			G_UNLOCK(printer_module);
			return result;
		case DMP_PB_TRIGGER_MODULE:
			G_LOCK(trigger_module);
			result = (*dmp_tm_edit_config)();
			G_UNLOCK(trigger_module);
			return result;
		case DMP_PB_CAMERA_MODULE:
			G_LOCK(camera_module);
			result = (*dmp_cm_edit_config)();
			G_UNLOCK(camera_module);
			return result;
		default:
			g_assert_not_reached();
	}
}

void dmp_pb_swap_module(dmp_pb_module_type to_swap, const GString * new_module_location, GError ** error)
{
	GError * working_error = NULL;
	
	dmp_pb_unload_module(to_swap);
	dmp_pb_load_module(to_swap, new_module_location, &working_error);
	
	if (working_error != NULL) g_propagate_error(error, working_error);
}



gint dmp_pb_cm_capture(const gchar * location, GError ** error)
{
	gint result;
	dmp_pb_module_state state = dmp_pb_check_module_state(DMP_PB_CAMERA_MODULE);
	G_LOCK(camera_module);
	if (state != DMP_PB_MODULE_READY)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				CAMERA_MODULE_FUNCTION_ERROR,
				"Module State is: %d, where it should be %d", state, 
				DMP_PB_MODULE_READY);
		G_UNLOCK(camera_module);
		return DMP_PB_FAILURE;
	}
	result = (*dmp_cm_capture)(location);
	G_UNLOCK(camera_module);
	return result;
}

gint dmp_pb_pm_print(const gchar * to_print, GError ** error)
{
	gint result;
	dmp_pb_module_state state = dmp_pb_check_module_state(DMP_PB_PRINTER_MODULE);
	G_LOCK(printer_module);
	if (state != DMP_PB_MODULE_READY)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				PRINTER_MODULE_FUNCTION_ERROR,
				"Module State is: %d, where it should be %d", state, 
				DMP_PB_MODULE_READY);
		G_UNLOCK(printer_module);
		return DMP_PB_FAILURE;
	}
	result = (*dmp_pm_print)(to_print);
	G_UNLOCK(printer_module);
	return result;
}

gint dmp_pb_tm_set_countdown(int current, GError ** error)
{
	gint result;
	dmp_pb_module_state state = dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE);
	G_LOCK(trigger_module);
	if (state != DMP_PB_MODULE_READY)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				TRIGGER_MODULE_FUNCTION_ERROR,
				"Module State is: %d, where it should be %d", state, 
				DMP_PB_MODULE_READY);
		G_UNLOCK(trigger_module);
		return DMP_PB_FAILURE;
	}
	result = (*dmp_tm_set_countdown)(current);
	G_UNLOCK(trigger_module);
	return result;
}

gint dmp_pb_tm_show_error(gint value, GError ** error)
{
	gint result;
	dmp_pb_module_state state = dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE);
	G_LOCK(trigger_module);
	if (state != DMP_PB_MODULE_READY)
	{
		g_set_error(error,
				dmp_pb_module_error_quark(),
				TRIGGER_MODULE_FUNCTION_ERROR,
				"Module State is: %d, where it should be %d", state, 
				DMP_PB_MODULE_READY);
		G_UNLOCK(trigger_module);
		return DMP_PB_FAILURE;
	}
	result = (*dmp_tm_show_error)(value);
	G_UNLOCK(trigger_module);
	return result;
}