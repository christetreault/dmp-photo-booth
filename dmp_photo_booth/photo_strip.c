#include "photo_strip.h"
#include "configuration.h"
#include "console_queue.h"

G_DEFINE_QUARK(DMP_PB_PHOTO_STRIP_ERROR, dmp_pb_photo_strip_error)

static GAsyncQueue * in_queue = NULL;
static GAsyncQueue * out_queue = NULL;
static GThread * dmp_pb_photo_strip_thread;

/**
 * in keeping with the spirit of MagickWand. TRUE if MagickWandGenesis() has been
 * called, FALSE if not, or after a call to MagickWandTerminus()
 */
static gboolean there_was_light = FALSE;

struct photo_strip_builder
{
	MagickWand * background_wand;
	MagickWand * working_wand;
	MagickWand * final_wand;
	
	GdkPixbuf * thumbnail;
	
	GString * completed_strip_file_name;
	GString * position_1_file_name;
	GString * position_2_file_name;
	GString * position_3_file_name;
	GString * position_4_file_name;
	GString * position_5_file_name;
	
	gboolean success;
	GQuark error_domain;
	dmp_pb_photo_strip_error error_code;
	GString * error_message;
};

/**
 * Function to be called by photo strip thread
 * @param user_data NULL, unused
 * @return NULL
 */
static gpointer dmp_pb_photo_strip_thread_function(gpointer user_data)
{
	while (there_was_light)
	{
		dmp_pb_photo_strip_assemble();
		g_usleep(G_USEC_PER_SEC * 5);
	}
	return NULL;
}

gboolean dmp_pb_photo_strip_initialized()
{
	return (there_was_light && in_queue != NULL && out_queue != NULL);
}

/**
 * cleans up the memory of a photo_strip_builder
 * @param to_smite the photo_strip_builder to reduce to a column of salt
 */
static void dmp_pb_photo_strip_smite_builder(struct photo_strip_builder * to_smite)
{
	g_assert(to_smite != NULL);
	g_assert(dmp_pb_photo_strip_initialized());
	
	if (to_smite->completed_strip_file_name != NULL) g_string_free(to_smite->completed_strip_file_name, TRUE);
	if (to_smite->position_1_file_name != NULL) g_string_free(to_smite->position_1_file_name, TRUE);
	if (to_smite->position_2_file_name != NULL) g_string_free(to_smite->position_2_file_name, TRUE);
	if (to_smite->position_3_file_name != NULL) g_string_free(to_smite->position_3_file_name, TRUE);
	if (to_smite->position_4_file_name != NULL) g_string_free(to_smite->position_4_file_name, TRUE);
	if (to_smite->position_5_file_name != NULL) g_string_free(to_smite->position_5_file_name, TRUE);
	
	if (to_smite->working_wand != NULL) DestroyMagickWand(to_smite->working_wand);
	if (to_smite->background_wand != NULL)DestroyMagickWand(to_smite->background_wand);
	if (to_smite->final_wand != NULL) DestroyMagickWand(to_smite->final_wand); // ...and there was Great Wrath!
	
	if (to_smite->error_message != NULL) g_string_free(to_smite->error_message, TRUE);
	if (to_smite->thumbnail != NULL) g_object_unref(to_smite->thumbnail);
	
	g_free(to_smite);
}

void dmp_pb_photo_strip_init()
{
	if (dmp_pb_photo_strip_initialized()) return;
	
	if (in_queue == NULL) in_queue = g_async_queue_new_full((GDestroyNotify) dmp_pb_photo_strip_smite_builder);
	if (out_queue == NULL) out_queue = g_async_queue_new_full((GDestroyNotify) dmp_pb_photo_strip_smite_builder);
	if (!there_was_light) MagickWandGenesis();
	there_was_light = TRUE;
	dmp_pb_photo_strip_thread = g_thread_new("Photo Strip Thread", dmp_pb_photo_strip_thread_function, NULL);
	
}

void dmp_pb_photo_strip_finalize()
{
	if (in_queue != NULL) g_async_queue_unref(in_queue);
	in_queue = NULL;
	if (in_queue != NULL) g_async_queue_unref(out_queue);
	out_queue = NULL;
	MagickWandTerminus();
	there_was_light = FALSE;
}

