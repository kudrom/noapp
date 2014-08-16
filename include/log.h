#ifndef LOG_H
    #define LOG_H
    #include <syslog.h>
    #include <stdio.h>

    #define Log(priority, ...)                                  \
    {                                                           \
        printf(__VA_ARGS__);                                    \
                                                                \
        if (priority != LOG_DEBUG)                              \
            syslog(priority, __VA_ARGS__);                      \
    }
#endif
