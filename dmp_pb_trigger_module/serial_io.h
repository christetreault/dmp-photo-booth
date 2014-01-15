/* 
 * File:   serial_io.h
 * Author: chris
 *
 * Created on December 10, 2013, 10:09 AM
 */

#ifndef SERIAL_IO_H
#define	SERIAL_IO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <glib.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include "dmp_trigger_module.h"
#include "trigger_config.h"
	
	typedef enum
	{
		OUTPUT_NONE = 0,
		OUTPUT_BUTTON_PRESS = 1 << 0
	}dmp_tm_output_flag;

	typedef enum
	{
		INPUT_NONE = 0,
		INPUT_COUNT_0 = 1 << 0,
		INPUT_COUNT_1 = 1 << 1,
		INPUT_COUNT_2 = 1 << 2,
		INPUT_COUNT_3 = 1 << 3,
		INPUT_COUNT_4 = 1 << 4
	}dmp_tm_input_flag;

	typedef enum
	{
		IS_ERROR = 1 << 7,
		NULL_ERROR = 0,
		TRIGGER_MODULE_ERROR = 1 << 0, 
		CAMERA_MODULE_ERROR = 1 << 1, 
		PRINTER_MODULE_ERROR = 1 << 2,
		PHOTO_BOOTH_CORE_ERROR = 1 << 3
	}dmp_tm_error_code;
	
	/**
	 * Opens a serial connection to the trigger 
     */
	void dmp_tm_io_start_serial(void);
	
	/**
	 * closes the serial connection to the trigger
     */
	void dmp_tm_io_stop_serial(void);
	
	/**
	 * writes a byte to the trigger
     * @param the byte to queue
     */
	void dmp_tm_io_write_byte(gint8 to_write);
	
	/**
     * @return TRUE if the serial thread is running, false if not
     */
	gboolean dmp_tm_io_thread_running(void);
	
	


#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_IO_H */

