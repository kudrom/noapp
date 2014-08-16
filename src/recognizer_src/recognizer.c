#ifndef RECOGNIZER_H
    #include "recognizer.h"
#endif

static int init_sphinx(recognizer_context_t *rctx){
    cmd_ln_t *config;
    ps_decoder_t *ps;

    if ((config = cmd_ln_init(NULL, ps_args(), TRUE,
                "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
                "-lm", MODELDIR "/lm/turtle/turtle.DMP",
                "-dict", MODELDIR "/lm/turtle/turtle.dic",
                NULL)) == NULL){
        return -1;
    }

    if ((ps = ps_init(config)) == NULL){
        Log(LOG_ERR, "Cannot initialize the ps_decoder.");
        return -1;
    }

    rctx->ps = ps;
    return 0;
}

static int init_filenames(recognizer_context_t *rctx){
    FILE *fh;
    int err;

    fh = fopen(rctx->in_filename, "rb");
    if ((err = ferror(fh)) != 0){
        Log(LOG_ERR, "Failed to open the in file with error %d.\n", err);
        return -1;
    }
    if (feof(fh) != 0){
        Log(LOG_ERR, "Failed to open the in file because the eof has been reached.\n");
        return -1;
    }
    rctx->in = fh;

    return 0;
}

recognizer_context_t  *init_recognizer_context(char *in_filename){
    recognizer_context_t *rctx;
    rctx = (recognizer_context_t *) malloc(sizeof(recognizer_context_t));
    rctx->in_filename = in_filename;

    return rctx;
}

int start_recognizing(recognizer_context_t *rctx){
    int retval = 0;

    char const *hyp, *uttid;
    int32 score;

    if (init_sphinx(rctx) < 0){
        Log(LOG_ERR, "Failed to init sphinx.\n");
        retval = -1;
    }

    if (init_filenames(rctx) < 0){
        Log(LOG_ERR, "Failed to init filenames.\n");
        retval = -1;
        goto exit;
    }

    if ((retval = ps_decode_raw(rctx->ps, rctx->in, "goforward", -1)) < 0){
        Log(LOG_ERR, "Cannot decode the raw.");
        goto exit;
    }

    if ((hyp = ps_get_hyp(rctx->ps, &score, &uttid)) == NULL){
        Log(LOG_INFO, "Cannot get a hypothesis.");
    }
    Log(LOG_INFO, "Hypothesis: %s Score: %d\n", hyp, score);

    fprintf(stdout, "\nRecognized: %s\n", hyp);

exit:
    stop_recognizing(rctx);
    return retval;
}

int stop_recognizing(recognizer_context_t *rctx){
    fclose(rctx->in);
    ps_free(rctx->ps);
    return 0;
}

int main(int argc, char *argv[])
{
    char *filename = argc == 1 ? "/tmp/noapp_recording.raw" : argv[1];
    recognizer_context_t *rctx;
    rctx = init_recognizer_context(filename);
    return start_recognizing(rctx);
}
