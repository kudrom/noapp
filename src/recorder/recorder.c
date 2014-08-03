#include <stdio.h>
#include <pulse/pulseaudio.h>
#include <errno.h>
#include <string.h>

static FILE *g_recording_file = NULL;

void pa_state_cb(pa_context *ctx, void *userdata){
    pa_context_state_t state;
    int *pa_ready = userdata;
    state = pa_context_get_state(ctx);

    switch (state){
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            *pa_ready = 2;
            break;
        case PA_CONTEXT_READY:
            *pa_ready = 1;
            break;
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        default:
            break;
    }
}

static void stream_request_cb(pa_stream *s, size_t length, void *userdata){
    const void *data;
    size_t nbytes = 0;
    pa_mainloop *pa_ml = (pa_mainloop *) userdata;
    pa_stream_peek(s, &data, &nbytes);
    fwrite(data, sizeof(uint8_t), nbytes, g_recording_file);
    pa_stream_drop(s);
}

int init(char *filename){
    pa_mainloop *pa_ml = NULL;
    pa_mainloop_api *pa_mlapi = NULL;
    pa_context *pa_ctx = NULL;
    pa_stream *recording_stream = NULL;
    pa_proplist *ctx_properties = pa_proplist_new();
    pa_proplist *stream_properties = pa_proplist_new();
    pa_sample_spec pa_ss;
    int retval = 0;
    int pa_ready = 0;

    if ((g_recording_file = fopen(filename, "wb")) < 0){
        fprintf(stderr, "Failed to open the file for recording.\n");
        fprintf(stderr, "\terrno: %s\n", strerror(errno));
        retval = -1;
        goto exit;
    }

    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_ctx = pa_context_new_with_proplist(pa_mlapi, "NoApp recorder", ctx_properties);
    pa_context_connect(pa_ctx, NULL, 0, NULL);

    pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ready);
    while (pa_ready == 0){
        pa_mainloop_iterate(pa_ml, 1, NULL);
    }
    if (pa_ready == 2){
        retval = -1;
        goto exit;
    }

    pa_ss.rate = 44100;
    pa_ss.channels = 1;
    pa_ss.format = PA_SAMPLE_S16LE;
    recording_stream = pa_stream_new_with_proplist(pa_ctx, "NoApp recorder", &pa_ss, NULL, stream_properties);
    pa_stream_set_read_callback(recording_stream, stream_request_cb, pa_ml);

    int r;
    r = pa_stream_connect_record(recording_stream, NULL, NULL, 0);
    if (r < 0){
        printf("pa_stream_connect_playback failed\n");
        retval = -1;
        goto exit;
    }

    pa_mainloop_run(pa_ml, &retval);
    printf("retval: %d\n", retval);

exit:
    if (g_recording_file)
        fclose(g_recording_file);
    pa_context_disconnect(pa_ctx);
    pa_context_unref(pa_ctx);
    pa_mainloop_free(pa_ml);
    return retval;
}

#ifdef DEBUG
int main(int argc, char*argv[])
{
    char *filename = argc == 1 ? "/tmp/noapp_recording.pcm" : argv[1];
    return init(filename);
}
#endif
