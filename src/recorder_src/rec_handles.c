#include "rec_handles.h"

static recorder_context_t *g_rctx;


static void shutdown(int sig)
{
    stop_recording(g_rctx, false);
    fflush(stdout);

    // Fallback to defaults...
    signal(sig, SIG_DFL);
    kill(getpid(), sig);
}

static void wait_then_restart(int sig)
{
    Log(LOG_INFO, "The other side of the pipe has disconnected.\n");

    stop_recording(g_rctx, true);
    // TODO: right now it's impossible to avoid pa to block or fail badly in a clean restart
    start_recording(g_rctx);
}

int init_recorder_handles(recorder_context_t *rctx){
    g_rctx = rctx;

    signal(SIGINT, shutdown);
    signal(SIGTERM, shutdown);
    signal(SIGPIPE, wait_then_restart);
    return 0;
}
