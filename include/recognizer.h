#ifndef RECOGNIZER_H
    #define RECOGNIZER_H
    #include <pocketsphinx.h>
    #include "log.h"

    #define CHUNK 512

    /*
     * Main structure to control the recognizer.
     */
    struct recognizer_context{
        ps_decoder_t *ps;

        FILE *in;
        char *in_filename;
        FILE *in_length;

        FILE *out;
        char *out_filename;
    };
    typedef struct recognizer_context recognizer_context_t;

    /*
     * This must be called to call with the result to start_recognizing and all the
     * following functions.
     */
    recognizer_context_t *init_recognizer_context();

    /*
     * Start the recognizing process.
     */
    int start_recognizing(recognizer_context_t *rctx);

    /*
     * Stop the recognizing process.
     */
    int stop_recognizing(recognizer_context_t *rctx);
#endif
