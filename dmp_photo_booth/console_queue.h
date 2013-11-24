/* 
 * File:   console_queue.h
 * Author: chris
 *
 * Created on October 24, 2013, 8:09 PM
 */

#ifndef CONSOLE_QUEUE_H
#define	CONSOLE_QUEUE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "global_defines.h"
#include <gtk/gtk.h>

	/**
	 * For whatever reason, gtk.h does not typedef this
	 */
	typedef struct _GtkTextBuffer GtkTextBuffer;

	/**
	 * Pushes the passed-in string into the queue. If the push fails because 
	 * the queue is not initialized, the passed-in GString is freed. This 
	 * function is thread safe
	 * @param message the message to push
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_console_queue_push(GString * message);

	/**
	 * Pops a string from the queue. The caller is responsible for memory
	 * management of the popped string. This function is thread safe.
	 * @return the string at the front of the queue, or NULL on error.
	 */
	GString * dmp_pb_console_queue_pop();

	/**
	 * Checks the value at the front of the queue, but does not pop it. Caller
	 * should not attempt to free the returned string. This function is 
	 * thread safe.
	 * @return the string at the front of the queue, or NULL on error.The queue 
	 * retains responsibility for this pointer
	 */
	GString * dmp_pb_console_queue_peek();

	/**
	 * Tests to see if the queue is empty
	 * @return true if it is, false if it isn't
	 */
	gboolean dmp_pb_console_queue_is_empty();

	/**
	 * initializes the console queue. All queue functions will fail and return
	 * DMP_PB_CONSOLE_QUEUE_NOT_INITIALIZED until this function has been called.
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_console_queue_init();

	/**
	 * Finalizes the console queue, ensuring all memory is freed.
	 * @return DMP_PB_SUCCESS, or an error code
	 */
	gint dmp_pb_console_queue_finalize();

	/**
	 * flushes the contents of the console queue, pushing it into the passed-in
	 * GtkTextBuffer
	 * @param user_data a GtkTextBuffer to push messages into
	 * @return G_SOURCE_CONTINUE. This function should not stop until gtk_main_quit
	 * has been called
	 */
	gboolean dmp_pb_console_queue_flush_queue(gpointer user_data);

#ifdef	__cplusplus
}
#endif

#endif	/* CONSOLE_QUEUE_H */

