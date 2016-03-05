/**
 * @file   log.h
 * @author Chinmay Kamat <chinmaykamat@cmu.edu>
 * @date   Thu, 14 February 2013 21:40:08 EST
 *
 * @brief Provides logging functionality.
 *
 *         Logging can be enabled at compile time per file as required by
 *         #define ing value of LOG_LEVEL before including log.h
 *
 *         Provides 4 levels of logging
 *
 *         1 = Error logs (default)
 *         2 = Debug logs
 *         3 = Verbose logs
 *
 *         Each higher level includes the previous one.
 *
 *         Error logs are written to stderr all other logs to stdout
 */


#ifndef _LOG_H_
#define _LOG_H_

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#define print_err() (errno == 0 ? "" : strerror(errno))

#if (LOG_LEVEL > 0)
#define error_log(FMT, ...)                             \
    fprintf(stderr, "[ERROR] (%s:%d) " FMT "\n",        \
            __FILE__, __LINE__, __VA_ARGS__);           \
    fflush(stdout);
#else
#define error_log(...)
#endif

#if (LOG_LEVEL > 1)
#define debug_log(FMT, ...)                             \
    fprintf(stdout, "[DEBUG] (%s:%d) " FMT "\n",        \
            __FILE__, __LINE__, __VA_ARGS__);           \
    fflush(stdout);
#else
#define debug_log(...)
#endif

#if (LOG_LEVEL > 2)
#define verbose_log(FMT, ...)                           \
    fprintf(stdout, "[VERBOSE] (%s:%d) " FMT "\n",      \
            __FILE__, __LINE__, __VA_ARGS__);           \
    fflush(stdout);
#else
#define verbose_log(...)
#endif

#endif
