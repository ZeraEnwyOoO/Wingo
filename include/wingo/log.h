/*
 * Wingo — P2P Internet Sharing Tool
 * Copyright (C) 2024 Wingo Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WINGO_LOG_H
#define WINGO_LOG_H

/*
 * ============================================================================
 * WINGO LOGGING
 * ============================================================================
 *
 * This header provides:
 *   - Log levels
 *   - Log output targets
 *   - Log functions
 *   - Log macros
 *   - Thread-safe logging
 *
 * ============================================================================
 */

#include "wingo/common.h"

/* ============================================================================
 * LOG LEVELS
 * ============================================================================ */

/*
 * Log levels, in order of increasing severity.
 *
 * TRACE:   Very detailed debugging information
 * DEBUG:   Detailed debugging information
 * INFO:    Informational messages
 * NOTICE:  Normal but significant events
 * WARN:    Warning messages
 * ERROR:   Error messages
 * FATAL:   Fatal error messages (program will exit)
 */

typedef enum {
    WINGO_LOG_TRACE     = 0,
    WINGO_LOG_DEBUG     = 1,
    WINGO_LOG_INFO      = 2,
    WINGO_LOG_NOTICE    = 3,
    WINGO_LOG_WARN      = 4,
    WINGO_LOG_ERROR     = 5,
    WINGO_LOG_FATAL     = 6,
    WINGO_LOG_NONE      = 7,    /* Disable all logging */
} wingo_log_level_t;

/* ============================================================================
 * LOG OUTPUT TARGETS
 * ============================================================================ */

/*
 * Log output targets.
 *
 * Multiple targets can be combined with bitwise OR.
 */

#define WINGO_LOG_TARGET_NONE       0x00
#define WINGO_LOG_TARGET_STDOUT     0x01
#define WINGO_LOG_TARGET_STDERR     0x02
#define WINGO_LOG_TARGET_FILE       0x04
#define WINGO_LOG_TARGET_SYSLOG     0x08
#define WINGO_LOG_TARGET_ALL        0xFF

/* ============================================================================
 * LOG FLAGS
 * ============================================================================ */

/*
 * Log flags.
 */

#define WINGO_LOG_FLAG_NONE         0x0000
#define WINGO_LOG_FLAG_COLOR        0x0001  /* Use ANSI colors */
#define WINGO_LOG_FLAG_TIMESTAMP    0x0002  /* Include timestamp */
#define WINGO_LOG_FLAG_LEVEL        0x0004  /* Include log level */
#define WINGO_LOG_FLAG_FILE         0x0008  /* Include source file */
#define WINGO_LOG_FLAG_LINE         0x0010  /* Include source line */
#define WINGO_LOG_FLAG_FUNC         0x0020  /* Include function name */
#define WINGO_LOG_FLAG_THREAD       0x0040  /* Include thread ID */
#define WINGO_LOG_FLAG_PID          0x0080  /* Include process ID */
#define WINGO_LOG_FLAG_ASYNC        0x0100  /* Asynchronous logging */
#define WINGO_LOG_FLAG_FLUSH        0x0200  /* Flush after each message */
#define WINGO_LOG_FLAG_DEFAULT      (WINGO_LOG_FLAG_COLOR | \
                                     WINGO_LOG_FLAG_TIMESTAMP | \
                                     WINGO_LOG_FLAG_LEVEL)

/* ============================================================================
 * LOG CONFIGURATION
 * ============================================================================ */

/*
 * Log configuration structure.
 */

#define WINGO_LOG_MAX_FILE_SIZE     (10 * 1024 * 1024)  /* 10 MB */
#define WINGO_LOG_MAX_FILES         5

typedef struct {
    wingo_log_level_t   level;          /* Minimum log level */
    wingo_u32           targets;        /* Output targets (bitmask) */
    wingo_u32           flags;          /* Log flags (bitmask) */
    char                file_path[WINGO_MAX_PATH];  /* Log file path */
    wingo_size          max_file_size;  /* Max file size before rotation */
    int                 max_files;      /* Max number of rotated files */
    bool                rotate;         /* Enable file rotation */
    bool                flush;          /* Flush after each message */
} wingo_log_config_t;

