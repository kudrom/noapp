#ifndef RECOGNIZER_H
    #define RECOGNIZER_H
    #include <pocketsphinx.h>
    #include "log.h"

    struct recognizer_context{
        ps_decoder_t *ps;

        FILE *in;
        char *in_filename;
        FILE *in_length;

        FILE *out;
    };
    typedef struct recognizer_context recognizer_context_t;

    recognizer_context_t *init_recognizer_context();

    int start_recognizing(recognizer_context_t *rctx);

    int stop_recognizing(recognizer_context_t *rctx);
#endif
