/*
 * File:   module_callbacks_tests.c
 * Author: chris
 *
 * Created on Nov 16, 2013, 6:51:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "module_callbacks.h"
#include "error_handling.h"

#define TEST_MESSAGE "Test Message"
/*
 * Simple C Test Suite
 */

void testDmp_pb_console_write_callback() {
	int result = dmp_pb_console_write_callback(TEST_MESSAGE);
	GString * message = dmp_pb_console_queue_pop();
	gint pop_result = DMP_PB_SUCCESS;
	gint strcmp_result;
	
	if (message == NULL) pop_result = dmp_pb_get_last_error_code();
	if (pop_result == DMP_PB_SUCCESS) strcmp_result = g_strcmp0(TEST_MESSAGE, message->str);
	else strcmp_result = DMP_PB_NULL_POINTER;
	
	if (result != DMP_PB_SUCCESS || pop_result != DMP_PB_SUCCESS ||
			strcmp_result != 0) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_write_callback (module_callbacks_tests) message=Test failed! Result = %d, pop result = %d, strcmp result = %d\n", result, pop_result, strcmp_result);
	}
}

/* TODO: currently untestable
void testDmp_pb_trigger_handler() {
	dmp_pb_trigger_handler();
	if () {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_trigger_handler (module_callbacks_tests) message=error message sample\n");
	}
}
*/

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% module_callbacks_tests\n");
	printf("%%SUITE_STARTED%%\n");

	dmp_pb_console_queue_init();
	
	printf("%%TEST_STARTED%%  testDmp_pb_console_write_callback (module_callbacks_tests)\n");
	testDmp_pb_console_write_callback();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_write_callback (module_callbacks_tests)\n");

	//printf("%%TEST_STARTED%%  testDmp_pb_trigger_handler (module_callbacks_tests)\n");
	//testDmp_pb_trigger_handler();
	//printf("%%TEST_FINISHED%% time=0 testDmp_pb_trigger_handler (module_callbacks_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	dmp_pb_console_queue_finalize();
	
	return (EXIT_SUCCESS);
}
