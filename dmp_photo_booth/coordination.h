/* 
 * File:   coordination.h
 * Author: chris
 *
 * Created on December 5, 2013, 9:56 AM
 */

#ifndef COORDINATION_H
#define	COORDINATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>

	/**
	 * Assuming you call this function at most 1 time per microsecond per prefix
	 * /extension pair, it will return a unique file name in the form of:
	 * 
	 * <i>[prefix]_[epoch_time].[extension]</i>
	 * @NOTE According to the GLib docs, some platforms cannot printfify a gint64.
	 * Unfortunately for us, it doesn't say what platforms these are. Hopefully
	 * it's not a common issue. Either way, if you are on one of these platforms,
	 * this function returns a "unique" file name in the form of:
	 * 
	 * <i>[prefix]_[random_integer].[extension]</i>
     * @param prefix Some human readable prefix. i.e. "photo_strip" or
	 * "console_output"
     * @param extension The file type. i.e. png or txt
     * @return The newly generated path. This pointer should be freed using
	 * g_free()
     */
	gchar * dmp_pb_coordination_get_epoch_filename(const gchar * prefix, const gchar * extension);
	
#ifdef	__cplusplus
}
#endif

#endif	/* COORDINATION_H */
