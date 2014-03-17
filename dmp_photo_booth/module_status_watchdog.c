#include "module_status_watchdog.h"

dmp_pb_ui_status_icons * dmp_pb_mwd_status_icons;

static gboolean dmp_pb_mwd_recorded_camera_status = FALSE;
static gboolean dmp_pb_mwd_recorded_printer_status = FALSE;
static gboolean dmp_pb_mwd_recorded_trigger_status = FALSE;

void dmp_pb_mwd_init(dmp_pb_ui_status_icons * in_status_icons)
{
	g_assert(in_status_icons != NULL);
	dmp_pb_mwd_status_icons = in_status_icons;
}

gboolean dmp_pb_mwd_poll_modules(gpointer user_data)
{
	gboolean status;
	status = (dmp_pb_check_module_state(DMP_PB_PRINTER_MODULE) == DMP_PB_MODULE_READY);
	if (status != dmp_pb_mwd_recorded_printer_status)
	{
		dmp_pb_mwd_recorded_printer_status = status;
		if (status) gtk_image_set_from_icon_name(dmp_pb_mwd_status_icons->printer_module_staus_icon, "gtk-ok", GTK_ICON_SIZE_BUTTON);
		else gtk_image_set_from_icon_name(dmp_pb_mwd_status_icons->printer_module_staus_icon, "gtk-dialog-warning", GTK_ICON_SIZE_BUTTON);
	}

	status = (dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE) == DMP_PB_MODULE_READY);
	if (status != dmp_pb_mwd_recorded_trigger_status)
	{
		dmp_pb_mwd_recorded_trigger_status = status;
		if (status) gtk_image_set_from_icon_name(dmp_pb_mwd_status_icons->trigger_module_staus_icon, "gtk-ok", GTK_ICON_SIZE_BUTTON);
		else gtk_image_set_from_icon_name(dmp_pb_mwd_status_icons->trigger_module_staus_icon, "gtk-dialog-warning", GTK_ICON_SIZE_BUTTON);
	}

	status = (dmp_pb_check_module_state(DMP_PB_CAMERA_MODULE) == DMP_PB_MODULE_READY);
	if (status != dmp_pb_mwd_recorded_camera_status)
	{
		dmp_pb_mwd_recorded_camera_status = status;
		if (status) gtk_image_set_from_icon_name(dmp_pb_mwd_status_icons->camera_module_staus_icon, "gtk-ok", GTK_ICON_SIZE_BUTTON);
		else gtk_image_set_from_icon_name(dmp_pb_mwd_status_icons->camera_module_staus_icon, "gtk-dialog-warning", GTK_ICON_SIZE_BUTTON);
	}
	return G_SOURCE_CONTINUE;
}