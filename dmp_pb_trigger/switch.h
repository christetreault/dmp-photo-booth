/* 
 * File:   switch.h
 * Author: chris
 *
 * Created on December 7, 2013, 10:58 PM
 */

#ifndef SWITCH_H
#define	SWITCH_H

#include "serial.h"

namespace dmp
{
	namespace trigger
	{
		typedef enum
		{
			TRIGGER_BUTTON = 8
		}switch_pin;
		
		/**
		 * Handles the trigger button
         */
		void handle_trigger_button();
	}
}

#endif	/* SWITCH_H */

