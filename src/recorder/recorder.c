#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include "recorder.h"

void SIGINT_handler(int sig);
// Is there any way to avoid using this global because of the SIGINT handler?
static recorder_context_t *g_rctx;

static void pa_state_cb(pa_context *ctx, void *userdata)
{
    pa_context_state_t state;
    recorder_context_t *rctx = (recorder_context_t *) userdata;
    state = pa_context_get_state(ctx);

    switch (state){
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            if (!rctx->started)
                fprintf(stderr, "State callback of pulseAudio return failure in its context.\n");
            rctx->pa_ready = 2;
            break;
        case PA_CONTEXT_READY:
            rctx->pa_ready = 1;
            break;
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        default:
            break;
    }
}

static void stream_request_cb(pa_stream *s, size_t length, void *userdata)
{
    const void *data;
    size_t nbytes = 0;
    recorder_context_t *rctx = (recorder_context_t *) userdata;
    pa_stream_peek(s, &data, &nbytes);
    fwrite(data, sizeof(uint8_t), nbytes, rctx->recording_file);
    if (data)
        pa_stream_drop(s);
}

static int init_pa(recorder_context_t *rctx)
{
    pa_mainloop_api *pa_mlapi;
    pa_proplist *ctx_properties = pa_proplist_new();
    pa_proplist *stream_properties = pa_proplist_new();
    int retval = 0;

    rctx->pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(rctx->pa_ml);
    rctx->pa_ctx = pa_context_new_with_proplist(pa_mlapi, "NoApp recorder", ctx_properties);
    pa_context_connect(rctx->pa_ctx, NULL, 0, NULL);

    pa_context_set_state_callback(rctx->pa_ctx, pa_state_cb, rctx);
    while (rctx->pa_ready == 0){
        pa_mainloop_iterate(rctx->pa_ml, 1, NULL);
    }
    if (rctx->pa_ready == 2){
        retval = -1;
        goto exit;
    }

    rctx->recording_stream = pa_stream_new_with_proplist(rctx->pa_ctx, "NoApp recorder", &rctx->pa_ss, NULL, stream_properties);
    pa_stream_set_read_callback(rctx->recording_stream, stream_request_cb, rctx);

    retval = pa_stream_connect_record(rctx->recording_stream, NULL, NULL, 0);
    if (retval < 0){
        fprintf(stderr, "pa_stream_connect_playback failed\n");
        goto exit;
    }

exit:
    return retval;
}


static int init_filename(recorder_context_t *rctx)
{
    int retval = 0;

    if ((rctx->recording_file = fopen(rctx->filename, "wb")) < 0){
        fprintf(stderr, "Failed to open the file for recording.\n");
        fprintf(stderr, "\terrno: %s\n", strerror(errno));
        retval = -1;
    }

    return retval;
}

recorder_context_t *init_recorder_context(char *filename)
{
    recorder_context_t *retval;
    retval = malloc(sizeof(recorder_context_t));
    retval->filename = filename;
    retval->pa_ready = 0;
    retval->pa_ss.rate = 44100;
    retval->pa_ss.channels = 2;
    retval->pa_ss.format = PA_SAMPLE_S16LE;
    retval->mute = false;
    retval->started = false;

    return retval;
}

int start_recording(recorder_context_t *rctx)
{
    int retval = 0;

    printf("Starting recorder.\n");
    if ((retval = init_filename(rctx))){
        fprintf(stderr, "Failed in the initialization of the recording file.\n");
        goto exit;
    }

    if ((retval = init_pa(rctx))){
        fprintf(stderr, "Failed in the initializaion of pulse audio\n.");
        goto exit;
    }
    printf("PulseAudio connected.\n");

    g_rctx = rctx;
    signal(SIGINT, SIGINT_handler);

    printf("Starting mainloop.\n");
    rctx->started = true;
    pa_mainloop_run(rctx->pa_ml, &retval);

exit:
    stop_recording(rctx);
    return retval;
}

int stop_recording(recorder_context_t *rctx)
{
    int retval = 0;

    printf("Stopping recorder.\n");
    if (rctx->recording_file)
        fclose(rctx->recording_file);
    pa_context_disconnect(rctx->pa_ctx);
    pa_context_unref(rctx->pa_ctx);
    pa_mainloop_free(rctx->pa_ml);
    free(rctx);
    printf("DONE.\n");
    return retval;
}

void SIGINT_handler(int sig)
{
    stop_recording(g_rctx);
    fflush(stdout);

    // Fallback to defaults...
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT);
}

#ifdef DEBUG
int main(int argc, char*argv[])
{
    char *filename = argc == 1 ? "/tmp/noapp_recording.pcm" : argv[1];
    recorder_context_t *rctx;
    rctx = init_recorder_context(filename);
    return start_recording(rctx);
}
#endif
