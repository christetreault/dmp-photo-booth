#include "photo_strip.h"


G_DEFINE_QUARK(DMP_PB_PHOTO_STRIP_ERROR, dmp_pb_photo_strip_error)

static GAsyncQueue * in_queue = NULL;
static GAsyncQueue * out_queue = NULL;
static GThread * dmp_pb_photo_strip_thread;

static gboolean is_initialized = FALSE;

struct photo_strip_builder
{
	MagickWand * background_wand;
	MagickWand * working_wand;
	
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
	while (dmp_pb_photo_strip_initialized())
	{
		dmp_pb_photo_strip_assemble();
		g_usleep(G_USEC_PER_SEC * 5);
	}
	return NULL;
}

gboolean dmp_pb_photo_strip_initialized(void)
{
	return (is_initialized && in_queue != NULL && out_queue != NULL);
}

static void dmp_pb_free_and_delete_individual_image(GString * file_name)
{
	if (file_name == NULL) return;
	
	if (g_file_test(file_name->str, G_FILE_TEST_EXISTS))
	{
		g_unlink(file_name->str);
	}
	
	g_string_free(file_name, TRUE);
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
	dmp_pb_free_and_delete_individual_image(to_smite->position_1_file_name);
	dmp_pb_free_and_delete_individual_image(to_smite->position_2_file_name);
	dmp_pb_free_and_delete_individual_image(to_smite->position_3_file_name);
	dmp_pb_free_and_delete_individual_image(to_smite->position_4_file_name);
	dmp_pb_free_and_delete_individual_image(to_smite->position_5_file_name);
	
	if (to_smite->working_wand != NULL) DestroyMagickWand(to_smite->working_wand);
	if (to_smite->background_wand != NULL)DestroyMagickWand(to_smite->background_wand);
	
	if (to_smite->error_message != NULL) g_string_free(to_smite->error_message, TRUE);
	if (to_smite->thumbnail != NULL) g_object_unref(to_smite->thumbnail);
	
	g_free(to_smite);
}

void dmp_pb_photo_strip_init(void)
{
	if (dmp_pb_photo_strip_initialized()) return;
	
	if (in_queue == NULL) in_queue = g_async_queue_new_full((GDestroyNotify) dmp_pb_photo_strip_smite_builder);
	if (out_queue == NULL) out_queue = g_async_queue_new_full((GDestroyNotify) dmp_pb_photo_strip_smite_builder);
	if (!is_initialized) MagickWandGenesis();
	is_initialized = TRUE;
	
	dmp_pb_photo_strip_thread = g_thread_new("Photo Strip Thread", dmp_pb_photo_strip_thread_function, NULL);
	
}

void dmp_pb_photo_strip_finalize(void)
{
	if (in_queue != NULL) g_async_queue_unref(in_queue);
	in_queue = NULL;
	if (out_queue != NULL) g_async_queue_unref(out_queue);
	out_queue = NULL;
	if (is_initialized) MagickWandTerminus();
	is_initialized = FALSE;
}

/**
 * given a MagickWand and a width, returns the scaled height of the current image
 */
static gint dmp_pb_photo_strip_scale_height_magickally(MagickWand * wand, gint width)
{
	return dmp_pb_photo_strip_calculate_new_height(
			(gdouble) MagickGetImageHeight(wand),
			(gdouble) MagickGetImageWidth(wand), 
			width);
}

/**
 * given a MagickWand and a height, returns the scaled width of the current image
 */
static gint dmp_pb_photo_strip_scale_width_magickally(MagickWand * wand, gint height)
{
	return dmp_pb_photo_strip_calculate_new_width(
			(gdouble) MagickGetImageWidth(wand),
			(gdouble) MagickGetImageHeight(wand),
			height);
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
	working->error_message = NULL;
	working->thumbnail = NULL;
	
	g_async_queue_push(in_queue, working);
}

static gint dmp_pb_photo_strip_initialize_total_height(const struct photo_strip_builder * builder,
																double width)
{
	g_assert(builder != NULL);
	MagickWand * working;
	GString * working_path;
	gint return_value = 0;
	
	if (builder->position_1_file_name != NULL) working_path = builder->position_1_file_name;
	else if (builder->position_2_file_name != NULL) working_path = builder->position_2_file_name;
	else if (builder->position_3_file_name != NULL) working_path = builder->position_3_file_name;
	else if (builder->position_4_file_name != NULL) working_path = builder->position_4_file_name;
	else if (builder->position_5_file_name != NULL) working_path = builder->position_5_file_name;
	else return return_value;
	
	working = NewMagickWand();
	
	if (MagickReadImage(working, working_path->str))
	{
		return_value = dmp_pb_photo_strip_scale_height_magickally(working, width);
	}
	
	DestroyMagickWand(working);
	return return_value;
}

