/*
 * File:   dmp_printer_module_tests.c
 * Author: chris
 *
 * Created on Oct 8, 2013, 2:57:21 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "dmp_printer_module.h"

/*
 * Simple C Test Suite
 */

void testDmp_pm_print() {
	char* to_print = "/tmp";
	int result = dmp_pm_print(to_print);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pm_print (dmp_printer_module_tests) message=Returned: %d\n", result);
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% dmp_printer_module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_pm_print (dmp_printer_module_tests)\n");
	testDmp_pm_print();
	printf("%%TEST_FINISHED%% time=0 testDmp_pm_print (dmp_printer_module_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
