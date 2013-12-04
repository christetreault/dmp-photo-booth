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
	
#define DMP_PB_ASPECT_RATIO_4_3 1.333333
#define DMP_PB_ASPECT_RATIO_3_2 1.5

	/**
	 * Enumeration of Photo Strip GError codes
	 */
	typedef ExceptionType dmp_pb_photo_strip_error;
	
	/**
	 * initializes the photo strip module
     */
	void dmp_pb_photo_strip_init();
	
	/**
	 * finalizes the photo strip module
     */
	void dmp_pb_photo_strip_finalize();
	
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
	void dmp_pb_photo_strip_assemble();

	/**
	 * Retrieves the next result from the queue. Either throws if assemble
	 * encountered an error, or returns a string containing the file name of an 
	 * assembled photo strip
     * @throws DMP_PB_PHOTO_STRIP_ERROR
     * @return the path to a photo strip, or NULL on error, or if there is no
	 * new results
     */
	GString * dmp_pb_photo_strip_get_result(GError ** error);
	
	/**
	 * tests to see if the photo strip module is initialized
     * @return TRUE if it is, FALSE if not
     */
	gboolean dmp_pb_photo_strip_initialized();

#ifdef	__cplusplus
}
#endif

#endif	/* PHOTO_STRIP_H */

