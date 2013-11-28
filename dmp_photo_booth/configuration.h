/* 
 * File:   configuration.h
 * Author: chris
 *
 * Created on November 27, 2013, 8:29 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DMP_PB_CONFIG_MODULE_GROUP "module"
#define DMP_PB_CONFIG_CAMERA_MODULE_PATH "camera_module_path"
#define DMP_PB_CONFIG_TRIGGER_MODULE_PATH "trigger_module_path"
#define DMP_PB_CONFIG_PRINTER_MODULE_PATH "printer_module_path"
	
#define DMP_PB_CONFIG_CORE_GROUP "core"
#define DMP_PB_CONFIG_BACKGROUND_PATH "background_path"
#define DMP_PB_CONFIG_POSITION_TOGGLE "positon_toggle"
	
#include <glib.h>
	
	/**
	 * Initializes the config
     * @param path the path to read from. Caller remains responsible for this
	 * pointer
     * @throws GFileError, GKeyFileError
     */
	void dmp_pb_config_initialize(GString * path, GError ** error);
	
	/**
	 * Finalizes the config, freeing resources
     */
	void dmp_pb_config_finalize();
	
	/**
	 * Writes the config back to the specified path
     * @param path the path to save to
     * @throws
     */
	void dmp_pb_config_write(GString * path, GError ** error);
	
	/**
	 * reads a string from the config. 
	 * @NOTE Use a constant defined in this header to ensure you are
	 * reading a valid key or group!
     * @param group the group to read from
     * @param key the key to read
     * @return a newly-allocated GString containing the value read, or an empty
	 * GString if the value was not found
     */
	GString * dmp_pb_config_read_string(const gchar * group, const gchar * key);
	
	/**
	 * writes a string to the config
	 * @NOTE Use a constant defined in this header to ensure you are
	 * reading a valid key or group!
     * @param group
     * @param key
     * @param value
     */
	void dmp_pb_config_write_string(const gchar * group, const gchar * key, GString * value);


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIGURATION_H */

