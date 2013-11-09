/* 
 * File:   main.c
 * Author: chris
 *
 * Created on October 1, 2013, 9:12 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "module.h"
#include "user_interface.h"
#include "console_queue.h"

int main(int argc, char** argv) {

	gint return_value = DMP_PB_SUCCESS;
	
	gtk_init(&argc, &argv);
	dmp_pb_console_queue_init();
	
	dmp_pb_ui_launch("dmp_photo_booth_ui.glade");
	
	dmp_pb_console_queue_finalize();

	return return_value;
}

