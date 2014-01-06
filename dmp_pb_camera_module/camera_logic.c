#include "camera_logic.h"
#include "global_defines.h"
#include "dmp_camera_module.h"

static Camera * camera;
static GPContext * context;

static gboolean is_initialized = FALSE;

static void dmp_cm_message_func(GPContext *context, const gchar *format, va_list args, void *data)
{
	g_printf("message_func()\n");
}

static void dmp_cm_error_func(GPContext *context, const gchar *format, va_list args, void *data)
{
	g_printf("error_func()\n");
}

gint dmp_cm_camera_init()
{
	if (gp_camera_new(&camera) != GP_OK) return DMP_PB_FAILURE;
	context = gp_context_new();
	
	// TODO: required?
	gp_context_set_error_func(context, dmp_cm_error_func, NULL);
	gp_context_set_message_func(context, dmp_cm_message_func, NULL);
	
	if (gp_camera_init(camera, context) != GP_OK)
	{
		dmp_cm_camera_finalize();
		return DMP_PB_FAILURE;
	}
	
	is_initialized = TRUE;
	
	return DMP_PB_SUCCESS;
}

gint dmp_cm_camera_finalize()
{
	gp_camera_unref(camera);
	gp_context_unref(context);
	
	is_initialized = FALSE;
	
	return DMP_PB_SUCCESS;
}

gboolean dmp_cm_camera_is_initialized()
{
	return is_initialized;
}

gint dmp_cm_camera_capture(gchar * location)
{
	g_assert(dmp_cm_camera_is_initialized());
	CameraFile * file;
	CameraFilePath camera_file_path;
	gint fd;
	GString * scratch_pad;
	GError * error = NULL;
	
	
	if (gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context) != GP_OK)
	{
		dmp_cm_console_write("Failed to capture image!\n");
		return DMP_PB_FAILURE;
	}
	
	if (fd = g_open(location, O_CREAT | O_WRONLY, 0644) == -1)
	{
		scratch_pad = g_string_new(NULL);
		g_string_printf(scratch_pad, "Failed to create file: \"%s\"!\n", location);
		dmp_cm_console_write(scratch_pad->str);
		g_string_free(scratch_pad, TRUE);
		return DMP_PB_FAILURE;
	}
	
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
	
	g_assert(FALSE);
}