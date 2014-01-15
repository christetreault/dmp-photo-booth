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

#define DMP_PB_DEFAULT_CONFIGURATION "dmp_photo_booth.rc"
	
#define DMP_PB_CONFIG_MODULE_GROUP "module"
#define DMP_PB_CONFIG_CAMERA_MODULE_PATH "camera_module_path"
#define DMP_PB_CONFIG_TRIGGER_MODULE_PATH "trigger_module_path"
#define DMP_PB_CONFIG_PRINTER_MODULE_PATH "printer_module_path"
	
#define DMP_PB_CONFIG_CORE_GROUP "core"
#define DMP_PB_CONFIG_BACKGROUND_PATH "background_path"
#define DMP_PB_CONFIG_POSITION_TOGGLE "positon_toggle"
#define DMP_PB_CONFIG_INDIVIDUAL_IMAGE_PATH "individual_image_path"
#define DMP_PB_CONFIG_COMPLETED_STRIP_PATH "completed_strip_path"
#define DMP_PB_CONFIG_INDIVIDUAL_IMAGE_WIDTH "individual_image_width"
//#define DMP_PB_CONFIG_INDIVIDUAL_IMAGE_ASPECT_RATIO "individual_image_aspect_ratio"
	
#define DMP_PB_PHOTO_STRIP_POSITION_1_FLAG 1 << 0
#define DMP_PB_PHOTO_STRIP_POSITION_2_FLAG 1 << 1
#define DMP_PB_PHOTO_STRIP_POSITION_3_FLAG 1 << 2
#define DMP_PB_PHOTO_STRIP_POSITION_4_FLAG 1 << 3
#define DMP_PB_PHOTO_STRIP_POSITION_5_FLAG 1 << 4
	
#include <glib.h>
	
	/**
	 * Initializes the config
     * @throws GFileError, GKeyFileError
     */
	void dmp_pb_config_initialize(GError ** error);
	
	/**
	 * Finalizes the config, freeing resources.
	 * @NOTE: This function <b>DOES NOT</b> attempt to save the config to file!
     */
	void dmp_pb_config_finalize(void);
	
	/**
	 * Writes the config back to file
     * @throws G_FILE_ERROR
     */
	void dmp_pb_config_write(GError ** error);
	
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
     * @param group the group to write to
     * @param key the key to write
     * @param value the value to set
     */
	void dmp_pb_config_write_string(const gchar * group, const gchar * key, const GString * value);
	
	/**
	 * reads an integer from the config
	 * @NOTE Use a constant defined in this header to ensure you are
	 * reading a valid key or group!
     * @param group the group to read from
     * @param key the key to read
     * @return the read value
     */
	gint dmp_pb_config_read_int(const gchar * group, const gchar * key);
	
	/**
	 * writes an integer to the config
	 * @NOTE Use a constant defined in this header to ensure you are
	 * reading a valid key or group!
     * @param group the group to write to
     * @param key the key to write to
     * @param value the value to write
     */
	void dmp_pb_config_write_int(const gchar * group, const gchar * key, gint value);
	
		/**
	 * reads a double from the config
	 * @NOTE Use a constant defined in this header to ensure you are
	 * reading a valid key or group!
     * @param group the group to read from
     * @param key the key to read
     * @return the read value
     */
	gdouble dmp_pb_config_read_double(const gchar * group, const gchar * key);
	
	/**
	 * writes a double to the config
	 * @NOTE Use a constant defined in this header to ensure you are
	 * reading a valid key or group!
     * @param group the group to write to
     * @param key the key to write to
     * @param value the value to write
     */
	void dmp_pb_config_write_double(const gchar * group, const gchar * key, gdouble value);


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIGURATION_H */

