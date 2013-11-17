#include "module.h"

/**
 * The printer module
 */
static GModule * dmp_pb_printer_module = NULL;
static int (*dmp_pm_print)(char * to_print) = NULL;
static int (*dmp_pm_edit_config)() = NULL;
static int (*dmp_pm_load_config)() = NULL;
static char * (*dmp_pm_get_config_location)(char * to_fill, size_t size) = NULL;
static gboolean dmp_pb_printer_module_consistent = TRUE;

/**
 * The trigger module
 */
static GModule * dmp_pb_trigger_module = NULL;
static int (*dmp_tm_add_trigger_handler)(void (*th)()) = NULL;
static int (*dmp_tm_set_countdown)(int current) = NULL;
static int (*dmp_tm_edit_config)() = NULL;
static int (*dmp_tm_load_config)() = NULL;
static char * (*dmp_tm_get_config_location)(char * to_fill, size_t size) = NULL;
static gboolean dmp_pb_trigger_module_consistent = TRUE;

/**
 * The camera module
 */
static GModule * dmp_pb_camera_module = NULL;
static int (*dmp_cm_capture)() = NULL;
static int (*dmp_cm_download)(char * location) = NULL;
static int (*dmp_cm_edit_config)() = NULL;
static int (*dmp_cm_load_config)() = NULL;
static char * (*dmp_cm_get_config_location)(char * to_fill, size_t size) = NULL;
static gboolean dmp_pb_camera_module_consistent = TRUE;

/**
 * state checking helper function. checks is_loaded and is_consistent
 * @param to_check the module type to check
 * @return DMP_PB_SUCCESS, or DMP_PB_MODULE_IN_INCONSISTENT_STATE if inconsistent,
 * or DMP_PB_MODULE_NOT_LOADED if not loaded
 */
static gint dmp_pb_check_module_state(dmp_pb_module_type to_check)
{
	if (!dmp_pb_is_consistent(to_check)) return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	if (!dmp_pb_is_loaded(to_check)) return dmp_pb_set_error_code_return(DMP_PB_MODULE_NOT_LOADED);
	return DMP_PB_SUCCESS;
}

/* dmp_pb_load_module helper functions */

/**
 * Loads the printer module
 * @param module_location the location of the printer module
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_load_printer_module(GString * module_location)
{
	if (!g_module_supported()) return dmp_pb_set_error_code_return(DMP_PB_G_MODULE_NOT_SUPPORTED);
	if (!dmp_pb_printer_module_consistent) return DMP_PB_MODULE_IN_INCONSISTENT_STATE;

	if (dmp_pb_printer_module != NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_ALREADY_LOADED);
	dmp_pb_printer_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_printer_module == NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_LOAD_UNLOAD_FAILED);

	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_print", (gpointer *) & dmp_pm_print))
	{
		dmp_pb_printer_module = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_edit_config", (gpointer *) & dmp_pm_edit_config))
	{
		dmp_pb_printer_module = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_load_config", (gpointer *) & dmp_pm_load_config))
	{
		dmp_pb_printer_module = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_printer_module, "dmp_pm_get_config_location", (gpointer *) & dmp_pm_get_config_location))
	{
		dmp_pb_printer_module = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}

	dmp_pb_printer_module_consistent = TRUE;
	return DMP_PB_SUCCESS;
}

/**
 * Loads the trigger module
 * @param module_location the location of the trigger module
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_load_trigger_module(GString * module_location)
{
	if (!g_module_supported()) return dmp_pb_set_error_code_return(DMP_PB_G_MODULE_NOT_SUPPORTED);
	if (!dmp_pb_trigger_module_consistent) return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);

	if (dmp_pb_trigger_module != NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_ALREADY_LOADED);
	dmp_pb_trigger_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_trigger_module == NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_LOAD_UNLOAD_FAILED);

	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_add_trigger_handler", (gpointer *) & dmp_tm_add_trigger_handler))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_set_countdown", (gpointer *) & dmp_tm_set_countdown))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_edit_config", (gpointer *) & dmp_tm_edit_config))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_load_config", (gpointer *) & dmp_tm_load_config))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_trigger_module, "dmp_tm_get_config_location", (gpointer *) & dmp_tm_get_config_location))
	{
		dmp_pb_trigger_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}

	dmp_pb_trigger_module_consistent = TRUE;
	return DMP_PB_SUCCESS;
}

/**
 * Loads the camera module
 * @param module_location the location of the camera module
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_load_camera_module(GString * module_location)
{
	if (!g_module_supported()) return dmp_pb_set_error_code_return(DMP_PB_G_MODULE_NOT_SUPPORTED);
	if (!dmp_pb_camera_module_consistent) return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);

	if (dmp_pb_camera_module != NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_ALREADY_LOADED);
	dmp_pb_camera_module = g_module_open(module_location->str, G_MODULE_BIND_LAZY);
	if (dmp_pb_camera_module == NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_LOAD_UNLOAD_FAILED);

	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_capture", (gpointer *) & dmp_cm_capture))
	{
		dmp_pb_camera_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_download", (gpointer *) & dmp_cm_download))
	{
		dmp_pb_camera_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_edit_config", (gpointer *) & dmp_cm_edit_config))
	{
		dmp_pb_camera_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_load_config", (gpointer *) & dmp_cm_load_config))
	{
		dmp_pb_camera_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}
	if (!g_module_symbol(dmp_pb_camera_module, "dmp_cm_get_config_location", (gpointer *) & dmp_cm_get_config_location))
	{
		dmp_pb_camera_module_consistent = FALSE;
		return dmp_pb_set_error_code_return(DMP_PB_MODULE_IN_INCONSISTENT_STATE);
	}

	dmp_pb_camera_module_consistent = TRUE;
	return DMP_PB_SUCCESS;
}

/* end of dmp_pb_load_module helper functions */

