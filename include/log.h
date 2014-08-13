#ifndef LOG_H
    #define LOG_H
    #include <syslog.h>
#endif

#define log(priority, ...)                              \
{                                                       \
    if (priority == LOG_INFO || priority == LOG_DEBUG)  \
        printf(__VA_ARGS__);                            \
    syslog(priority, __VA_ARGS__);                      \
}
