/*
 * File:   dmp_trigger_module_tests.c
 * Author: chris
 *
 * Created on Oct 8, 2013, 3:24:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "dmp_trigger_module.h"

/*
 * Simple C Test Suite
 */

void th()
{
	printf("test handler\n");
}

void testDmp_tm_add_trigger_handler() {
	int result = dmp_tm_add_trigger_handler(th);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_tm_add_trigger_handler (dmp_trigger_module_tests) message=Returned: %d\n", result);
	}
}

void testDmp_tm_set_countdown() {
	int current;
	int result = dmp_tm_set_countdown(current);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_tm_set_countdown (dmp_trigger_module_tests) message=Returned: %d\n", result);
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% dmp_trigger_module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_tm_add_trigger_handler (dmp_trigger_module_tests)\n");
	testDmp_tm_add_trigger_handler();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_add_trigger_handler (dmp_trigger_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_tm_set_countdown (dmp_trigger_module_tests)\n");
	testDmp_tm_set_countdown();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_set_countdown (dmp_trigger_module_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
