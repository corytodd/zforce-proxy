#pragma clang diagnostic push
#pragma ide diagnostic ignored "clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling"
#pragma ide diagnostic ignored "clang-analyzer-core.CallAndMessage"
//
// Created by Cory Todd on 4/4/2020.
//
#include "zforce_proxy/zforce_proxy_common.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

#if ENABLE_ANSI_COLORS == 1
#define COLOR_INFO "\x1b[36m"
#define COLOR_ERROR "\x1b[31m"
#define COLOR_CLEAR "\x1b[0m"
#else
#define COLOR_INFO ""
#define COLOR_ERROR ""
#define COLOR_CLEAR ""
#endif

#if ASSERT_ENABLED || defined(ENABLE_LOGGING)
bool logMessage(int level, const char *test, const char *func, const char *file, size_t line, const char *fmt, ...) {
    // We have to go with two buffers to get things done
    char reserve0[1024];
    char reserve1[1024];
    const char *buffer0;
    char *buffer1;
    bool allocatedBuffer0 = false;

    if (fmt) {
        va_list list;
        va_start(list, fmt);
        int len = vsnprintf(NULL, 0, fmt, list) + 1;
        va_end(list);
        char *buffer0target;
        if (len < 0) {
            len = sizeof(reserve0);
            buffer0target = reserve0;
        } else if ((size_t)len <= sizeof(reserve0)) {
            buffer0target = reserve0;
        } else {
            buffer0target = malloc(len);
            if (buffer0target == NULL) {
                // Just die, we can't do anything safe
                printf("Failed to allocate logging buffer");
                assert(false);
            }
            allocatedBuffer0 = true;
        }
        va_start(list, fmt);
        vsnprintf(buffer0target, len, fmt, list);
        va_end(list);
        buffer0 = buffer0target;
    } else {
        buffer0 = "No reason provided.";
    }

    const char *formatString;
    switch (level) {
    case 0:
        if (test == NULL) {
            formatString = "(%s(%d): Always assert triggered failed -"
                           "Reason: %s"
                           "At:     %s)";
        } else {
            formatString = "(%s(%d): Assertion \"%s\" failed -"
                           "Reason: %s"
                           "At:     %s )";
        }
        break;
    case 1:
        formatString = COLOR_INFO "%s(%d):%s]:INFO - %s" COLOR_CLEAR;
        break;
    case 2:
        formatString = COLOR_ERROR "[%s(%d):%s]:ERROR - %s" COLOR_CLEAR;
        break;
    }
    if (test == NULL) {
        int len = snprintf(NULL, 0, formatString, file, line, buffer0, func) + 1;
        if (len < 0) {
            len = sizeof(reserve1);
            buffer1 = reserve1;
        } else if ((size_t)len <= sizeof(reserve1)) {
            buffer1 = reserve1;
        } else {
            buffer1 = malloc(len);
        }
        snprintf(buffer1, len, formatString, file, line, func, buffer0);
    } else {
        int len = snprintf(NULL, 0, formatString, file, line, test, buffer0, func) + 1;
        if (len < 0) {
            len = sizeof(reserve1);
            buffer1 = reserve1;
        } else if ((size_t)len <= sizeof(reserve1)) {
            buffer1 = reserve1;
        } else {
            buffer1 = malloc(len);
        }
        snprintf(buffer1, len, formatString, file, line, test, func, buffer0);
    }
    printf("%s\n", buffer1);
    if (buffer1 != reserve1)
        free(buffer1);
    if (allocatedBuffer0)
        free((void*)buffer0);

    if(level == 0) {
#ifdef WIN32
        if (IsDebuggerPresent()) {
            return false;
        } else {
            assert(0);
        }
#endif
    }
    return true;
}

#endif // ASSERT_ENABLED || defined(ENABLE_LOGGING)
#pragma clang diagnostic pop