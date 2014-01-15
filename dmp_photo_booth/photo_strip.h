/* 
 * File:   photo_strip.h
 * Author: chris
 *
 * Created on December 2, 2013, 2:07 PM
 */

#ifndef PHOTO_STRIP_H
#define	PHOTO_STRIP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <wand/MagickWand.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "configuration.h"
#include "console_queue.h"
#include "module.h"
#include "coordination.h"

	/**
	 * Enumeration of Photo Strip GError codes
	 */
	typedef ExceptionType dmp_pb_photo_strip_error;
	
	/**
	 * calculates a new scaled width
     * @param width the current width
     * @param old_height the current height
     * @param new_height the desired height
     * @return the scaled width
     */
	gint dmp_pb_photo_strip_calculate_new_width(gdouble width, 
												gdouble old_height,
												gdouble new_height);
	
	/**
	 * calculates a new scaled height
     * @param height the current height
     * @param old_width the current width
     * @param new_height the desired width
     * @return the scaled height
     */
	gint dmp_pb_photo_strip_calculate_new_height(gdouble height,
												 gdouble old_width,
												 gdouble new_width);
	
	/**
	 * calculates the size of the border around the photo strip images
     * @param total_height the height of the background image
	 * @param image_height the height of the last photo strip image
     * @return the border size
     */
	gint dmp_pb_photo_strip_calculate_whitespace(gint total_height, gint image_height);
	
	/**
	 * initializes the photo strip module
     */
	void dmp_pb_photo_strip_init(void);
	
	/**
	 * finalizes the photo strip module
     */
	void dmp_pb_photo_strip_finalize(void);
	
	/**
	 * Request that the photo strip module combine the 5 images and the 
	 * background, and save them to completed_strip_name
     * @param completed_strip_name the file name of the resulting image
     * @param pos_1_name the file name of image position 1, or NULL
     * @param pos_2_name the file name of image position 2, or NULL
     * @param pos_3_name the file name of image position 3, or NULL
     * @param pos_4_name the file name of image position 4, or NULL
     * @param pos_5_name the file name of image position 5, or NULL
	 * @NOTE The photo_strip_builder takes ownership of all passed-in GStrings!
     */
	void dmp_pb_photo_strip_request(GString * completed_strip_name,
												GString * pos_1_name,
												GString * pos_2_name,
												GString * pos_3_name,
												GString * pos_4_name,
												GString * pos_5_name);
	
	/**
	 * Pulls a photo strip request from the in queue, and assembles it.
	 * Completed requests can be retrieved by calling dmp_pb_photo_strip_get_result()
     */
	void dmp_pb_photo_strip_assemble(void);

	/**
	 * Retrieves the next result from the queue. Either throws if assemble
	 * encountered an error, or returns a string containing the file name of an 
	 * assembled photo strip
	 * @param out_path a location to return the path of the resulting strip, or 
	 * NULL. If out_path is NULL, no value is returned. *out_path <b>MUST</b> be
	 * NULL, or you will leak memory. Rest assured that if some random pointer
	 * is going down, you're going down with it!
     * @throws DMP_PB_PHOTO_STRIP_ERROR
     * @return A GdkPixbuf of the new strip, or NULL on error, or if there is no
	 * new results
     */
	GdkPixbuf * dmp_pb_photo_strip_get_result(GString ** out_path, GError ** error);
	
	/**
	 * tests to see if the photo strip module is initialized
     * @return TRUE if it is, FALSE if not
     */
	gboolean dmp_pb_photo_strip_initialized(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PHOTO_STRIP_H */

