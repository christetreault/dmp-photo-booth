/* 
 * File:   bitwise.h
 * Author: chris
 *
 * Created on November 25, 2013, 10:58 AM
 */

#ifndef BITWISE_MAGIC_H
#define	BITWISE_MAGIC_H

#include "led.h"
#include <HardwareSerial.h>

	namespace dmp
	{
		namespace trigger
		{

			typedef enum
			{
				OUTPUT_NONE = 0,
				OUTPUT_BUTTON_PRESS = 1 << 0
			}output_flag;
			
			typedef enum
			{
				INPUT_NONE = 0,
				INPUT_COUNT_0 = 1 << 0,
				INPUT_COUNT_1 = 1 << 1,
				INPUT_COUNT_2 = 1 << 2,
				INPUT_COUNT_3 = 1 << 3,
				INPUT_COUNT_4 = 1 << 4
			}input_flag;
			
			typedef enum
			{
				IS_ERROR = 1 << 7,
				NULL_ERROR = 0,
				TRIGGER_MODULE_ERROR = 1 << 0, 
				CAMERA_MODULE_ERROR = 1 << 1, 
				PRINTER_MODULE_ERROR = 1 << 2,
				PHOTO_BOOTH_CORE_ERROR = 1 << 3
			}error_code;

			/**
			 * Reads one byte from the serial connection
             * @return the read byte
             */
			int read();
			
			/**
			 * writes one byte to the serial connection
             * @param to_write the byte to write
             */
			void write(char to_write);
			
			/**
			 * resolves a value read from the serial port to a pin
             * @param byte
             * @return 
             */
			led_pin resolve_pin(int byte);
			
			/**
			 * Resolves a value read from the serial port to an error code
             * @param byte
             * @return 
             */
			unsigned resolve_error(int byte);
			
			/**
			 * test a byte to see if its an error code
             * @param byte the byte to test
             * @return true if byte is an error code
             */
			bool is_error(int byte);

		}
	}

#endif	/* BITWISE_MAGIC_H */

