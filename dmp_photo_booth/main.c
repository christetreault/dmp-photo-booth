/* 
 * File:   main.c
 * Author: chris
 *
 * Created on October 1, 2013, 9:12 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "module.h"
#include "user_interface.h"
#include "console_queue.h"
#include "configuration.h"

#define DMP_PB_DEFAULT_CONFIGURATION "dmp_photo_booth.rc"

/**
 * Called upon a failure in main. Cleans up resources and returns FAILURE
 * @param error the error that was thrown
 * @return DMP_PB_FAILURE
 */
static gint dmp_pb_failure(GError * error)
{
	dmp_pb_console_queue_push(dmp_pb_error_to_string(error));
	g_error_free(error);
		
	dmp_pb_console_queue_flush_stdout();
		
	dmp_pb_config_finalize();
	dmp_pb_console_queue_finalize();
	return DMP_PB_FAILURE;
}

int main(int argc, char** argv)
{
	GError * error = NULL;
	GString * config_path = g_string_new(DMP_PB_DEFAULT_CONFIGURATION);
	
	gtk_init(&argc, &argv);
	dmp_pb_console_queue_init();
	dmp_pb_config_initialize(config_path, &error);
	g_string_free(config_path, TRUE);
	
	if (error != NULL) return dmp_pb_failure(error);
	
	dmp_pb_ui_launch("dmp_photo_booth_ui.glade", &error);
	
	if (error != NULL) return dmp_pb_failure(error);
	
	dmp_pb_config_finalize();
	dmp_pb_console_queue_finalize();
	return DMP_PB_SUCCESS;
}