void dmp_pb_photo_strip_assemble(void)	//	PHOTO STRIP! ASEEEEEEEEEMBLE!!!!!!
{
	if (!dmp_pb_photo_strip_initialized()) return;
	
	/* -------- */
	/* Creation */
	/* -------- */
	GAsyncQueue * local_in_queue = g_async_queue_ref(in_queue);
	struct photo_strip_builder * working = g_async_queue_try_pop(local_in_queue);
	g_async_queue_unref(local_in_queue);
	
	if (working == NULL) return; // No requests available
	
	gchar * magick_exception_message = NULL;
	dmp_pb_photo_strip_error magick_exception_severity;
	gdouble width = dmp_pb_config_read_double(DMP_PB_CONFIG_CORE_GROUP, DMP_PB_CONFIG_INDIVIDUAL_IMAGE_WIDTH);
	gint last_height;
	
	GAsyncQueue * local_out_queue = g_async_queue_ref(out_queue);
	
	if (!(last_height = dmp_pb_photo_strip_initialize_total_height(working, width)))
	{
		working->error_message = g_string_new("Photo Request contains no images!");
		working->error_domain = dmp_pb_photo_strip_error_quark();
		working->error_code = FileOpenError;
		g_async_queue_push(local_out_queue, working);
		g_async_queue_unref(local_out_queue);
		return;
	}
	
	gint offset_x = 15;
	gint offset_y = 15;
	gint total_height = 0;
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
		g_async_queue_push(local_out_queue, working);
		g_async_queue_unref(local_out_queue);
		g_string_free(background, TRUE);
		return;
	}
	g_string_free(background, TRUE);
	total_height = MagickGetImageHeight(working->background_wand) - 30;
	
	if (working->position_1_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_1_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		
		if (!MagickResizeImage(working->working_wand, 
				(gint) width, 
				last_height = dmp_pb_photo_strip_scale_height_magickally(working->working_wand, width),
				LanczosFilter, 
				1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickCompositeImage(working->background_wand, working->working_wand,
									OverCompositeOp, offset_x, offset_y))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
	}
	
	offset_y = offset_y + last_height + dmp_pb_photo_strip_calculate_whitespace(total_height, last_height);
	
	if (working->position_2_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_2_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickResizeImage(working->working_wand, 
				(gint) width, 
				last_height = dmp_pb_photo_strip_scale_height_magickally(working->working_wand, width),
				LanczosFilter, 
				1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickCompositeImage(working->background_wand, working->working_wand,
									OverCompositeOp, offset_x, offset_y))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
	}
	
	offset_y = offset_y + last_height + dmp_pb_photo_strip_calculate_whitespace(total_height, last_height);
	
	if (working->position_3_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_3_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickResizeImage(working->working_wand, 
				(gint) width, 
				last_height = dmp_pb_photo_strip_scale_height_magickally(working->working_wand, width),
				LanczosFilter, 
				1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickCompositeImage(working->background_wand, working->working_wand,
									OverCompositeOp, offset_x, offset_y))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
	}
	
	offset_y = offset_y + last_height + dmp_pb_photo_strip_calculate_whitespace(total_height, last_height);
	
	if (working->position_4_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_4_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickResizeImage(working->working_wand, 
				(gint) width, 
				last_height = dmp_pb_photo_strip_scale_height_magickally(working->working_wand, width),
				LanczosFilter, 
				1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickCompositeImage(working->background_wand, working->working_wand,
									OverCompositeOp, offset_x, offset_y))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
	}
	
	offset_y = offset_y + last_height + dmp_pb_photo_strip_calculate_whitespace(total_height, last_height);
	
	if (working->position_5_file_name != NULL)
	{
		if (!MagickReadImage(working->working_wand, working->position_5_file_name->str))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickResizeImage(working->working_wand, 
				(gint) width, 
				last_height = dmp_pb_photo_strip_scale_height_magickally(working->working_wand, width),
				LanczosFilter, 
				1.0))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
			return;
		}
		if (!MagickCompositeImage(working->background_wand, working->working_wand,
									OverCompositeOp, offset_x, offset_y))
		{
			magick_exception_message = MagickGetException(working->working_wand, &magick_exception_severity);
			working->error_message = g_string_new(magick_exception_message);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = magick_exception_severity;
			MagickRelinquishMemory(magick_exception_message);
			g_async_queue_push(local_out_queue, working);
			g_async_queue_unref(local_out_queue);
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
		g_async_queue_push(local_out_queue, working);
		g_async_queue_unref(local_out_queue);
		return;
	}
	
	/* ----- */
	/* Saving*/
	/* ----- */

	MagickSetFirstIterator(working->background_wand);
	if (!MagickWriteImage(working->background_wand, working->completed_strip_file_name->str))
	{
		magick_exception_message = MagickGetException(working->background_wand, &magick_exception_severity);
		working->error_message = g_string_new(magick_exception_message);
		working->error_domain = dmp_pb_photo_strip_error_quark();
		working->error_code = magick_exception_severity;
		MagickRelinquishMemory(magick_exception_message);
		g_async_queue_push(local_out_queue, working);
		g_async_queue_unref(local_out_queue);
		return;
	}
	/*-------------*/
	/* Cleaning up */
	/*-------------*/
	
	if (working->position_1_file_name != NULL) g_unlink(working->position_1_file_name);
	if (working->position_2_file_name != NULL) g_unlink(working->position_2_file_name);
	if (working->position_3_file_name != NULL) g_unlink(working->position_3_file_name);
	if (working->position_4_file_name != NULL) g_unlink(working->position_4_file_name);
	if (working->position_5_file_name != NULL) g_unlink(working->position_5_file_name);
	
	/* -------- */
	/* Printing */
	/* -------- */
	
	if (dmp_pb_pm_print(working->completed_strip_file_name->str, &error) != DMP_PB_SUCCESS)
	{
		if (error != NULL)
		{
			working->error_message = g_string_new(error->message);
			working->error_domain = error->domain;
			working->error_code = error->code;
			g_clear_error(&error);
		}
		else
		{
			working->error_message = g_string_new(NULL);
			g_string_printf(working->error_message, "Failed to print photo strip: %s",
							working->completed_strip_file_name->str);
			working->error_domain = dmp_pb_photo_strip_error_quark();
			working->error_code = DMP_PB_FAILURE;
		}
		g_async_queue_push(local_out_queue, working);
		g_async_queue_unref(local_out_queue);
		dmp_pb_tm_show_error(PRINTER_MODULE_ERROR, NULL);
		return;
	}
	
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
				dmp_pb_photo_strip_calculate_new_width(gdk_pixbuf_get_width(full),
					gdk_pixbuf_get_height(full),
					256.0),
				256,
				GDK_INTERP_NEAREST
			);
		g_object_unref(full);
	}
	
	/* ---------------- */
	/* One last push... */
	/* ---------------- */
	
	g_async_queue_push(local_out_queue, working);
	g_async_queue_unref(local_out_queue);
}

