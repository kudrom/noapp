#ifndef NOAPP_H
    #define NOAPP_H
    #include <fcntl.h>
    #include <stdlib.h>
    #include <sys/stat.h>
    #include "log.h"
    #include "recorder.h"
    #include "recognizer.h"
#endif

struct noapp_config{
    char *recorder;
    char *recognizer;
    char *reco_fifo;
};
typedef struct noapp_config noapp_config_t;
