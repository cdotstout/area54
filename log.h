#include <stdio.h>

#define LOG_ENABLED 1

#define LOG(...)                                                                                  \
    do {                                                                                           \
        if (LOG_ENABLED) {                                                                   \
            printf("%s:%d ", __FILE__, __LINE__);                                                  \
            printf(__VA_ARGS__);                                                                   \
            printf("\n\r");                                                                          \
        }                                                                                          \
    } while (0)

