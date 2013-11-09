/*
 * File:   module_tests.c
 * Author: chris
 *
 * Created on Oct 9, 2013, 3:46:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "module.h"

/*
 * Simple C Test Suite
 */

#define DMP_PB_CAMERA_MODULE_LOCATION "test_camera_module.so"
#define DMP_PB_PRINTER_MODULE_LOCATION "test_printer_module.so"
#define DMP_PB_TRIGGER_MODULE_LOCATION "test_trigger_module.so"

gboolean load_camera_passed = FALSE;
gboolean camera_consistent = TRUE;
gboolean load_trigger_passed = FALSE;
gboolean trigger_consistent = TRUE;
gboolean load_printer_passed = FALSE;
gboolean printer_consistent = TRUE;

void testDmp_pb_is_loaded() {
	gboolean cam = dmp_pb_is_loaded(DMP_PB_CAMERA_MODULE) == load_camera_passed;
	gboolean tri = dmp_pb_is_loaded(DMP_PB_TRIGGER_MODULE) == load_trigger_passed;
	gboolean pri = dmp_pb_is_loaded(DMP_PB_PRINTER_MODULE) == load_printer_passed;
	if (!(cam && tri && pri)) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_is_loaded (module_tests) message=Function returns don't match recorded results. Camera: %d, Trigger: %d, Printer: %d\n", cam, tri, pri);
	}
}

void testDmp_pb_is_consistent() {
	gboolean cam = dmp_pb_is_consistent(DMP_PB_CAMERA_MODULE) == camera_consistent;
	gboolean tri = dmp_pb_is_consistent(DMP_PB_TRIGGER_MODULE) == trigger_consistent;
	gboolean pri = dmp_pb_is_consistent(DMP_PB_PRINTER_MODULE) == printer_consistent;
	if (!(cam && tri && pri)) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_is_loaded (module_tests) message=Function returns don't match recorded results. Camera: %d, Trigger: %d, Printer: %d\n", cam, tri, pri);
	}
}

void testDmp_pb_load_module() {
	GString * camera_module = g_string_new(DMP_PB_CAMERA_MODULE_LOCATION);
	GString * printer_module = g_string_new(DMP_PB_PRINTER_MODULE_LOCATION);
	GString * trigger_module = g_string_new(DMP_PB_TRIGGER_MODULE_LOCATION);
	gint cam = dmp_pb_load_module(DMP_PB_CAMERA_MODULE, camera_module);
	load_camera_passed = cam == DMP_PB_SUCCESS;
	if (cam == DMP_PB_MODULE_IN_INCONSISTENT_STATE) camera_consistent = FALSE;
	gint tri = dmp_pb_load_module(DMP_PB_TRIGGER_MODULE, trigger_module);
	load_trigger_passed = tri == DMP_PB_SUCCESS;
	if (tri == DMP_PB_MODULE_IN_INCONSISTENT_STATE) trigger_consistent = FALSE;
	gint pri = dmp_pb_load_module(DMP_PB_PRINTER_MODULE, printer_module);
	load_printer_passed = pri == DMP_PB_SUCCESS;
	if (pri == DMP_PB_MODULE_IN_INCONSISTENT_STATE) printer_consistent = FALSE;
	if (!(load_camera_passed && load_trigger_passed && load_printer_passed)) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_load_module (module_tests) message=Failed to load modules. Camera: %d, Trigger: %d, Printer: %d\n", cam, tri, pri);
	}
	g_string_free(trigger_module, TRUE);
	g_string_free(printer_module, TRUE);
	g_string_free(camera_module, TRUE);
}

void testDmp_pb_unload_module() {
	gint cam = dmp_pb_unload_module(DMP_PB_CAMERA_MODULE);
	load_camera_passed = cam == DMP_PB_SUCCESS;// || cam == DMP_PB_MODULE_NOT_LOADED;
	gint tri = dmp_pb_unload_module(DMP_PB_TRIGGER_MODULE);
	load_trigger_passed = tri == DMP_PB_SUCCESS;// || cam == DMP_PB_MODULE_NOT_LOADED;
	gint pri = dmp_pb_unload_module(DMP_PB_PRINTER_MODULE);
	load_printer_passed = pri == DMP_PB_SUCCESS;// || cam == DMP_PB_MODULE_NOT_LOADED;
	if (!(load_camera_passed && load_trigger_passed && load_printer_passed)) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_unload_module (module_tests) message=Failed to unload modules. Camera: %d, Trigger: %d, Printer: %d\n", cam, tri, pri);
	}
}

void testDmp_pb_swap_module() {
	GString * camera_module = g_string_new(DMP_PB_CAMERA_MODULE_LOCATION);
	GString * printer_module = g_string_new(DMP_PB_PRINTER_MODULE_LOCATION);
	GString * trigger_module = g_string_new(DMP_PB_TRIGGER_MODULE_LOCATION);
	gint pri = dmp_pb_swap_module(DMP_PB_PRINTER_MODULE, printer_module);
	gint cam = dmp_pb_swap_module(DMP_PB_CAMERA_MODULE, camera_module);
	gint tri = dmp_pb_swap_module(DMP_PB_TRIGGER_MODULE, trigger_module);
	if (pri != DMP_PB_SUCCESS || cam != DMP_PB_SUCCESS || tri != DMP_PB_SUCCESS) {
		printf("%%TEST_FAILED%% time=0 testname=testDmp_pb_unload_module (module_tests) message=Failed to swap modules. Camera: %d, Trigger: %d, Printer: %d\n", cam, tri, pri);
	}
	g_string_free(trigger_module, TRUE);
	g_string_free(printer_module, TRUE);
	g_string_free(camera_module, TRUE);
}

int main(int argc, char** argv) {
	printf("%%SUITE_STARTING%% module_tests\n");
	printf("%%SUITE_STARTED%%\n");

	printf("%%TEST_STARTED%%  testDmp_pb_load_module (module_tests)\n");
	testDmp_pb_load_module();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_load_module (module_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_pb_is_loaded (module_tests)\n");
	testDmp_pb_is_loaded();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_is_loaded (module_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_pb_is_consistent (module_tests)\n");
	testDmp_pb_is_consistent();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_is_consistent (module_tests)\n");
	
	printf("%%TEST_STARTED%%  testDmp_pb_swap_module (module_tests)\n");
	testDmp_pb_swap_module();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_swap_module (module_tests)\n");

	printf("%%TEST_STARTED%%  testDmp_pb_unload_module (module_tests)\n");
	testDmp_pb_unload_module();
	printf("%%TEST_FINISHED%% time=0 testDmp_pb_unload_module (module_tests)\n");
	
	printf("%%SUITE_FINISHED%% time=0\n");

	return (EXIT_SUCCESS);
}
