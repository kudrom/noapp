#ifndef NOAPP_H
    #define NOAPP_H
    #include <stdlib.h>
    #include "log.h"
    #include "recorder.h"
    #include "recognizer.h"
    #include "utils.h"

    struct noapp_config{
        const char *recorder;
        const char *recognizer;
        const char *reco_fifo;
    };
    typedef struct noapp_config noapp_config_t;
#endif
