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

	/**
	 * Converts a GError to a string
     * @param error the error to convert
     * @return a newly allocated GString initialized by the passed-in GError
     */
	GString * dmp_pb_error_to_string(GError * error);


#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_HANDLING_H */

