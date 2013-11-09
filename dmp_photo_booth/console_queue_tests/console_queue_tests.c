
/*
 * File:   console_queue_tests.c
 * Author: chris
 *
 * Created on Oct 27, 2013, 11:31:02 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "console_queue.h"

#define STRING_1 "String 1"
#define STRING_2 "String 2"
#define STRING_3 "String 3"


/*
 * Simple C Test Suite
 */

void testDmp_pb_console_queue_finalize() {
	gint result = dmp_pb_console_queue_finalize();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_queue_finalize (console_queue_tests) message=Test failed, returned: %d\n", result);
	}
}

void testDmp_pb_console_queue_init() {
	gint result = dmp_pb_console_queue_init();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_queue_init (console_queue_tests) message=Test failed, returned: %d\n", result);
	}
}

void testDmp_pb_console_queue_is_empty() {
	gboolean result = dmp_pb_console_queue_is_empty();
	if (result) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_queue_is_empty (console_queue_tests) message=Test failed, returned: %d\n", result);
	}
}

void testDmp_pb_console_queue_peek() {
	GString* result = dmp_pb_console_queue_peek();
	if (g_strcmp0(result->str, STRING_1) != 0) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_queue_peek (console_queue_tests) message=Test failed, returned: %s, should be %s\n", result->str, STRING_1);
	}
}

void testDmp_pb_console_queue_pop() {
	GString* result1 = dmp_pb_console_queue_pop();
	GString* result2 = dmp_pb_console_queue_pop();
	GString* result3 = dmp_pb_console_queue_pop();
	gboolean result_empty = dmp_pb_console_queue_is_empty();
	
	if (g_strcmp0(result1->str, STRING_1) != 0 || g_strcmp0(result2->str, STRING_2) != 0 ||
			g_strcmp0(result3->str, STRING_3) != 0 || result_empty != TRUE) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_queue_pop (console_queue_tests) message=Test failed, returned: %s, %s, %s, %d, should be %s, %s, %s, %d\n", result1, result2, result3, result_empty, STRING_1, STRING_2, STRING_3, TRUE);
	}
}

void testDmp_pb_console_queue_push() {
	GString * str1 = g_string_new(STRING_1);
	GString * str2 = g_string_new(STRING_2);
	GString * str3 = g_string_new(STRING_3);
	
	gint result1 = dmp_pb_console_queue_push(str1);
	gint result2 = dmp_pb_console_queue_push(str2);
	gint result3 = dmp_pb_console_queue_push(str3);
	if (result1 != DMP_PB_SUCCESS || result2 != DMP_PB_SUCCESS || result3 != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_console_queue_push (console_queue_tests) message=Test failed, returned: %d, %d, %d\n", result1, result2, result3);
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% console_queue_tests\n");
	printf("%%SUITE_STARTED%%\n");
	
	printf("%%TEST_STARTED%%  testDmp_pb_console_queue_init (console_queue_tests)\n");
	testDmp_pb_console_queue_init();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_queue_init (console_queue_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pb_console_queue_push (console_queue_tests)\n");
	testDmp_pb_console_queue_push();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_queue_push (console_queue_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_pb_console_queue_is_empty (console_queue_tests)\n");
	testDmp_pb_console_queue_is_empty();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_queue_is_empty (console_queue_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pb_console_queue_peek (console_queue_tests)\n");
	testDmp_pb_console_queue_peek();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_queue_peek (console_queue_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pb_console_queue_pop (console_queue_tests)\n");
	testDmp_pb_console_queue_pop();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_queue_pop (console_queue_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pb_console_queue_finalize (console_queue_tests)\n");
	testDmp_pb_console_queue_finalize();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_console_queue_finalize (console_queue_tests)\n");
	
	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
