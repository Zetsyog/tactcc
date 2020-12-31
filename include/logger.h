/**
 * @file
 * Un simple fichier pour le log
 *
 */
#ifndef LOGGER_H
#define LOGGER_H


#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 100
#define LOG_LEVEL_INFO 400
#define LOG_LEVEL_DEBUG 500
#define LOG_LEVEL_ALL __INT_MAX__

extern int syntax_error;

/**
 *  @brief Initialize logger data
 *  Must be called before using any log function
 */
void log_init();

/**
 *  @brief Define the current log level
 *  Every logged message at a level above will be ignored
 */
void set_log_level(int level);

/**
 * @brief Log a message at a given level
 * @param level the message log level
 * @param message a formatable string to log
 * @param ...
 */
void log_level(int level, const char *message, ...);

/**
 * @brief Log at level ERROR a message
 * Output in stderr
 * @param message a formatable string to log
 * @param ...
 */
void log_error(const char *message, ...);

/**
 * @brief Log at level INFO a message
 * @param message a formatable string to log
 * @param ...
 */
void log_info(const char *message, ...);

/**
 * @brief Log at level DEBUG a message
 * @param message a formatable string to log
 * @param ...
 */
void log_debug(const char *message, ...);

/**
 * @brief Print a syntax error in stderr
 * Calls first print_scanner_current_line() and then write the error
 * 
 * @see print_scanner_current_line()
 * 
 * @param message 
 * @param ... 
 */
void log_syntax_error(const char *message, ...);

#endif
