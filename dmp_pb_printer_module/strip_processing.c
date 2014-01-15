#include "strip_processing.h"

/**
 * Finds the location of the top left corner. if = is the canvas_width, - is
 * the image_width, then * is the returned value:<br><br>
 * ===*--------===
 * @param canvas_width the width, in pixels, of the canvas
 * @param image_width the width, in pixels, of the image
 * @return the X coordinate of the top left corner of the image
 */
static gsize dmp_pm_find_center_origin(gsize canvas_width, gsize image_width)
{
	if (canvas_width < image_width) return 0;
	return (canvas_width / 2) - (image_width / 2);
}

/**
 * Resolves a printer name to a cups_dest_t index
 * @param to_resolve the destination array to resolve
 * @param num_dests the size of to_resolve
 * @param name the name to find
 * @return an index, or -1 if not found
 */
static gint dmp_pm_resolve_dests_index(const cups_dest_t * to_resolve,
										gint num_dests,
										const gchar * name)
{
	int count;
	for (count = 0; count < num_dests; ++count)
	{
		if (g_strcmp0(name, to_resolve[count].name) == 0) return count;
	}
	GString * working = g_string_new(NULL);
	g_string_printf(working, "Invalid printer name: \"%s\"!\n", name);
	dmp_pm_console_write(g_string_free(working, FALSE));
	return -1;
}

/**
 * Executes a print job
 * @param to_print the file to print
 */
static gint dmp_pm_execute_print(const GString * to_print)
{
	cups_dest_t * dests;
	gint num_dests = cupsGetDests(&dests);
	gchar * printer_name = dmp_pm_config_get_printer_name();
	gint dest_index = dmp_pm_resolve_dests_index(dests, num_dests, printer_name);
	g_free(printer_name);
	
	if (dest_index == -1) return DMP_PB_FAILURE;
	
	int job_id = cupsPrintFile(dests[dest_index].name, 
								to_print->str,
								to_print->str, 
								dests[dest_index].num_options, 
								dests[dest_index].options);
	
	cupsFreeDests(num_dests, dests);
	return DMP_PB_SUCCESS;
}

/**
 * Handles a MagickWandException
 * @param exceptional the wand that threw
 */
static void dmp_pm_handle_exception(const MagickWand * exceptional)
{
	gchar * exception_message = NULL;
	ExceptionType exception_type;
	GString * message_builder = g_string_new(NULL);
	
	exception_message = MagickGetException(exceptional, &exception_type);
	g_string_printf(message_builder, "DMP_PM_STRIP_PROCESSING_ERROR %d: %s\n",
					exception_type, exception_message);
	MagickRelinquishMemory(exception_message);
	dmp_pm_console_write(message_builder->str);
	g_string_free(message_builder, TRUE);
}

/**
 * @return a white, fully transparent PixelWand
 */
PixelWand * dmp_pm_create_bg_color(void)
{
	PixelWand * return_value = NewPixelWand();
	
	gchar * exception_message = NULL;
	ExceptionType exception_type;
	GString * message_builder = NULL;
	
	if (G_LIKELY(!PixelSetColor(return_value, "white")))
	{
		message_builder = g_string_new(NULL);
		exception_message = PixelGetException(return_value, &exception_type);
		g_string_printf(message_builder, "DMP_PM_STRIP_PROCESSING_ERROR %d: %s\n",
						exception_type, exception_message);
		MagickRelinquishMemory(exception_message);
		dmp_pm_console_write(message_builder->str);
		g_string_free(message_builder, TRUE);
		DestroyPixelWand(return_value);
		return NULL;
	}
	PixelSetAlpha(return_value, 0.0);
	
	return return_value;
}

gint dmp_pm_process_print(const gchar * to_print)
{
	g_assert(to_print != NULL);
	
	gsize width = dmp_pm_config_get_canvas_width() * dmp_pm_config_get_dpi();
	gsize height = dmp_pm_config_get_canvas_height() * dmp_pm_config_get_dpi();
	gboolean print_to_file = dmp_pm_config_print_to_file();
	
	MagickWand * working = NewMagickWand();
	MagickWand * background = NewMagickWand();
	PixelWand * bg_color = NULL;
	GString * temp_file = NULL;
	
	if (!MagickReadImage(working, to_print))
	{
		dmp_pm_handle_exception(working);
		DestroyMagickWand(working);
		DestroyMagickWand(background);
		return DMP_PB_FAILURE;
	}
	
	if ((bg_color = dmp_pm_create_bg_color()) == NULL) return DMP_PB_FAILURE;
	
	if (!MagickNewImage(background, width, height, bg_color))
	{
		dmp_pm_handle_exception(background);
		DestroyPixelWand(bg_color);
		DestroyMagickWand(working);
		DestroyMagickWand(background);
		return DMP_PB_FAILURE;
	}
	
	DestroyPixelWand(bg_color);
	
	MagickSetLastIterator(working);
	MagickSetLastIterator(background);
	
	if (!MagickCompositeImage(background, 
								working, 
								OverCompositeOp, 
								dmp_pm_find_center_origin(width / 2,
															MagickGetImageWidth(working)), 
								0))
	{
		dmp_pm_handle_exception(background);
		DestroyMagickWand(working);
		DestroyMagickWand(background);
		return DMP_PB_FAILURE;
	}
	
	if (!MagickCompositeImage(background, 
								working, 
								OverCompositeOp, 
								(width / 2) + 
									dmp_pm_find_center_origin(width / 2,
																MagickGetImageWidth(working)), 
								0))
	{
		dmp_pm_handle_exception(background);
		DestroyMagickWand(working);
		DestroyMagickWand(background);
		return DMP_PB_FAILURE;
	}
	
	temp_file = g_string_new(to_print);
	g_string_append(temp_file, "_working.png");
	MagickSetFirstIterator(background);
	
	if (!MagickWriteImage(background, temp_file->str))
	{
		dmp_pm_handle_exception(background);
		DestroyMagickWand(working);
		DestroyMagickWand(background);
		g_string_free(temp_file, TRUE);
		return DMP_PB_FAILURE;
	}
	
	DestroyMagickWand(working);
	DestroyMagickWand(background);

	if (!print_to_file)
	{
		dmp_pm_execute_print(temp_file);
	
		if (g_unlink(temp_file->str) != 0)
		{
			g_string_prepend(temp_file, "Failed to remove temporary file: ");
			g_string_append(temp_file, "!\n");
			dmp_pm_console_write(temp_file->str);
		}
	}

	g_string_free(temp_file, TRUE);
	return DMP_PB_SUCCESS;
}

