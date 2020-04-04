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
bool assertHandler(const char *test, const char *func, const char *file, size_t line, const char *fmt, ...);
#define ASSERT_ENABLED 1
#define ASSERT(x, ...) (!!(x) || assertHandler(#x, __func__, __FILE__,__LINE__ , ## __VA_ARGS__ ) || (DEBUG_BREAK, true))
#define ALWAYS_ASSERT(...) (assertHandler(nullptr, __func__, __FILE__,__LINE__ , ## __VA_ARGS__ ) || (DEBUG_BREAK, true))
#endif