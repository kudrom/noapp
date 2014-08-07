#ifndef RECORDER_H
    #include "rec_handlers.h"
#endif

static recorder_context_t *g_rctx;


static void shutdown(int sig)
{
    stop_recording(g_rctx);
    fflush(stdout);

    // Fallback to defaults...
    signal(sig, SIG_DFL);
    kill(getpid(), sig);
}

int init_handlers(recorder_context_t *rctx){
    g_rctx = rctx;

    signal(SIGINT, shutdown);
    signal(SIGTERM, shutdown);
    return 0;
}
