#include "recognizer.h"
#include "recon_handles.h"

/*
 * Initialize Sphinx
 */
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

/*
 * Initialize filenames
 */
static int init_filenames(recognizer_context_t *rctx)
{
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
        Log(LOG_ERR, "Failed to open the file for the recording's length.\n");
        return -1;
    }
    rctx->length_file = fh;

    fh = open_file(rctx->out_filename, "w");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the out file of recognizer.\n");
        return -1;
    }
    rctx->out = fh;

    Log(LOG_INFO, "Reading from %s.\n", rctx->in_filename);
    Log(LOG_INFO, "Writing into %s.\n", rctx->out_filename);

    free(length_filename);

    return 0;
}

/*
 * Initialize recognizer's context
 */
recognizer_context_t  *init_recognizer_context(char *in_filename, char *out_filename)
{
    recognizer_context_t *rctx;
    rctx = (recognizer_context_t *) malloc(sizeof(recognizer_context_t));

    rctx->in_filename = in_filename;
    rctx->out_filename = out_filename;

    return rctx;
}

/*
 * Initialize epoll syscall
 */
static int init_epoll(recognizer_context_t *rctx, bool restart)
{
    int fd_in;
    struct epoll_event event;
    FILE *file;

    fd_in = fileno(rctx->in);
    event.data.fd = fd_in;
    event.events = EPOLLIN;

    if (restart){
        if (epoll_ctl(rctx->epfd, EPOLL_CTL_DEL, fd_in, &event) < 0){
            return -1;
        }

        fclose(rctx->in);
        Log(LOG_INFO, "epoll shutdown.\n");

        if ((file = open_file(rctx->in_filename, "rb")) < 0){
            return -1;
        }
        rctx->in = file;

        fd_in = fileno(rctx->in);
        event.data.fd = fd_in;
    }

    if (epoll_ctl(rctx->epfd, EPOLL_CTL_ADD, fd_in, &event) < 0){
        Log(LOG_ERR,
            "Failed to add the in file to the epoll system with error %s.\n",
            strerror(errno));
        return -1;
    }
    Log(LOG_INFO, "epoll up and running.\n");

    return 0;
}

/*
 * Process the nrevents return value of epoll_wait
 */
static int process_nrevents(recognizer_context_t *rctx, int nrevents)
{
    int fd_in;
    static int retries = 0;
    bool check_retries = false;

    fd_in = fileno(rctx->in);
    if (nrevents < 0){
        Log(LOG_ERR,
            "There was an error in the epoll's wait for the out file with error %s.\n",
            strerror(errno));
        check_retries = true;
    }
    if (nrevents != 1){
        Log(LOG_ERR, "More than one event is returned by epoll.\n");
        return -1;
    }
    if (rctx->events[0].events & EPOLLERR){
        Log(LOG_ERR, "An error occurred on the file");
        check_retries = true;
    }
    if (rctx->events[0].events & EPOLLHUP){
        Log(LOG_INFO, "The other side of the FIFO hanged up.\n");
        init_epoll(rctx, true);
        check_retries = true;
    }
    if (!(rctx->events[0].events & EPOLLIN)){
        Log(LOG_ERR,
            "epoll is callbacking with a different event %d.\n",
            rctx->events[0].events);
        return -1;
    }
    if (rctx->events[0].data.fd != fd_in){
        Log(LOG_ERR,
            "epoll is callbacking with a different file descriptor.\n");
        return -1;
    }

    if (check_retries){
        // The caller should retry
        if (retries < MAX_RETRIES){
            retries++;
            Log(LOG_INFO, "Retrying process_nrevents for the %d time.\n", retries);
            return 1;
        // The caller should give up
        }else{
            Log(LOG_INFO, "Rechead limit of retries in process_nrevents.\n");
            retries = 0;
            return -1;
        }
    }

    retries = 0;
    return 0;
}

static void enter_mainloop(recognizer_context_t *rctx)
{
    char const *hyp, *uttid;
    int32 score;
    int nrevents, retval;
    size_t size, acc, nsamp;
    int16 buf[CHUNK];

    while(1){
        nrevents = epoll_wait(rctx->epfd, rctx->events, 1, -1);

        do{
            retval = process_nrevents(rctx, nrevents);
        } while(retval == 1);
        if (retval < 0){
            return;
        }

        if (ps_start_utt(rctx->ps, "noapp") < 0){
            Log(LOG_ERR, "Failed to start utterance.\n");
            return;
        }

        size = acc = 0;
        fscanf(rctx->length_file, "%zd\n", &size);
        while (acc <= size){
            nsamp = fread(buf, 2, 512, rctx->in);
            ps_process_raw(rctx->ps, buf, nsamp, FALSE, FALSE);
            acc += 2*nsamp;
        }

        if (ps_end_utt(rctx->ps) < 0){
            Log(LOG_ERR, "Failed to end utterance.\n");
            return;
        }

        if ((hyp = ps_get_hyp(rctx->ps, &score, &uttid)) == NULL || (strlen(hyp) == 0)){
            Log(LOG_INFO, "Cannot get a hypothesis.\n");
        }else{
            fprintf(rctx->out, "%s\n", hyp);
            fflush(rctx->out);
            Log(LOG_INFO, "Hypothesis: %s Score: %d\n", hyp, score);
        }
    }
}

int start_recognizing(recognizer_context_t *rctx)
{
    int retval = 0, epfd;

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
        Log(LOG_ERR,
            "Failed to initialize epoll with error %s.\n",
            strerror(errno));
        exit(-1);
    }
    rctx->epfd = epfd;

    if (init_epoll(rctx, false) < 0){
        Log(LOG_ERR, "Failed to init with the epoll syscall.\n");
        retval = -1;
        goto exit;
    }

    Log(LOG_INFO, "Entering into the mainloop.\n");
    enter_mainloop(rctx);

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
