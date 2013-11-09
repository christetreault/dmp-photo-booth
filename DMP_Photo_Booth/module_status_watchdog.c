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
		return_status = (dmp_pb_is_loaded(DMP_PB_PRINTER_MODULE) && dmp_pb_is_consistent(DMP_PB_PRINTER_MODULE));
		if (return_status != recorded_printer_status)
		{
			recorded_printer_status = return_status;
			if (return_status) g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_PRINTER_UP));
			else g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_PRINTER_DOWN));
		}
		
		return_status = (dmp_pb_is_loaded(DMP_PB_TRIGGER_MODULE) && dmp_pb_is_consistent(DMP_PB_TRIGGER_MODULE));
		if (return_status != recorded_trigger_status)
		{
			recorded_trigger_status = return_status;
			if (return_status) g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_TRIGGER_UP));
			else g_async_queue_push(thread_queue_ref, GINT_TO_POINTER(DMP_PB_MWD_TRIGGER_DOWN));
		}
		
		return_status = (dmp_pb_is_loaded(DMP_PB_CAMERA_MODULE) && dmp_pb_is_consistent(DMP_PB_CAMERA_MODULE));
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

gint dmp_pb_mwd_init(dmp_pb_ui_status_icons * in_status_icons)
{
	if (in_status_icons == NULL) dmp_pb_set_error_code_return(DMP_PB_NULL_POINTER);
	dmp_pb_mwd_status_icons = in_status_icons;
	dmp_pb_mwd_status_queue = g_async_queue_new();
	
	dmp_pb_module_watchdog_thread = g_thread_new("MWD Thread", dmp_pb_mwd_thread_function, NULL);
	
	return DMP_PB_SUCCESS;
}

gint dmp_pb_mwd_push(gint to_push)
{
	g_async_queue_push(dmp_pb_mwd_status_queue, GINT_TO_POINTER(to_push));
	return DMP_PB_SUCCESS;
}

gboolean dmp_pb_mwd_handle_message(gpointer user_data) //TODO: do I need to call repaint method?
{
	gint message = GPOINTER_TO_INT(g_async_queue_try_pop(dmp_pb_mwd_status_queue));
	
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

gint dmp_pb_mwd_finalize()
{
	dmp_pb_mwd_thread_should_die = TRUE;
	g_thread_join(dmp_pb_module_watchdog_thread);
	g_async_queue_unref(dmp_pb_mwd_status_queue);
}

