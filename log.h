#pragma once

#include <stdarg.h>
#include <stdio.h>

constexpr bool kLogEnabled = true;
constexpr bool kDretEnabled = true;

#define LOG(...)                                                                                   \
    do {                                                                                           \
        if (kLogEnabled) {                                                                         \
            printf("%s:%d ", __FILE__, __LINE__);                                                  \
            printf(__VA_ARGS__);                                                                   \
            printf("\n\r");                                                                        \
        }                                                                                          \
    } while (0)

__attribute__((format(printf, 3, 4))) static inline bool dret_false(const char* file, int line,
                                                                    const char* msg, ...)
{
    printf("%s:%d returning false: ", file, line);
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    printf("\n");
    return false;
}

// Must provide const char* msg as the 2nd paramter; other parameters optional.
#define DRETF(ret, ...)                                                                            \
    (kDretEnabled ? (ret == true ? true : dret_false(__FILE__, __LINE__, __VA_ARGS__)) : ret)

__attribute__((format(printf, 3, 4))) static inline void dret_null(const char* file, int line,
                                                                   const char* msg, ...)
{
    printf("%s:%d returning null: ", file, line);
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    printf("\n");
}
// Must provide const char* msg as the 2nd paramter; other parameters optional.
#define DRETP(ret, ...)                                                                            \
    (kDretEnabled ? (ret != nullptr ? ret : (dret_null(__FILE__, __LINE__, __VA_ARGS__), nullptr)) \
                  : ret)