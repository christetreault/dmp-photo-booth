#include "coordination.h"
#include "configuration.h"
#include "module.h"
#include "console_queue.h"

G_DEFINE_QUARK(DMP_PB_COORDINATION_ERROR, dmp_pb_coordination_error)

gchar * dmp_pb_coordination_get_epoch_filename(const gchar * prefix, const gchar * extension)
{
	GString * working = g_string_new(NULL);
	
	#ifdef G_GINT64_MODIFIER
	g_string_printf(working, "%s_%" G_GINT64_MODIFIER "d.%s", prefix, g_get_real_time(), extension);
	#endif
	#ifndef G_GINT64_MODIFIER
	guint val = ABS(g_random_int());
	g_string_printf(working, "%s_%u.%s", prefix, val, extension);
	#endif

	return g_string_free(working, FALSE);
}

G_LOCK_DEFINE(photo_request);
static GThread * photo_request_thread = NULL;

#define DMP_PB_COUNTDOWN_TIME 4
#define DMP_PB_POS5_FILE_NAME "5.jpg"
#define DMP_PB_POS4_FILE_NAME "4.jpg"
#define DMP_PB_POS3_FILE_NAME "3.jpg"
#define DMP_PB_POS2_FILE_NAME "2.jpg"
#define DMP_PB_POS1_FILE_NAME "1.jpg"

/**
 * counts down, captures, then downloads an image
 * @param secs the seconds to count down
 * @param file_name the file name to save to
 */
static void dmp_pb_photo_request_countdown_capture(gint secs, gchar * file_name, GError ** error)
{
	g_assert(file_name != NULL);
	
	int count;
	
	for (count = secs; count > 0; count--)
	{
		dmp_pb_tm_set_countdown(count);
		g_usleep(G_USEC_PER_SEC * 1);
	}
	
	if (dmp_pb_cm_capture() != DMP_PB_SUCCESS)
	{
		g_set_error(error,
					dmp_pb_coordination_error_quark(),
					CAMERA_MODULE_ERROR,
					"Failed to capture image!");
		return;
	}
	dmp_pb_tm_set_countdown(0);
	g_usleep(G_USEC_PER_SEC * 1);
	
	if (dmp_pb_cm_download(file_name) != DMP_PB_SUCCESS)
	{
		g_set_error(error,
					dmp_pb_coordination_error_quark(),
					CAMERA_MODULE_ERROR,
					"Failed to download image to %s!", file_name);
		return;
	}
	return;
}

/**
 * cleans up after the 5 gchar * working filenames in dmp_pb_photo_request_thread_function
 * @param file1
 * @param file2
 * @param file3
 * @param file4
 * @param file5
 */
static void dmp_pb_photo_request_cleanup_file_names(gchar * file1, gchar * file2, gchar * file3,
													gchar * file4, gchar * file5)
{
	if (file1 != NULL) g_free(file1);
	if (file2 != NULL) g_free(file2);
	if (file3 != NULL) g_free(file3);
	if (file4 != NULL) g_free(file4);
	if (file5 != NULL) g_free(file5);
}

/**
 * Similar to g_string_new, but passing in NULL causes NULL to be retruned
 * @param working
 * @return 
 */
static GString * dmp_pb_photo_request_new_or_null(gchar * working)
{
	if (working == NULL) return NULL;
	return g_string_new(working);
}

/**
 * This thread does the actual photo request processing so that
 * dmp_pb_handle_photo_request can return immediatly and not hang the UI
 * @param user_data NULL, not used
 * @return NULL, not used
 */
