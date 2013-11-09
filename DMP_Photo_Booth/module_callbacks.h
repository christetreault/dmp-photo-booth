/* 
 * File:   module_callbacks.h
 * Author: chris
 *
 * Created on October 23, 2013, 12:05 PM
 */

#ifndef MODULE_CALLBACKS_H
#define	MODULE_CALLBACKS_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include <glib.h>
#include <glib/gprintf.h>
#include "global_defines.h"
#include "console_queue.h"

	// TODO: Create Unit Tests!
	
	/**
	 * The callback function to be called by the trigger module when
	 * the user "presses the button"
     */
	void dmp_pb_trigger_handler();
	
	/**
	 * The callback function to be called by all modules to write to
	 * the console. console_queue_pop is thread safe, so by extension so
	 * is this function
     * @param message the message to be written. The caller retains
	 * responsability for the memory management of this pointer
     * @return DMP_PB_SUCCESS, or an error code
     */
	int dmp_pb_console_write_callback(char * message);
	
	


#ifdef	__cplusplus
}
#endif

#endif	/* MODULE_CALLBACKS_H */

