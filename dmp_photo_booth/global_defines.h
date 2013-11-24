/* 
 * File:   global_defines.h
 * Author: chris
 *
 * Created on October 9, 2013, 1:50 PM
 * 
 * This header contains global #define statements
 */

#ifndef GLOBAL_DEFINES_H
#define	GLOBAL_DEFINES_H

#ifdef	__cplusplus
extern "C" {
#endif

	/* Error code for when there is a bad error code. So meta... */
#define DMP_PB_INVALID_ERROR_CODE -1

	/* All's well that ends well; a successful return */
#define DMP_PB_SUCCESS 0

	/* Error due to null pointer being detected */
#define DMP_PB_NULL_POINTER 1

	/* Attempted to load a module that is already loaded */
#define DMP_PB_MODULE_ALREADY_LOADED 2

	/* Attempted to use/unload a module that is not loaded */
#define DMP_PB_MODULE_NOT_LOADED 3

	/* g_module_supported() returned false */
#define DMP_PB_G_MODULE_NOT_SUPPORTED 4

	/* Invalid module type passed to module function */
#define DMP_PB_INVALID_MODULE_TYPE 5

	/* Module is only partially loaded. Call unload or reload and try again */
#define DMP_PB_MODULE_IN_INCONSISTENT_STATE 6

	/* Module load/unload operation failed */
#define DMP_PB_MODULE_LOAD_UNLOAD_FAILED 7

	/* Attempted to overwrite a pointer already in use. Potential memory leak */
#define DMP_PB_POINTER_ALREADY_IN_USE 8

	/* GLib operation failed */
#define DMP_PB_GLIB_CALL_FAILED 9

	/* Attempted to call a console queue function before calling 
	 * dmp_pb_console_queue_init() */
#define DMP_PB_CONSOLE_QUEUE_NOT_INITIALIZED 10

	/* GTK operation failed */
#define DMP_PB_GTK_CALL_FAILED 11

	/* Attempted to call UI function while the UI is not initialized */
#define DMP_PB_UI_NOT_INITIALIZED 12

	/* Attempted to initialize UI when it is already initialized */
#define DMP_PB_UI_ALREADY_INITIALIZED 13

	/* Attempted an operation that by design does not work */
#define DMP_PB_NO_OP 14
	
	/* Attempted to load config while config already loaded */
#define DMP_PB_CONFIG_ALREADY_LOADED 15
	
	/* Attempted to access config while config not loaded */
#define DMP_PB_CONFIG_NOT_LOADED 16
	
	/* Module not initialized */
#define DMP_PB_MODULE_NOT_INITIALIZED 17
	
	/* Module already initialized */
#define DMP_PB_MODULE_ALREADY_INITIALIZED 18


#ifdef	__cplusplus
}
#endif

#endif	/* GLOBAL_DEFINES_H */

