/*
 * File:   printer_module_tests.c
 * Author: chris
 *
 * Created on Oct 15, 2013, 12:25:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "dmp_printer_module.h"

/*
 * Simple C Test Suite
 */

int tc(char * test_message)
{
	return DMP_PB_SUCCESS;
}

void testDmp_pm_install_console() {
	int result = dmp_pm_install_console(tc);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pm_install_console (printer_module_tests) message=Returned: %d\n", result);
	}
}

void testDmp_pm_edit_config() {
	int result = dmp_pm_edit_config();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pm_edit_config (printer_module_tests) message=Returned: %d\n");
	}
}

void testDmp_pm_get_config_location() {
	size_t size = 20;
	char* to_fill = malloc(sizeof(char) * size);
	char* result = dmp_pm_get_config_location(to_fill, size);
	if (result == NULL) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pm_get_config_location (printer_module_tests) message=Returned: NULL\n");
	}
}

void testDmp_pm_load_config() {
	int result = dmp_pm_load_config();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pm_load_config (printer_module_tests) message=Returned: %d\n");
	}
}

void testDmp_pm_print() {
	char* to_print = "/tmp";
	int result = dmp_pm_print(to_print);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pm_print (dmp_printer_module_tests) message=Returned: %d\n", result);
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% printer_module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_pm_install_console (printer_module_tests)\n");
	testDmp_pm_install_console();
	printf("%%TEST_FINISHED%% time=0 testDmp_pm_install_console (printer_module_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_pm_edit_config (printer_module_tests)\n");
	testDmp_pm_edit_config();
	printf("%%TEST_FINISHED%% time=0 testDmp_pm_edit_config (printer_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pm_get_config_location (printer_module_tests)\n");
	testDmp_pm_get_config_location();
	printf("%%TEST_FINISHED%% time=0 testDmp_pm_get_config_location (printer_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pm_load_config (printer_module_tests)\n");
	testDmp_pm_load_config();
	printf("%%TEST_FINISHED%% time=0 testDmp_pm_load_config (printer_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pm_print (printer_module_tests)\n");
	testDmp_pm_print();
	printf("%%TEST_FINISHED%% time=0 testDmp_pm_print (printer_module_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
