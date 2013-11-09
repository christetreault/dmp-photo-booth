/*
 * File:   trigger_module_tests.c
 * Author: chris
 *
 * Created on Oct 15, 2013, 12:33:32 PM
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

int tc(char * test_message)
{
	return DMP_PB_SUCCESS;
}

void testDmp_tm_install_console() {
	int result = dmp_tm_install_console(tc);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_tm_install_console (dmp_trigger_module_tests) message=Returned: %d\n", result);
	}
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

void testDmp_tm_edit_config() {
	int result = dmp_tm_edit_config();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_tm_edit_config (dmp_trigger_module_tests) message=Returned: %d\n");
	}
}

void testDmp_tm_get_config_location() {
	size_t size = 20;
	char* to_fill = malloc(sizeof(char) * size);
	char* result = dmp_tm_get_config_location(to_fill, size);
	if (result == NULL) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_tm_get_config_location (dmp_trigger_module_tests) message=Returned: NULL\n");
	}
}

void testDmp_tm_load_config() {
	int result = dmp_tm_load_config();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_tm_load_config (dmp_trigger_module_tests) message=Returned: %d\n");
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% trigger_module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_tm_install_console (trigger_module_tests)\n");
	testDmp_tm_install_console();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_install_console (trigger_module_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_tm_add_trigger_handler (trigger_module_tests)\n");
	testDmp_tm_add_trigger_handler();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_add_trigger_handler (trigger_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_tm_edit_config (trigger_module_tests)\n");
	testDmp_tm_edit_config();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_edit_config (trigger_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_tm_get_config_location (trigger_module_tests)\n");
	testDmp_tm_get_config_location();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_get_config_location (trigger_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_tm_load_config (trigger_module_tests)\n");
	testDmp_tm_load_config();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_load_config (trigger_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_tm_set_countdown (trigger_module_tests)\n");
	testDmp_tm_set_countdown();
	printf("%%TEST_FINISHED%% time=0 testDmp_tm_set_countdown (trigger_module_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