gint dmp_pb_load_module(dmp_pb_module_type to_load, GString * module_location)
{
	switch (to_load)
	{
		case DMP_PB_PRINTER_MODULE:
			return dmp_pb_load_printer_module(module_location);
		case DMP_PB_TRIGGER_MODULE:
			return dmp_pb_load_trigger_module(module_location);
		case DMP_PB_CAMERA_MODULE:
			return dmp_pb_load_camera_module(module_location);
		default:
			return DMP_PB_INVALID_MODULE_TYPE;
	}
}

/* dmp_pb_unload_module helper functions */

/**
 * unloads the printer module
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_unload_printer_module()
{
	if (!g_module_supported()) return dmp_pb_set_error_code_return(DMP_PB_G_MODULE_NOT_SUPPORTED);

	if (dmp_pb_printer_module == NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_NOT_LOADED);
	if (!g_module_close(dmp_pb_printer_module)) return dmp_pb_set_error_code_return(DMP_PB_MODULE_LOAD_UNLOAD_FAILED);

	dmp_pb_printer_module = NULL;
	dmp_pm_print = NULL;
	dmp_pm_edit_config = NULL;
	dmp_pm_get_config_location = NULL;
	dmp_pm_load_config = NULL;

	dmp_pb_printer_module_consistent = TRUE;
	return DMP_PB_SUCCESS;

}

/**
 * unloads the trigger module
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_unload_trigger_module()
{
	if (!g_module_supported()) return dmp_pb_set_error_code_return(DMP_PB_G_MODULE_NOT_SUPPORTED);

	if (dmp_pb_trigger_module == NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_NOT_LOADED);
	if (!g_module_close(dmp_pb_trigger_module)) return dmp_pb_set_error_code_return(DMP_PB_MODULE_LOAD_UNLOAD_FAILED);

	dmp_pb_trigger_module = NULL;
	dmp_tm_add_trigger_handler = NULL;
	dmp_tm_set_countdown = NULL;
	dmp_tm_edit_config = NULL;
	dmp_tm_get_config_location = NULL;
	dmp_tm_load_config = NULL;

	dmp_pb_trigger_module_consistent = TRUE;
	return DMP_PB_SUCCESS;
}

/**
 * unloads the camera module
 * @return DMP_PB_SUCCESS, or an error code
 */
static gint dmp_pb_unload_camera_module()
{
	if (!g_module_supported()) return dmp_pb_set_error_code_return(DMP_PB_G_MODULE_NOT_SUPPORTED);

	if (dmp_pb_camera_module == NULL) return dmp_pb_set_error_code_return(DMP_PB_MODULE_NOT_LOADED);
	if (!g_module_close(dmp_pb_camera_module)) return dmp_pb_set_error_code_return(DMP_PB_MODULE_LOAD_UNLOAD_FAILED);

	dmp_pb_camera_module = NULL;
	dmp_cm_capture = NULL;
	dmp_cm_download = NULL;
	dmp_cm_edit_config = NULL;
	dmp_cm_get_config_location = NULL;
	dmp_cm_load_config = NULL;

	dmp_pb_camera_module_consistent = TRUE;
	return DMP_PB_SUCCESS;
}

/* end of dmp_pb_unload_module helper functions */

gint dmp_pb_unload_module(dmp_pb_module_type to_unload)
{
	switch (to_unload)
	{
		case DMP_PB_PRINTER_MODULE:
			return dmp_pb_unload_printer_module();
		case DMP_PB_TRIGGER_MODULE:
			return dmp_pb_unload_trigger_module();
		case DMP_PB_CAMERA_MODULE:
			return dmp_pb_unload_camera_module();
		default:
			return DMP_PB_INVALID_MODULE_TYPE;
	}
}

