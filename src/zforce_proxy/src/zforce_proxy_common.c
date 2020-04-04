//
// Created by Cory Todd on 4/4/2020.
//
#include "zforce_proxy/zforce_proxy_common.h"

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#endif

#if ASSERT_ENABLED
bool assertHandler(const char *test, const char *func, const char *file, size_t line, const char *fmt, ...)
{
    // We have to go with two buffers to get things done
    char reserve0[1024];
    char reserve1[1024];
    const char *buffer0;
    char *buffer1;
    bool allocatedBuffer0 = false;

    if (fmt)
    {
        va_list list;
        va_start(list, fmt);
        int len = vsnprintf(NULL, 0, fmt, list) + 1;
        va_end(list);
        char *buffer0target;
        if (len < 0)
        {
            len = sizeof(reserve0);
            buffer0target = reserve0;
        }
        else if ((size_t)len <= sizeof(reserve0))
        {
            buffer0target = reserve0;
        }
        else
        {
            buffer0target = malloc(len);
            if(buffer0target == NULL){
                // Just die, we can't do anything safe
                assert(false);
            }
            allocatedBuffer0 = true;
        }
        va_start(list, fmt);
        vsnprintf(buffer0target, len, fmt, list);
        va_end(list);
        buffer0 = buffer0target;
    }
    else
    {
        buffer0 = "No reason provided.";
    }

    const char *formatString;
    if (test == NULL)
    {
        formatString = "(%s(%d): Always assert triggered failed -"
                       "Reason: %s"
                       "At:     %s)";
        int len = snprintf(NULL, 0, formatString, file, line, buffer0, func) + 1;
        if (len < 0)
        {
            len = sizeof(reserve1);
            buffer1 = reserve1;
        }
        else if ((size_t)len <= sizeof(reserve1))
        {
            buffer1 = reserve1;
        }
        else
        {
            buffer1 = malloc(len);
        }
        snprintf(buffer1, len, formatString, file, line, buffer0, func);
    }
    else
    {
        formatString = "(%s(%d): Assertion \"%s\" failed -"
                                                 "Reason: %s"
                                                 "At:     %s )";
        int len = snprintf(NULL, 0, formatString, file, line, test, buffer0, func) + 1;
        if (len < 0)
        {
            len = sizeof(reserve1);
            buffer1 = reserve1;
        }
        else if ((size_t)len <= sizeof(reserve1))
        {
            buffer1 = reserve1;
        }
        else
        {
            buffer1 = malloc(len);
        }
        snprintf(buffer1, len, formatString, file, line, test, buffer0, func);
    }
    printf("%s\n", buffer1);
    if (buffer1 != reserve1) free(buffer1);
    if (allocatedBuffer0)    free(buffer0);

#ifdef WIN32
    if (IsDebuggerPresent())
    {
        return false;
    }
    else
    {
        assert(0);
    }
#endif
    return true;
}

#endif // ASSERT_ENABLED