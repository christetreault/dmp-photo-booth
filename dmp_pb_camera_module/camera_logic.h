/* 
 * File:   camera_logic.h
 * Author: chris
 *
 * Created on January 3, 2014, 9:20 PM
 */

#ifndef CAMERA_LOGIC_H
#define	CAMERA_LOGIC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include <gphoto2/gphoto2.h>
#include <glib/gstdio.h>
#include <fcntl.h>
	
	/**
	 * Initializes the camera
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_cm_camera_init();
	
	/**
	 * Finalizes the camera
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_cm_camera_finalize();
	
	/**
	 * signal the camera to capture and download to the passed-in location
     * @param location the location to download to
     * @return DMP_PB_SUCCESS or DMP_PB_FAILURE
     */
	gint dmp_cm_camera_capture(gchar * location);


#ifdef	__cplusplus
}
#endif

#endif	/* CAMERA_LOGIC_H */

