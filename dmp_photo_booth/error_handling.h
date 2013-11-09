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
	 * returns the last error code. This would be any return values that are not
	 * DMP_PB_SUCCESS. If a function that does not return gint fails, it will
	 * also set the error code, which can be obtained by calling this function
     * @return the last error code. Returns DMP_PB_SUCCESS if nothing has failed
     */
	gint dmp_pb_get_last_error_code();
	
	/**
	 * Sets the error code. This will be returned by dmp_pb_get_last_error_code.
	 * This function is thread safe
     * @param to_set the code to set.
     * @return DMP_PB_SUCCESS, or DMP_PB_NO_OP if called with DMP_PB_SUCCESS
     */
	gint dmp_pb_set_error_code(gint to_set);
	
	/**
	 * resolves an error code into a string
     * @param to_resolve the code to resolve
     * @return a string describing the error code, or NULL if DMP_PB_INVALID_ERROR_CODE
     */
	GString * dmp_pb_resolve_error_code(gint to_resolve);

	/**
	 * Calls dmp_pb_set_error_code, then returns the value that was set. Useful
	 * for when you'd just do return SOME_VALUE. This function is thread safe.
     * @param to_set the code to set. Causes an assertion failure if called with
	 * DMP_PB_SUCCESS
     * @return the value that was set
     */
	gint dmp_pb_set_error_code_return(gint to_set);

#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_HANDLING_H */

