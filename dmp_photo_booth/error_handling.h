/* 
 * File:   error_handling.h
 * Author: chris
 *
 * Created on November 1, 2013, 9:33 PM
 */

#ifndef ERROR_HANDLING_H
#define	ERROR_HANDLING_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "global_defines.h"
#include <glib.h>

	typedef enum
	{
		IS_ERROR = 1 << 7,
		NULL_ERROR = 0,
		TRIGGER_MODULE_ERROR = 1 << 0, 
		CAMERA_MODULE_ERROR = 1 << 1, 
		PRINTER_MODULE_ERROR = 1 << 2,
		PHOTO_BOOTH_CORE_ERROR = 1 << 3
	}dmp_pb_tm_error_code;
	
	/**
	 * Converts a GError to a string
     * @param error the error to convert
     * @return a newly allocated GString initialized by the passed-in GError
     */
	GString * dmp_pb_error_to_string(const GError * error);


#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_HANDLING_H */

