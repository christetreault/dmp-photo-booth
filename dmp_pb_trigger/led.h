/* 
 * File:   led.h
 * Author: chris
 *
 * Created on December 7, 2013, 10:47 PM
 */

#ifndef LED_H
#define	LED_H

#include <Arduino.h>

	namespace dmp
	{
		namespace trigger
		{
			typedef enum
			{
				COUNT_NULL_LED = 0,
				COUNT_0_LED = 9,
				COUNT_1_LED = 10,
				COUNT_2_LED = 11,
				COUNT_3_LED = 12,
				COUNT_4_LED = 13
			}led_pin;
			
			/**
			 * Lights up the LEDs corresponding the the passed-in value
             * @param value the value to show
             */
			void show_value(led_pin value);
			
			/**
			 * Sets all output LEDs to LOW
             */
			void clear();
			
			/**
			 * Shows an error value. With 5 LEDs, the trigger is capable of
			 * outputting values 0 - 31
			 * @param the error code to display
             */
			void show_error(unsigned code);
		}
	}



#endif	/* LED_H */