/* ============================================================================
 * LOG FUNCTIONS
 * ============================================================================ */

/*
 * Initialize logging system.
 *
 * @param config    Log configuration (NULL for defaults)
 * @return          WINGO_SUCCESS on success, error code on failure
 */

wingo_error_t wingo_log_init(const wingo_log_config_t *config);

/*
 * Shutdown logging system.
 *
 * Flushes and closes all log outputs.
 */

void wingo_log_shutdown(void);

/*
 * Set log level.
 *
 * @param level     New log level
 */

void wingo_log_set_level(wingo_log_level_t level);

/*
 * Get current log level.
 *
 * @return          Current log level
 */

wingo_log_level_t wingo_log_get_level(void);

/*
 * Set log targets.
 *
 * @param targets   New targets (bitmask)
 */

void wingo_log_set_targets(wingo_u32 targets);

/*
 * Get current log targets.
 *
 * @return          Current targets (bitmask)
 */

wingo_u32 wingo_log_get_targets(void);

/*
 * Set log flags.
 *
 * @param flags     New flags (bitmask)
 */

void wingo_log_set_flags(wingo_u32 flags);

/*
 * Get current log flags.
 *
 * @return          Current flags (bitmask)
 */

wingo_u32 wingo_log_get_flags(void);

/*
 * Set log file path.
 *
 * @param path      New log file path
 * @return          WINGO_SUCCESS on success, error code on failure
 */

wingo_error_t wingo_log_set_file(const char *path);

/*
 * Check if a log level is enabled.
 *
 * @param level     Log level to check
 * @return          true if enabled, false otherwise
 */

bool wingo_log_is_enabled(wingo_log_level_t level);

/*
 * Log a message.
 *
 * @param level     Log level
 * @param file      Source file
 * @param line      Source line
 * @param func      Function name
 * @param fmt       Format string
 * @param ...       Format arguments
 */

void wingo_log_write(wingo_log_level_t level,
                     const char *file,
                     int line,
                     const char *func,
                     const char *fmt, ...)
    WINGO_ATTR_FORMAT(5, 6);

/*
 * Log a message with va_list.
 *
 * @param level     Log level
 * @param file      Source file
 * @param line      Source line
 * @param func      Function name
 * @param fmt       Format string
 * @param args      Format arguments
 */

void wingo_log_vwrite(wingo_log_level_t level,
                      const char *file,
                      int line,
                      const char *func,
                      const char *fmt,
                      va_list args);

/*
 * Flush log outputs.
 */

void wingo_log_flush(void);

/*
 * Rotate log files.
 *
 * @return          WINGO_SUCCESS on success, error code on failure
 */

wingo_error_t wingo_log_rotate(void);

/* ============================================================================
 * LOG MACROS
 * ============================================================================ */

/*
 * Log at TRACE level.
 */

