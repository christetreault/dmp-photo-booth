/* 
 * File:   error_handling.h
 * Author: chris
 *
 * Created on January 6, 2014, 3:18 PM
 */

#ifndef ERROR_HANDLING_H
#define	ERROR_HANDLING_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include "dmp_camera_module.h"
	
gint dmp_cm_error_console_write(GError * error);


#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_HANDLING_H */