/**
 * Sets the format string for call to MagickResetImagePage
 * @param to_modify the string to modify
 * @param in_width the image width
 * @param aspect_ratio the image aspect ratio
 * @param offset_x offset from x
 * @param offset_y offset from y
 * @return to_modify->str. <b>DO NOT ATTEMPT TO FREE THIS POINTER!</b> 
 */
static gchar * dmp_pb_photo_strip_set_builder(GString * to_modify, gdouble in_width, gdouble aspect_ratio, guint offset_x, guint offset_y)
{
	g_assert(dmp_pb_photo_strip_initialized());
	g_assert(to_modify != NULL);
	
	gint width = (gint) in_width;
	gint height = (gint) in_width / aspect_ratio;
	
	g_string_printf(to_modify, "%dx%d+%d+%d!", width, height, offset_x, offset_y);
	
	return to_modify->str;
}

void dmp_pb_photo_strip_request(GString * completed_strip_name,
												GString * pos_1_name,
												GString * pos_2_name,
												GString * pos_3_name,
												GString * pos_4_name,
												GString * pos_5_name)
{
	g_assert(completed_strip_name != NULL);
	g_assert(dmp_pb_photo_strip_initialized());
	
	struct photo_strip_builder * working = g_malloc(sizeof(struct photo_strip_builder));
	
	working->completed_strip_file_name = completed_strip_name;
	working->position_1_file_name = pos_1_name;
	working->position_2_file_name = pos_2_name;
	working->position_3_file_name = pos_3_name;
	working->position_4_file_name = pos_4_name;
	working->position_5_file_name = pos_5_name;
	working->background_wand = NewMagickWand();
	working->working_wand = NewMagickWand();
	working->final_wand = NULL;
	working->error_message = NULL;
	working->thumbnail = NULL;
	
	g_async_queue_push(in_queue, working);
}

