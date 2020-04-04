//
// Created by Cory Todd on 4/4/2020.
//

#pragma once

#include <stdbool.h>

// DLLEXTERNAL is for members visible outside the Shared Library.
// DLLINTERNAL is for members visible only inside the Shared Library.
// MODULEINTERNAL is for members visible only inside the module.

#ifdef _WIN32
#ifdef COMPILING_ZFORCE_PROXY_SDK
#define DLLEXTERNAL __declspec(dllexport)
#else
#define DLLEXTERNAL __declspec(dllimport)
#endif // COMPILING_ZFORCE_PROXY_SDK
#define DLLINTERNAL
#define MODULEINTERNAL static
#elif defined (__clang__) || __GNUC__ > 4
#define DLLEXTERNAL __attribute__ ((visibility ("default")))
    #define DLLINTERNAL __attribute__ ((visibility ("hidden")))
    #define MODULEINTERNAL static
#else
    #error Unsupported compiler.
#endif // _WIN32

// Helper macro to force the debugger to break at this location.
#if defined(WIN32) && !defined(__MINGW32__)
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK assert(0)
#endif

#if NDEBUG
#define ASSERT_ENABLED 0
#define ASSERT(x, ...) ((void)0)
#define ALWAYS_ASSERT(...) ((void)0)
#else

#define LOG_LEVEL_ASSERT (0)
#define LOG_LEVEL_INFO (1)
#define LOG_LEVEL_ERROR (2)
/**
 * @brief Logging helper
 * @param level LOG_LEVEL_ASSERT, LOG_LEVEL_INFO, LOG_LEVEL_ERROR
 * @param test assertion test (if level == 0)
 * @param func calling function
 * @param file containing function
 * @param line line of function in file
 * @param fmt printf format string
 * @param ... printf format args
 * @retval false assertion failed
 * @retval true no assertion or assertion passed
 */
bool logMessage(int level, const char *test, const char *func, const char *file, size_t line, const char *fmt, ...);
#define ASSERT_ENABLED 1
#define ASSERT(x, ...) (!!(x) || logMessage(LOG_LEVEL_ASSERT, #x, __func__, __FILE__,__LINE__ , ## __VA_ARGS__ ) || (DEBUG_BREAK, true))
#define ALWAYS_ASSERT(...) (logMessage(LOG_LEVEL_ASSERT, NULL, __func__, __FILE__,__LINE__ , ## __VA_ARGS__ ) || (DEBUG_BREAK, true))
#endif

#if ENABLE_LOGGING == 1
#define LOG_INFO(...) logMessage(LOG_LEVEL_INFO, NULL, __func__, __FILE__,__LINE__ , ## __VA_ARGS__ )
#define LOG_ERROR(...) logMessage(LOG_LEVEL_ERROR, NULL, __func__, __FILE__,__LINE__ , ## __VA_ARGS__ )
#else
#define LOG_INFO(...) ((void)0)
#define LOG_ERROR(...) ((void)0)
#endif