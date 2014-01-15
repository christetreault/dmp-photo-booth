#include "camera_logic.h"
#include "global_defines.h"
#include "dmp_camera_module.h"

static Camera * camera;
static GPContext * context;

static gboolean is_initialized = FALSE;

static void dmp_cm_camera_logic_log(GPLogLevel level, 
									const char *domain, 
									const char *format, 
									va_list args, 
									void *data)
{
	GString * va_working = g_string_new(NULL);
	GString * working = g_string_new(NULL);
	g_string_vprintf(va_working, format, args);
	g_string_append(va_working, "\n");
	g_string_printf(working, "%s %d: %s", domain, level, va_working->str);
	dmp_cm_console_write(working->str);
	g_string_free(va_working, TRUE);
	g_string_free(working, TRUE);
}

gint dmp_cm_camera_init(void)
{
	context = gp_context_new();
	//gp_log_add_func(GP_LOG_ERROR, (GPLogFunc) dmp_cm_camera_logic_log, NULL);
	
	if (gp_camera_new(&camera) != GP_OK)
	{
		gp_context_unref(context);
		dmp_cm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	
	if (gp_camera_init(camera, context) != GP_OK)
	{
		dmp_cm_camera_finalize();
		dmp_cm_set_status(FALSE);
		return DMP_PB_FAILURE;
	}
	
	is_initialized = TRUE;
	dmp_cm_set_status(TRUE);
	return DMP_PB_SUCCESS;
}

gint dmp_cm_camera_finalize(void)
{
	dmp_cm_set_status(FALSE);
	
	gp_camera_unref(camera);
	gp_context_unref(context);
	
	is_initialized = FALSE;
	
	return DMP_PB_SUCCESS;
}

gint dmp_cm_camera_capture(const gchar * location)
{
	g_assert(is_initialized);
	CameraFile * file;
	CameraFilePath camera_file_path;
	gint fd;
	GString * scratch_pad;
	GError * error = NULL;
	
	CameraEventType event_type;
	void * event_data;
	
	if (gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context) != GP_OK)
	{
		dmp_cm_console_write("Failed to capture image!\n");
		return DMP_PB_FAILURE;
	}
	
	if ((fd = g_open(location, O_CREAT | O_WRONLY, 0644)) == -1)
	{
		scratch_pad = g_string_new(NULL);
		g_string_printf(scratch_pad, "Failed to create file: \"%s\"!\n", location);
		dmp_cm_console_write(scratch_pad->str);
		g_string_free(scratch_pad, TRUE);
		return DMP_PB_FAILURE;
	}
	
	do
	{
		gp_camera_wait_for_event(camera, 1000, &event_type, &event_data, context);
		if (event_type == GP_EVENT_CAPTURE_COMPLETE) break;
	}
	while(event_type != GP_EVENT_TIMEOUT);
	
	if (gp_file_new_from_fd(&file, fd) != GP_OK)
	{
		dmp_cm_console_write("Failed to create camera file!\n");
		g_close(fd, &error);
		if (error != NULL)
		{
			scratch_pad = g_string_new(NULL);
			g_string_printf(scratch_pad, "%s %d: %s\n", g_quark_to_string(error->domain),
							error->code, error->message);
			dmp_cm_console_write(scratch_pad->str);
			g_string_free(scratch_pad, TRUE);
			g_clear_error(&error);
		}
		return DMP_PB_FAILURE;
	}
	
	do
	{
		gp_camera_wait_for_event(camera, 1000, &event_type, &event_data, context);
	}
	while(event_type != GP_EVENT_TIMEOUT);
	
	if (gp_camera_file_get(camera, 
							camera_file_path.folder, 
							camera_file_path.name, 
							GP_FILE_TYPE_NORMAL, 
							file, 
							context) != GP_OK)
	{
		scratch_pad = g_string_new(NULL);
		g_string_printf(scratch_pad, "Failed to copy file from camera to: \"%s\"!\n", location);
		dmp_cm_console_write(scratch_pad->str);
		g_string_free(scratch_pad, TRUE);
		gp_file_free(file);
		return DMP_PB_FAILURE;
	}
	
	if (gp_camera_file_delete(camera, 
								camera_file_path.folder, 
								camera_file_path.name,
								context) != GP_OK)
	{
		dmp_cm_console_write("Failed to delete file from camera!\n");
		gp_file_free(file);
		return DMP_PB_FAILURE;
	}
	gp_file_free(file);
	
	do
	{
		gp_camera_wait_for_event(camera, 1000, &event_type, &event_data, context);
	}
	while(event_type != GP_EVENT_TIMEOUT);
	
	return DMP_PB_SUCCESS;
}

