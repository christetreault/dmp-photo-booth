#include "module_status_watchdog.h"

dmp_pb_ui_status_icons * dmp_pb_mwd_status_icons;
static GAsyncQueue * dmp_pb_mwd_status_queue;
static GThread * dmp_pb_module_watchdog_thread;

static gboolean dmp_pb_mwd_thread_should_die = FALSE;

/**
 * The function to be called by the module status watchdog thread
 * @param user_data required by GThread. Is not used by anything, so NULL should
 * be passed in
 * @return required by GThread. Always returns NULL
 */
static gpointer dmp_pb_mwd_thread_function(gpointer user_data)
{
	gboolean recorded_camera_status = FALSE;
	gboolean recorded_printer_status = FALSE;
	gboolean recorded_trigger_status = FALSE;
	gboolean return_status;
	GAsyncQueue * thread_queue_ref = g_async_queue_ref(dmp_pb_mwd_status_queue);

	while (!dmp_pb_mwd_thread_should_die)
	{
		return_status = (dmp_pb_check_module_state(DMP_PB_PRINTER_MODULE) == DMP_PB_MODULE_READY);
		if (return_status != recorded_printer_status)
		{
			recorded_printer_status = return_status;
			if (return_status) g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_PRINTER_UP));
			else g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_PRINTER_DOWN));
		}

		return_status = (dmp_pb_check_module_state(DMP_PB_TRIGGER_MODULE) == DMP_PB_MODULE_READY);
		if (return_status != recorded_trigger_status)
		{
			recorded_trigger_status = return_status;
			if (return_status) g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_TRIGGER_UP));
			else g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_TRIGGER_DOWN));
		}

		return_status = (dmp_pb_check_module_state(DMP_PB_CAMERA_MODULE) == DMP_PB_MODULE_READY);
		if (return_status != recorded_camera_status)
		{
			recorded_camera_status = return_status;
			if (return_status) g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_CAMERA_UP));
			else g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_CAMERA_DOWN));
		}
		g_usleep(G_USEC_PER_SEC * 3);
	}
	g_async_queue_unref(thread_queue_ref);
	return NULL;
}

void dmp_pb_mwd_init(dmp_pb_ui_status_icons * in_status_icons)
{
	g_assert(in_status_icons != NULL);
	dmp_pb_mwd_status_icons = in_status_icons;
	dmp_pb_mwd_status_queue = g_async_queue_new();

	dmp_pb_module_watchdog_thread = g_thread_new("MWD Thread", dmp_pb_mwd_thread_function, NULL);
}

gboolean dmp_pb_mwd_handle_message(gpointer user_data)
{
	GAsyncQueue * local_status_queue = g_async_queue_ref(dmp_pb_mwd_status_queue);
	gint message = GPOINTER_TO_INT(g_async_queue_try_pop(local_status_queue));
	g_async_queue_unref(local_status_queue);
	

	switch (message)
	{
		case DMP_PB_MWD_CAMERA_UP:
			gtk_image_set_from_stock(dmp_pb_mwd_status_icons->camera_module_staus_icon, "gtk-ok", GTK_ICON_SIZE_BUTTON);
			break;
		case DMP_PB_MWD_CAMERA_DOWN:
			gtk_image_set_from_stock(dmp_pb_mwd_status_icons->camera_module_staus_icon, "gtk-dialog-warning", GTK_ICON_SIZE_BUTTON);
			break;
		case DMP_PB_MWD_PRINTER_UP:
			gtk_image_set_from_stock(dmp_pb_mwd_status_icons->printer_module_staus_icon, "gtk-ok", GTK_ICON_SIZE_BUTTON);
			break;
		case DMP_PB_MWD_PRINTER_DOWN:
			gtk_image_set_from_stock(dmp_pb_mwd_status_icons->printer_module_staus_icon, "gtk-dialog-warning", GTK_ICON_SIZE_BUTTON);
			break;
		case DMP_PB_MWD_TRIGGER_UP:
			gtk_image_set_from_stock(dmp_pb_mwd_status_icons->trigger_module_staus_icon, "gtk-ok", GTK_ICON_SIZE_BUTTON);
			break;
		case DMP_PB_MWD_TRIGGER_DOWN:
			gtk_image_set_from_stock(dmp_pb_mwd_status_icons->trigger_module_staus_icon, "gtk-dialog-warning", GTK_ICON_SIZE_BUTTON);
			break;
	}
	return G_SOURCE_CONTINUE;
}

void dmp_pb_mwd_finalize(void)
{
	dmp_pb_mwd_thread_should_die = TRUE;
	g_thread_join(dmp_pb_module_watchdog_thread);
	g_async_queue_unref(dmp_pb_mwd_status_queue);
}

