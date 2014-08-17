#include "recognizer.h"
#include "utils.h"

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
    char *length_filename;

    fh = open_file(rctx->in_filename, "rb");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the in filename of recognizer.\n");
        return -1;
    }
    rctx->in = fh;

    length_filename = generate_length_filename(rctx->in_filename);
    fh = open_file(length_filename, "r");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the file for the recognizer's length.\n");
        return -1;
    }
    rctx->in_length = fh;

    fh = open_file(rctx->out_filename, "w");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the file for the recognizer's length.\n");
        return -1;
    }
    rctx->out = fh;

    return 0;
}

recognizer_context_t  *init_recognizer_context(char *in_filename, char *out_filename){
    recognizer_context_t *rctx;
    rctx = (recognizer_context_t *) malloc(sizeof(recognizer_context_t));
    rctx->in_filename = in_filename;
    rctx->out_filename = out_filename;

    return rctx;
}

int start_recognizing(recognizer_context_t *rctx){
    char const *hyp, *uttid;
    int32 score;
    int retval = 0;
    size_t size, acc, nsamp;

    int16 buf[CHUNK];

    if (init_sphinx(rctx) < 0){
        Log(LOG_ERR, "Failed to init sphinx.\n");
        retval = -1;
    }

    if (init_filenames(rctx) < 0){
        Log(LOG_ERR, "Failed to init filenames.\n");
        retval = -1;
        goto exit;
    }

    if ((retval = ps_start_utt(rctx->ps, "noapp")) < 0){
        Log(LOG_ERR, "Failed to start utterance.\n");
        retval = -1;
        goto exit;
    }

    size = acc = 0;
    fscanf(rctx->in_length, "%zd\n", &size);
    while (!feof(rctx->in) && acc <= size){
        nsamp = fread(buf, 2, 512, rctx->in);
        ps_process_raw(rctx->ps, buf, nsamp, FALSE, FALSE);
        acc += nsamp;
    }

    if ((retval = ps_end_utt(rctx->ps)) < 0){
        Log(LOG_ERR, "Failed to end utterance.\n");
        retval = -1;
        goto exit;
    }

    if ((hyp = ps_get_hyp(rctx->ps, &score, &uttid)) == NULL){
        Log(LOG_INFO, "Cannot get a hypothesis.");
    }
    Log(LOG_INFO, "Hypothesis: %s Score: %d\n", hyp, score);

    fprintf(rctx->out, "%s\n", hyp);

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
    char *in_filename = argc <= 1 ? "/tmp/noapp_recording.raw" : argv[1];
    char *out_filename = argc <= 2 ? "/tmp/utterances" : argv[2];
    recognizer_context_t *rctx;
    rctx = init_recognizer_context(in_filename, out_filename);
    return start_recognizing(rctx);
}