#define WINGO_LOG_TRACE(...) \
    wingo_log_write(WINGO_LOG_TRACE, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*
 * Log at DEBUG level.
 */

#define WINGO_LOG_DEBUG(...) \
    wingo_log_write(WINGO_LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*
 * Log at INFO level.
 */

#define WINGO_LOG_INFO(...) \
    wingo_log_write(WINGO_LOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*
 * Log at NOTICE level.
 */

#define WINGO_LOG_NOTICE(...) \
    wingo_log_write(WINGO_LOG_NOTICE, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*
 * Log at WARN level.
 */

#define WINGO_LOG_WARN(...) \
    wingo_log_write(WINGO_LOG_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*
 * Log at ERROR level.
 */

#define WINGO_LOG_ERROR(...) \
    wingo_log_write(WINGO_LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*
 * Log at FATAL level and abort.
 */

#define WINGO_LOG_FATAL(...) \
    do { \
        wingo_log_write(WINGO_LOG_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        wingo_log_flush(); \
        abort(); \
    } while (0)

/*
 * Conditional log macros.
 */

#define WINGO_LOG_TRACE_IF(cond, ...) \
    do { if ((cond)) WINGO_LOG_TRACE(__VA_ARGS__); } while (0)

#define WINGO_LOG_DEBUG_IF(cond, ...) \
    do { if ((cond)) WINGO_LOG_DEBUG(__VA_ARGS__); } while (0)

#define WINGO_LOG_INFO_IF(cond, ...) \
    do { if ((cond)) WINGO_LOG_INFO(__VA_ARGS__); } while (0)

#define WINGO_LOG_WARN_IF(cond, ...) \
    do { if ((cond)) WINGO_LOG_WARN(__VA_ARGS__); } while (0)

#define WINGO_LOG_ERROR_IF(cond, ...) \
    do { if ((cond)) WINGO_LOG_ERROR(__VA_ARGS__); } while (0)

/*
 * Log with error context.
 */

#define WINGO_LOG_ERR_CTX(ctx) \
    do { \
        if ((ctx) != NULL) { \
            WINGO_LOG_ERROR("%s: %s (at %s:%d in %s)", \
                wingo_error_name((ctx)->code), \
                (ctx)->message, \
                (ctx)->file, \
                (ctx)->line, \
                (ctx)->func); \
        } \
    } while (0)

/* ============================================================================
 * LOG HELPER MACROS
 * ============================================================================ */

/*
 * Log a hex dump.
 */

#define WINGO_LOG_HEX(level, data, len) \
    wingo_log_hex(level, __FILE__, __LINE__, __func__, data, len)

void wingo_log_hex(wingo_log_level_t level,
                   const char *file,
                   int line,
                   const char *func,
                   const void *data,
                   wingo_size len);

/*
 * Log a buffer.
 */

#define WINGO_LOG_BUF(level, buf) \
    wingo_log_buf(level, __FILE__, __LINE__, __func__, buf)

void wingo_log_buf(wingo_log_level_t level,
                   const char *file,
                   int line,
                   const char *func,
                   const wingo_buf_t *buf);

/* ============================================================================
 * COLOR CODES
 * ============================================================================ */

/*
 * ANSI color codes for log levels.
 */

#define WINGO_COLOR_RESET       "\033[0m"
#define WINGO_COLOR_BLACK       "\033[30m"
#define WINGO_COLOR_RED         "\033[31m"
#define WINGO_COLOR_GREEN       "\033[32m"
#define WINGO_COLOR_YELLOW      "\033[33m"
#define WINGO_COLOR_BLUE        "\033[34m"
#define WINGO_COLOR_MAGENTA     "\033[35m"
#define WINGO_COLOR_CYAN        "\033[36m"
#define WINGO_COLOR_WHITE       "\033[37m"

#define WINGO_COLOR_BOLD        "\033[1m"
#define WINGO_COLOR_DIM         "\033[2m"
#define WINGO_COLOR_UNDERLINE   "\033[4m"
#define WINGO_COLOR_BLINK       "\033[5m"
#define WINGO_COLOR_REVERSE     "\033[7m"

/*
 * Color for each log level.
 */

#define WINGO_LOG_COLOR_TRACE   WINGO_COLOR_DIM
#define WINGO_LOG_COLOR_DEBUG   WINGO_COLOR_CYAN
#define WINGO_LOG_COLOR_INFO    WINGO_COLOR_GREEN
#define WINGO_LOG_COLOR_NOTICE  WINGO_COLOR_BLUE
#define WINGO_LOG_COLOR_WARN    WINGO_COLOR_YELLOW
#define WINGO_LOG_COLOR_ERROR   WINGO_COLOR_RED
#define WINGO_LOG_COLOR_FATAL   WINGO_COLOR_RED WINGO_COLOR_BOLD

/* ============================================================================
 * END OF HEADER
 * ============================================================================ */

#endif /* WINGO_LOG_H */
