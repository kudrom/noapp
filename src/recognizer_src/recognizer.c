#include "recognizer.h"
#include "recon_handles.h"

static int init_sphinx(recognizer_context_t *rctx)
{
    cmd_ln_t *config;
    ps_decoder_t *ps;

    if ((config = cmd_ln_init(NULL, ps_args(), TRUE,
                "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
                "-lm", MODELDIR "/lm/turtle/turtle.DMP",
                "-dict", MODELDIR "/lm/turtle/turtle.dic",
                NULL)) == NULL){
        Log(LOG_ERR, "Cannot initiate the cmd call of sphinx.\n");
        return -1;
    }

    if ((ps = ps_init(config)) == NULL){
        Log(LOG_ERR, "Cannot initialize the ps_decoder.\n");
        return -1;
    }

    rctx->ps = ps;
    return 0;
}

static int init_filenames(recognizer_context_t *rctx)
{
    FILE *fh;

    fh = open_file(rctx->in_filename, "rb");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the in filename of recognizer.\n");
        return -1;
    }
    rctx->in = fh;

    fh = open_file(rctx->out_filename, "w");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the out file of recognizer.\n");
        return -1;
    }
    rctx->out = fh;

    Log(LOG_INFO, "Reading from %s.\n", rctx->in_filename);
    Log(LOG_INFO, "Writing into %s.\n", rctx->out_filename);

    return 0;
}

recognizer_context_t  *init_recognizer_context(char *in_filename, char *out_filename)
{
    recognizer_context_t *rctx;
    rctx = (recognizer_context_t *) malloc(sizeof(recognizer_context_t));
    rctx->in_filename = in_filename;
    rctx->out_filename = out_filename;

    return rctx;
}

int start_recognizing(recognizer_context_t *rctx)
{
    char const *hyp, *uttid;
    int32 score;
    int retval = 0, epfd, nrevents, retries = 0, fd_in;
    size_t size;
    int16 buf[CHUNK];
    struct epoll_event event, events[1];

    Log(LOG_INFO, "Starting recognizer.\n");
    init_recognizer_handles(rctx);

    if (init_sphinx(rctx) < 0){
        Log(LOG_ERR, "Failed to init sphinx.\n");
        retval = -1;
        goto exit;
    }
    Log(LOG_INFO, "Sphinx up and running.\n");

    if (init_filenames(rctx) < 0){
        Log(LOG_ERR, "Failed to init filenames.\n");
        retval = -1;
        goto exit;
    }
    Log(LOG_INFO, "Filenames up and running.\n");

    if ((epfd = epoll_create1(0)) < 0){
        Log(LOG_ERR, "Failed to initialize epoll with error %s.\n", strerror(errno));
        retval = -1;
        goto exit;
    }

    fd_in = fileno(rctx->in);
    event.data.fd = fd_in;
    event.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd_in, &event) < 0){
        Log(LOG_ERR, "Failed to add the in file to the epoll system with error %s.\n", strerror(errno));
        retval = -1;
        goto exit;
    }

    Log(LOG_INFO, "Entering into the mainloop.\n");
    while(1){
        nrevents = epoll_wait(epfd, events, 1, -1);
        if (nrevents < 0){
            Log(LOG_ERR, "There was an error in the epoll's wait for the out file with error %s.\n", strerror(errno));

check_retries:
            if (retries < MAX_RETRIES){
                retries++;
                continue;
            }else{
                retval = -1;
                goto exit;
            }
        }
        if (nrevents != 1){
            Log(LOG_ERR, "More than one event is returned by epoll.\n");
            retval = -1;
            goto exit;
        }
        if (events[0].events & EPOLLERR){
            Log(LOG_ERR, "An error occurred on the file");
            goto check_retries;
        }
        if (events[0].events & EPOLLHUP){
            FILE *file;

            Log(LOG_INFO, "The other side of the FIFO hanged up.\n");
            if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd_in, &event) < 0){
                retval = -1;
                goto exit;
            }

            // TODO: Can a deadlock be possible?
            fclose(rctx->in);
            if ((file = open_file(rctx->in_filename, "rb")) < 0){
                retval = -1;
                goto exit;
            }
            rctx->in = file;

            fd_in = fileno(rctx->in);
            event.data.fd = fd_in;
            if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd_in, &event) < 0){
                retval = -1;
                goto exit;
            }
            continue;
        }
        if (!(events[0].events & EPOLLIN)){
            Log(LOG_ERR, "epoll is callbacking with a different event %d.\n", events[0].events);
            retval = -1;
            goto exit;
        }
        if (events[0].data.fd != fd_in){
            Log(LOG_ERR, "epoll is callbacking with a different file descriptor.\n");
            retval = -1;
            goto exit;
        }
        Log(LOG_DEBUG, "Before\n");

        retries = 0;

        if ((retval = ps_start_utt(rctx->ps, "noapp")) < 0){
            Log(LOG_ERR, "Failed to start utterance.\n");
            retval = -1;
            goto exit;
        }
        Log(LOG_DEBUG, "After\n");

        while (!feof(rctx->in)){
            size = fread(buf, 2, 512, rctx->in);
            ps_process_raw(rctx->ps, buf, size, FALSE, FALSE);
        }

        if ((retval = ps_end_utt(rctx->ps)) < 0){
            Log(LOG_ERR, "Failed to end utterance.\n");
            retval = -1;
            goto exit;
        }

        if ((hyp = ps_get_hyp(rctx->ps, &score, &uttid)) == NULL || (strlen(hyp) == 0)){
            Log(LOG_INFO, "Cannot get a hypothesis.\n");
        }else{
            fprintf(rctx->out, "%s\n", hyp);
            Log(LOG_INFO, "Hypothesis: %s Score: %d\n", hyp, score);
        }
    }

exit:
    stop_recognizing(rctx);
    return retval;
}

int stop_recognizing(recognizer_context_t *rctx)
{
    Log(LOG_INFO, "Stopping recognizer.\n");
    if (rctx->in)
        fclose(rctx->in);
    if (rctx->out)
        fclose(rctx->out);
    ps_free(rctx->ps);
    free(rctx);
    Log(LOG_INFO, "DONE");
    return 0;
}

int main(int argc, char *argv[])
{
    char *in_filename = argc <= 1 ? "/tmp/noapp.raw" : argv[1];
    char *out_filename = argc <= 2 ? "/tmp/utterances" : argv[2];
    recognizer_context_t *rctx;
    rctx = init_recognizer_context(in_filename, out_filename);
    return start_recognizing(rctx);
}
