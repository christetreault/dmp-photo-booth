/* 
 * File:   configuration.h
 * Author: chris
 *
 * Created on November 13, 2013, 2:57 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <glib.h>
	
#define DMP_PB_CONFIG_FILE ".dmp_pb_config.rc"
	
	/**
	 * loads the config
     * @return DMP_PB_SUCCESS, or an error code
     */
	gint dmp_pb_config_load();
	
	/**
	 * reads a string from the config
     * @param key the key to read
     * @return the value to be read
     */
	const gchar * dmp_pb_config_read_string(const gchar * key);
	
	/**
	 * reads an integer from the config
     * @param key the key to read
     * @return the value to be read
     */
	gint dmp_pb_config_read_int(const gchar * key);
	
	/**
	 * reads a boolean value from the config
     * @param key the key to read
     * @return the value to be read
     */
	gboolean dmp_pb_config_read_boolean(const gchar * key);
	
	/**
	 * sets a string value in the config
     * @param key the key to set
     * @param val the value to set
     * @return DMP_PB_SUCCESS, or an error code
     */
	gint dmp_pb_config_set_string(const gchar * key, gchar * val);
	
	/**
	 * sets an integer value in the config
     * @param key the key to set
     * @param val the value to set
     * @return DMP_PB_SUCCESS, or an error code
     */
	gint dmp_pb_config_set_int(const gchar * key, gint val);
	
	/**
	 * sets a boolean value in the config
     * @param key the key to set
     * @param val the value to set
     * @return DMP_PB_SUCCESS, or an error code
     */
	gint dmp_pb_config_set_boolean(const gchar * key, gboolean val);
	
	/**
	 * writes the config back to the config file.
     * @return DMP_PB_SUCCESS, or an error code
     */
	gint dmp_pb_config_save();
	
	/**
	 * finalizes the config
     * @return DMP_PB_SUCCESS, or an error code
     */
	gint dmp_pb_config_finalize();


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIGURATION_H */

