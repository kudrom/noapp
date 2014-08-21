#ifndef RECOGNIZER_H
    #define RECOGNIZER_H
    #include <errno.h>
    #include <sys/epoll.h>
    #include <pocketsphinx.h>
    #include "log.h"
    #include "utils.h"

    #define CHUNK 512
    #define MAX_RETRIES 10

    /*
     * Main structure to control the recognizer.
     */
    struct recognizer_context{
        ps_decoder_t *ps;

        FILE *in;
        char *in_filename;
        FILE *length_file;

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