void dmp_pb_photo_strip_assemble()
{
	g_assert(dmp_pb_photo_strip_initialized());
	
	/* -------- */
	/* Creation */
	/* -------- */
	
	struct photo_strip_builder * working = g_async_queue_try_pop(in_queue);
	
	if (working == NULL) return; // No requests available
	
	gchar * magick_exception_message = NULL;
	dmp_pb_photo_strip_error magick_exception_severity;
	gdouble width = dmp_pb_config_read_double(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_WIDTH);
	gdouble aspect_ratio = dmp_pb_config_read_double(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_ASPECT_RATIO);
	gint offset_x = 15;
	gint offset_y = 15;
	GError * error = NULL;
	
	/* ---------- */
	/* Processing */
	/* ---------- */
	
	GString * background = dmp_pb_config_read_string(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_BACKGROUND_PATH);
	if (!MagickReadImage(working->background_wand, background->str))
	{
		magick_exception_message = MagickGetException(working->background_wand, &magick_exception_severity);
		working->error_message = g_string_new(magick_exception_message);
		working->error_domain = dmp_pb_photo_strip_error_quark();
		working->error_code = magick_exception_severity;
		MagickRelinquishMemory(magick_exception_message);
		g_async_queue_push(out_queue, working);
		g_string_free(background, TRUE);
		return;
	}
	g_string_free(background, TRUE);
	
	GString * page_builder = g_string_new(NULL);
	
	if (working->position_1_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_1_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		
		if (!MagickResizeImage(working->working_wand, (gint) width, ((gint) width / aspect_ratio), LanczosFilter, 1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResetImagePage(working->working_wand, dmp_pb_photo_strip_set_builder(page_builder, width, aspect_ratio, offset_x, offset_y)))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
	}
	
	offset_y = offset_y + ((gint) width / aspect_ratio) + 15;
	
	if (working->position_2_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_2_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResizeImage(working->working_wand, (gint) width, ((gint) width / aspect_ratio), LanczosFilter, 1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResetImagePage(working->working_wand, dmp_pb_photo_strip_set_builder(page_builder, width, aspect_ratio, offset_x, offset_y)))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
	}
	
	offset_y = offset_y + ((gint) width / aspect_ratio) + 15;
	
	if (working->position_3_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_3_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResizeImage(working->working_wand, (gint) width, ((gint) width / aspect_ratio), LanczosFilter, 1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResetImagePage(working->working_wand, dmp_pb_photo_strip_set_builder(page_builder, width, aspect_ratio, offset_x, offset_y)))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
	}
	
	offset_y = offset_y + ((gint) width / aspect_ratio) + 15;
	
	if (working->position_4_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_4_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResizeImage(working->working_wand, (gint) width, ((gint) width / aspect_ratio), LanczosFilter, 1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResetImagePage(working->working_wand, dmp_pb_photo_strip_set_builder(page_builder, width, aspect_ratio, offset_x, offset_y)))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
	}
	
	offset_y = offset_y + ((gint) width / aspect_ratio) + 15;
	
	if (working->position_5_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_5_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResizeImage(working->working_wand, (gint) width, ((gint) width / aspect_ratio), LanczosFilter, 1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
		if (!MagickResetImagePage(working->working_wand, dmp_pb_photo_strip_set_builder(page_builder, width, aspect_ratio, offset_x, offset_y)))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(out_queue, working);
			g_string_free(page_builder, TRUE);
			return;
		}
	}
	
	MagickSetLastIterator(working->background_wand);
	if (!MagickAddImage(working->background_wand, working->working_wand))
	{
		magick_exception_message = MagickGetException(working->background_wand, &magick_exception_severity);
		working->error_message = g_string_new(magick_exception_message);
		working->error_domain = dmp_pb_photo_strip_error_quark();
		working->error_code = magick_exception_severity;
		MagickRelinquishMemory(magick_exception_message);
		g_async_queue_push(out_queue, working);
		g_string_free(page_builder, TRUE);
		return;
	}
	
	/* ----- */
	/* Saving*/
	/* ----- */
	
	working->final_wand = MagickCoalesceImages(working->background_wand);
	if (working->final_wand == NULL)
	{
		magick_exception_message = MagickGetException(working->background_wand, &magick_exception_severity);
		working->error_message = g_string_new(magick_exception_message);
		working->error_domain = dmp_pb_photo_strip_error_quark();
		working->error_code = magick_exception_severity;
		MagickRelinquishMemory(magick_exception_message);
		g_async_queue_push(out_queue, working);
		g_string_free(page_builder, TRUE);
		return;
	}
	MagickSetLastIterator(working->final_wand);
	if (!MagickWriteImage(working->final_wand, working->completed_strip_file_name->str))
	{
		magick_exception_message = MagickGetException(working->final_wand, &magick_exception_severity);
		working->error_message = g_string_new(magick_exception_message);
		working->error_domain = dmp_pb_photo_strip_error_quark();
		working->error_code = magick_exception_severity;
		MagickRelinquishMemory(magick_exception_message);
		g_async_queue_push(out_queue, working);
		g_string_free(page_builder, TRUE);
		return;
	}
	g_string_free(page_builder, TRUE);
	
	/* ---------------- */
	/* Create Thumbnail */
	/* ---------------- */
	
	GdkPixbuf * full = gdk_pixbuf_new_from_file(working->completed_strip_file_name->str, &error);
	
	if (error != NULL)
	{
		working->error_message = g_string_new(error->message);
		working->error_domain = error->domain;
		working->error_code = error->code;
		g_clear_error(&error);
	}
	else
	{
		working->thumbnail = gdk_pixbuf_scale_simple
			(
				full, 
				(gint) (gdk_pixbuf_get_width(full) * (256.0 / gdk_pixbuf_get_height(full))),
				256,
				GDK_INTERP_BILINEAR
			);
	
		g_object_unref(full);
	}
	
	/* ---------------- */
	/* One last push... */
	/* ---------------- */
	
	g_async_queue_push(out_queue, working);
}

GdkPixbuf * dmp_pb_photo_strip_get_result(GString ** out_path, GError ** error)
{
	g_assert(dmp_pb_photo_strip_initialized());
	if (out_path != NULL) g_assert(*out_path == NULL);
	
	struct photo_strip_builder * working = g_async_queue_try_pop(out_queue);
	
	if (working == NULL) return NULL; // No new results
	
	if (working->error_message != NULL)
	{
		g_set_error(error,
				working->error_domain,
				working->error_code,
				"%s", working->error_message->str);
		g_string_free(working->error_message, TRUE);
		working->error_message = NULL;
		dmp_pb_photo_strip_smite_builder(working);
		out_path = NULL;
		return NULL;
	}
	
	GdkPixbuf * return_value = working->thumbnail;
	working->thumbnail = NULL;
	
	if (out_path != NULL)
	{
		*out_path = working->completed_strip_file_name;
		working->completed_strip_file_name = NULL;
	}
	
	dmp_pb_photo_strip_smite_builder(working);
	return return_value;
}