static gpointer dmp_pb_photo_request_thread_function(gpointer user_data)
{
	gint image_position_toggle = dmp_pb_config_read_int(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_POSITION_TOGGLE);
	GError * error = NULL;
	
	GString * working = dmp_pb_config_read_string(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_PATH);
	gchar * file1 = NULL;
	gchar * file2 = NULL;
	gchar * file3 = NULL;
	gchar * file4 = NULL;
	gchar * file5 = NULL;
	
	if (image_position_toggle & DMP_PB_PHOTO_STRIP_POSITION_1_FLAG)
	{
		file1 = g_build_filename(working->str, DMP_PB_POS1_FILE_NAME, NULL);
		dmp_pb_photo_request_countdown_capture(DMP_PB_COUNTDOWN_TIME, file1, &error);
		if (error != NULL)
		{
			dmp_pb_tm_show_error(error->code);
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
			dmp_pb_photo_request_cleanup_file_names(file1, file2, file3, file4, file5);
			g_string_free(working, TRUE);
			return NULL;
		}
	}
	
	if (image_position_toggle & DMP_PB_PHOTO_STRIP_POSITION_2_FLAG)
	{
		file2 = g_build_filename(working->str, DMP_PB_POS2_FILE_NAME, NULL);
		dmp_pb_photo_request_countdown_capture(DMP_PB_COUNTDOWN_TIME, file2, &error);
		if (error != NULL)
		{
			dmp_pb_tm_show_error(error->code);
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
			dmp_pb_photo_request_cleanup_file_names(file1, file2, file3, file4, file5);
			g_string_free(working, TRUE);
			return NULL;
		}
	}
	
	if (image_position_toggle & DMP_PB_PHOTO_STRIP_POSITION_3_FLAG)
	{
		file3 = g_build_filename(working->str, DMP_PB_POS3_FILE_NAME, NULL);
		dmp_pb_photo_request_countdown_capture(DMP_PB_COUNTDOWN_TIME, file3, &error);
		if (error != NULL)
		{
			dmp_pb_tm_show_error(error->code);
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
			dmp_pb_photo_request_cleanup_file_names(file1, file2, file3, file4, file5);
			g_string_free(working, TRUE);
			return NULL;
		}
	}
	
	if (image_position_toggle & DMP_PB_PHOTO_STRIP_POSITION_4_FLAG)
	{
		file4 = g_build_filename(working->str, DMP_PB_POS4_FILE_NAME, NULL);
		dmp_pb_photo_request_countdown_capture(DMP_PB_COUNTDOWN_TIME, file4, &error);
		if (error != NULL)
		{
			dmp_pb_tm_show_error(error->code);
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
			dmp_pb_photo_request_cleanup_file_names(file1, file2, file3, file4, file5);
			g_string_free(working, TRUE);
			return NULL;
		}
	}
	
	if (image_position_toggle & DMP_PB_PHOTO_STRIP_POSITION_5_FLAG)
	{
		file5 = g_build_filename(working->str, DMP_PB_POS5_FILE_NAME, NULL);
		dmp_pb_photo_request_countdown_capture(DMP_PB_COUNTDOWN_TIME, file5, &error);
		if (error != NULL)
		{
			dmp_pb_tm_show_error(error->code);
			dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
			g_clear_error(&error);
			dmp_pb_photo_request_cleanup_file_names(file1, file2, file3, file4, file5);
			g_string_free(working, TRUE);
			return NULL;
		}
	}
	
	GString * completed_path = dmp_pb_config_read_string(DMP_PB_CONFIG_CORE_GROUP, 
												DMP_PB_CONFIG_COMPLETED_STRIP_PATH);
	gchar * completed_path_filename = dmp_pb_coordination_get_epoch_filename("photo_strip", "jpg");
	
	gchar * completed_path_working = g_build_filename
			(
				completed_path->str,
				completed_path_filename,
				NULL
			);
	g_string_assign(completed_path, completed_path_working);
	g_free(completed_path_filename);
	g_free(completed_path_working);
	
	dmp_pb_photo_strip_request(completed_path,
			dmp_pb_photo_request_new_or_null(file1),
			dmp_pb_photo_request_new_or_null(file2),
			dmp_pb_photo_request_new_or_null(file3),
			dmp_pb_photo_request_new_or_null(file4),
			dmp_pb_photo_request_new_or_null(file5));
	
	dmp_pb_photo_request_cleanup_file_names(file1, file2, file3, file4, file5);
	g_string_free(working, TRUE);
	G_UNLOCK(photo_request);
}

gboolean dmp_pb_handle_photo_request()
{
	if (!G_TRYLOCK(photo_request)) return FALSE;
	if (photo_request_thread != NULL) g_thread_unref(photo_request_thread);
	photo_request_thread = g_thread_new("Photo Request", dmp_pb_photo_request_thread_function, NULL);
	return TRUE;
}