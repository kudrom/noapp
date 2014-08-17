#include "recon_handles.h"

static recognizer_context_t *g_rctx;


static void shutdown(int sig)
{
    stop_recognizing(g_rctx);
    fflush(stdout);

    // Fallback to defaults...
    signal(sig, SIG_DFL);
    kill(getpid(), sig);
}

int init_recognizer_handles(recognizer_context_t *rctx){
    g_rctx = rctx;

    signal(SIGINT, shutdown);
    signal(SIGTERM, shutdown);
    return 0;
}
