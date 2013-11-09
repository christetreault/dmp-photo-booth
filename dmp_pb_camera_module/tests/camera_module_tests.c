/*
 * File:   camera_module_tests.c
 * Author: chris
 *
 * Created on Oct 15, 2013, 12:13:07 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "dmp_camera_module.h"

/*
 * Simple C Test Suite
 */

int tc(char * test_message)
{
	return DMP_PB_SUCCESS;
}

void testDmp_cm_install_console() {
	int result = dmp_cm_install_console(tc);
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_install_console (camera_module_tests) message=Returned: %d\n", result);
	}
}

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

void testDmp_cm_edit_config() {
	int result = dmp_cm_edit_config();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_edit_config (camera_module_tests) message=Returned: %d\n");
	}
}

void testDmp_cm_get_config_location() {
	size_t size = 20;
	char* to_fill = malloc(sizeof(char) * size);
	char* result = dmp_cm_get_config_location(to_fill, size);
	if (result == NULL) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_get_config_location (camera_module_tests) message=Returned: NULL\n");
	}
	free(to_fill);
}

void testDmp_cm_load_config() {
	int result = dmp_cm_load_config();
	if (result != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_cm_load_config (camera_module_tests) message=Returned: %d\n");
	}
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% camera_module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_cm_install_console (camera_module_tests)\n");
	testDmp_cm_install_console();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_install_console (camera_module_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_cm_capture (camera_module_tests)\n");
	testDmp_cm_capture();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_capture (camera_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_cm_download (camera_module_tests)\n");
	testDmp_cm_download();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_download (camera_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_cm_edit_config (camera_module_tests)\n");
	testDmp_cm_edit_config();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_edit_config (camera_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_cm_get_config_location (camera_module_tests)\n");
	testDmp_cm_get_config_location();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_get_config_location (camera_module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_cm_load_config (camera_module_tests)\n");
	testDmp_cm_load_config();
	printf("%%TEST_FINISHED%% time=0 testDmp_cm_load_config (camera_module_tests)\n");

	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