GdkPixbuf * dmp_pb_photo_strip_get_result(GString ** out_path, GError ** error)
{
	g_assert(dmp_pb_photo_strip_initialized());
	if (out_path != NULL) g_assert(*out_path == NULL);
	
	GAsyncQueue * local_out_queue = g_async_queue_ref(out_queue);
	struct photo_strip_builder * working = g_async_queue_try_pop(local_out_queue);
	g_async_queue_unref(local_out_queue);
	
	if (working == NULL) return NULL; // No new results
	
	if (working->error_message != NULL)
	{
		g_set_error(error,
				working->error_domain,
				working->error_code,
				"%s", working->error_message->str);
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

gint dmp_pb_photo_strip_calculate_new_width(gdouble width, 
											gdouble old_height,
											gdouble new_height)
{
	return (gint) (width * (new_height / old_height));
}

gint dmp_pb_photo_strip_calculate_new_height(gdouble height,
											 gdouble old_width,
											 gdouble new_width)
{
	return (gint) (height * (new_width / old_width));
}

gint dmp_pb_photo_strip_calculate_whitespace(gint total_height, gint image_height)
{
	g_assert(total_height > 0 && image_height > 0);
	g_assert(total_height >= (image_height * 5));
	
	return ((total_height - (image_height * 5)) / 4);
}
