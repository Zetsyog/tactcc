#ifndef LOGGER_H
#define LOGGER_H
/**
 *
 * Un simple fichier pour le log
 *
 */

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 100
#define LOG_LEVEL_INFO 400
#define LOG_LEVEL_DEBUG 500
#define LOG_LEVEL_ALL __INT_MAX__

/**
 *  Initialize logger data
 *  Must be called before using any log function
 */
void log_init();

/**
 *  Define the current log level
 *  Every logged message at a level above will be ignored
 */
void set_log_level(int level);

/**
 * Log a message at a given level
 * @param level the message log level
 * @param message a formatable string to log
 */
void log_level(int level, const char *message, ...);

/**
 * Log at level DEBUG a message
 * @param message a formatable string to log
 */
void log_error(const char *message, ...);

/**
 * Log at level DEBUG a message
 * @param message a formatable string to log
 */
void log_info(const char *message, ...);

/**
 * Log at level DEBUG a message
 * @param message a formatable string to log
 */
void log_debug(const char *message, ...);

#endif