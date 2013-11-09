/*
 * File:   camera_module_tests.c
 * Author: chris
 *
 * Created on Oct 8, 2013, 2:52:25 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "camera_module.h"

/*
 * Simple C Test Suite
 */

void testDmp_cm_capture() {
	int result = dmp_cm_capture();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_capture (camera_module_tests) message=Returned: %d\n", result);
	}
}

void testDmp_cm_download() {
	char* location = "/tmp";
	int result = dmp_cm_download(location);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_download (camera_module_tests) message=Returned: %d\n", result);
	}
}

void testDmp_cm_get_config_location() {
	char * result = dmp_cm_get_config_location();
	if (result == NULL) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_capture (camera_module_tests) message=Returned: NULL\n", result);
	}
}

void testDmp_cm_capture() {
	int result = dmp_cm_capture();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_capture (camera_module_tests) message=Returned: %d\n", result);
	}
}

void testDmp_cm_capture() {
	int result = dmp_cm_capture();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_capture (camera_module_tests) message=Returned: %d\n", result);
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% camera_module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_cm_capture (camera_module_tests)\n");
	testDmp_cm_capture();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_capture (camera_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_cm_download (camera_module_tests)\n");
	testDmp_cm_download();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_download (camera_module_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