gchar * dmp_pb_get_module_config_location(gchar * to_fill, gsize size, dmp_pb_module_type type)
{
	if (!dmp_pb_is_loaded(type)) return NULL;
	else if (!dmp_pb_is_consistent(type)) return NULL;
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			return (*dmp_pm_get_config_location)(to_fill, size);
		case DMP_PB_TRIGGER_MODULE:
			return (*dmp_tm_get_config_location)(to_fill, size);
		case DMP_PB_CAMERA_MODULE:
			return (*dmp_cm_get_config_location)(to_fill, size);
		default:
			dmp_pb_set_error_code(DMP_PB_INVALID_MODULE_TYPE);
			return NULL;
	}
}

gint dmp_pb_edit_module_config(dmp_pb_module_type type)
{
	gint result;
	if (result = dmp_pb_check_module_state(type) != DMP_PB_SUCCESS) return result;
	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			return (*dmp_pm_edit_config)();
		case DMP_PB_TRIGGER_MODULE:
			return (*dmp_tm_edit_config)();
		case DMP_PB_CAMERA_MODULE:
			return (*dmp_cm_edit_config)();
		default:
			return dmp_pb_set_error_code_return(DMP_PB_INVALID_MODULE_TYPE);
	}
}

gint dmp_pb_load_module_config(dmp_pb_module_type type)
{
	gint result;
	if (result = dmp_pb_check_module_state(type) != DMP_PB_SUCCESS) return result;

	switch (type)
	{
		case DMP_PB_PRINTER_MODULE:
			return (*dmp_pm_load_config)();
		case DMP_PB_TRIGGER_MODULE:
			return (*dmp_tm_load_config)();
		case DMP_PB_CAMERA_MODULE:
			return (*dmp_cm_load_config)();
		default:
			return dmp_pb_set_error_code_return(DMP_PB_INVALID_MODULE_TYPE);
	}
}

gint dmp_pb_swap_module(dmp_pb_module_type to_swap, GString * new_module_location)
{
	gint result = dmp_pb_unload_module(to_swap);
	if (result != DMP_PB_SUCCESS) return result;
	return dmp_pb_load_module(to_swap, new_module_location);
}

gboolean dmp_pb_is_loaded(dmp_pb_module_type to_check)
{
	if (!g_module_supported())
	{
		dmp_pb_set_error_code(DMP_PB_G_MODULE_NOT_SUPPORTED);
		return FALSE;
	}

	if (to_check == DMP_PB_CAMERA_MODULE) return (dmp_pb_camera_module != NULL
			&& dmp_cm_capture != NULL && dmp_cm_download != NULL);
	else if (to_check == DMP_PB_TRIGGER_MODULE) return (dmp_pb_trigger_module != NULL
			&& dmp_tm_add_trigger_handler != NULL && dmp_tm_set_countdown != NULL);
	else if (to_check == DMP_PB_PRINTER_MODULE) return (dmp_pb_printer_module != NULL
			&& dmp_pm_print != NULL);
	else
	{
		dmp_pb_set_error_code(DMP_PB_MODULE_NOT_LOADED);
		return FALSE;
	}
}

gboolean dmp_pb_is_consistent(dmp_pb_module_type to_check)
{
	if (!g_module_supported())
	{
		dmp_pb_set_error_code(DMP_PB_G_MODULE_NOT_SUPPORTED);
		return FALSE;
	}

	if (to_check == DMP_PB_CAMERA_MODULE) return dmp_pb_camera_module_consistent;
	else if (to_check == DMP_PB_TRIGGER_MODULE) return dmp_pb_trigger_module_consistent;
	else if (to_check == DMP_PB_PRINTER_MODULE) return dmp_pb_printer_module_consistent;
	else
	{
		dmp_pb_set_error_code(DMP_PB_MODULE_NOT_LOADED);
		return FALSE;
	}
}

gint dmp_pb_cm_capture()
{
	int result;
	if (result = dmp_pb_check_module_state(DMP_PB_CAMERA_MODULE) != DMP_PB_SUCCESS) return result;
	return (*dmp_cm_capture)();
}

gint dmp_pb_cm_download(gchar * location)
{
	int result;
	if (result = dmp_pb_check_module_state(DMP_PB_CAMERA_MODULE) != DMP_PB_SUCCESS) return result;
	return (*dmp_cm_download)(location);
}

gint dmp_pb_pm_print(gchar * to_print)
{
	int result;
	if (result = dmp_pb_check_module_state(DMP_PB_PRINTER_MODULE) != DMP_PB_SUCCESS) return result;
	return (*dmp_pm_print)(to_print);
}

gint dmp_pb_tm_add_trigger_handler(void (*th)())
{
	int result;
	if (result = dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE) != DMP_PB_SUCCESS) return result;
	return (*dmp_tm_add_trigger_handler)(th);
}

gint dmp_pb_tm_set_countdown(int current)
{
	int result;
	if (result = dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE) != DMP_PB_SUCCESS) return result;
	return (*dmp_tm_set_countdown)(current);
}