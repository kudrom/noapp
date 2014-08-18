#include "recorder.h"
#include "rec_handles.h"

#ifdef DEBUG
static FILE *threshold_file;
#endif

static int init_filename(recorder_context_t *);

static void pa_state_cb(pa_context *ctx, void *userdata)
{
    pa_context_state_t state;
    recorder_context_t *rctx = (recorder_context_t *) userdata;
    state = pa_context_get_state(ctx);

    switch (state){
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            if (rctx->pa_ready == 1)
                Log(LOG_ERR, "State callback of pulseAudio return failure in its context.\n");
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

static double sqsum(const uint8_t *data, size_t size)
{
    size_t i;
    double sum = 0.0, value;
    int16_t value_signed;

    for (i = 0; i < size; i++){
        value_signed = (int16_t) ((data[2*i+1] << 8) | data[2*i]);
        value = (double) value_signed;
        sum += value * value;
    }

    return sum;
}

static double calculate_rms_power(const uint8_t *data, size_t size)
{
    double sum = sqsum(data, size);
    return sqrt(sum / size);
}

/*
 * The callback will record when mute isn't activated and:
 *    - it'll always record the first SILENCE_BREAKPOINTS events that are above the low_point but below the 
 *    high_point (these are normally the trailing of a utterance).
 *    - it'll always record if the event is well below the high_level (these are normally the utterances)
 * The counter_activity (which is one of the responsible to record after all, see the above comment) is reseted
 * mainly by a detected high_point utterance or when we have detected a long streak of idle.
 *
 * The callback will split an utterance when nothing interesting has been said in the last HOT_ZONE seconds.
 */
static void stream_request_cb(pa_stream *stream, size_t length, void *userdata)
{
    const void *data;
    size_t size = 0;
    double power, low_point, high_point;
    int retval, retries;
    time_t current_time;
    recorder_context_t *rctx = (recorder_context_t *) userdata;

    if (rctx->dirty_filename){
        fclose(rctx->recording_file);

        retries = 0;
        do{
            retval = init_filename(rctx);
            retries++;
        } while(retval != 0 && retries < 20);
        if (retries == 20){
            Log(LOG_ERR, "There was some nasty problems with the opening of %s file.\n", rctx->filename);
            stop_recording(rctx);
        }

        rctx->dirty_filename = false;
    }

    if (!rctx->mute){
        pa_stream_peek(stream, &data, &size);
        rctx->is_recording = false;

        power = calculate_rms_power(data, size);
        low_point = rctx->threshold * LOW_BREAKPOINT;
        high_point = rctx->threshold * HIGH_BREAKPOINT;
        rctx->total_activity++;
        if (data){
            if (power >= low_point){
                if (rctx->counter_silence < SILENCE_BREAKPOINT || power > high_point){

                    rctx->counter_idle = 0;

                    current_time = time(NULL);
                    if (difftime(current_time, rctx->timestamp) >= HOT_ZONE){
                        if ((rctx->high_activity / rctx->total_activity <= INTERESTING_RATIO) && (rctx->data_length > 0)){
                            fprintf(rctx->length_file, "%u\n", rctx->data_length);
                            rctx->data_length = 0;
                            Log(LOG_DEBUG, "Utterance cut. High: %d total: %d\n", 
                                            rctx->high_activity, rctx->total_activity);
                            //TODO: remove this
                            Log(LOG_INFO, "Utterance cut.\n");
                            rctx->high_activity = rctx->total_activity = 0;
                        }
                        rctx->timestamp = current_time;
                    }

                    if (power <= high_point){
                        rctx->counter_silence++;
                    }else{
                        rctx->counter_silence = 0;
                        rctx->high_activity++;
                    }

                    rctx->is_recording = true;
                    fwrite(data, sizeof(uint8_t), size, rctx->recording_file);
                    rctx->data_length += (unsigned int) length;

                    Log(LOG_DEBUG, "-> power: %f[%f, %f] threshold: %f silence: %d idle: %d\n",
                            power, low_point, high_point,rctx->threshold, rctx->counter_silence, rctx->counter_idle);
                }else{
                    Log(LOG_DEBUG, "SS power: %f[%f, %f] threshold: %f silence: %d idle: %d\n",
                            power, low_point, high_point, rctx->threshold, rctx->counter_silence, rctx->counter_idle);
                }
            }else{
                rctx->counter_idle = fmin(++rctx->counter_idle, IDLE_BREAKPOINT);
                if (rctx->counter_idle == IDLE_BREAKPOINT)
                    rctx->counter_silence = 0;
                Log(LOG_DEBUG, "   power: %f[%f, %f] threshold: %f silence: %d idle: %d\n",
                        power, low_point, high_point, rctx->threshold, rctx->counter_silence, rctx->counter_idle);
            }
            pa_stream_drop(stream);
        }
    }
}

static void detect_threshold_cb(pa_stream *stream, size_t length, void *userdata)
{
    const void *data;
    size_t size = 0;
    static size_t acc_size = 0;
    double sum;
    static double acc_sqsum = 0;
    recorder_context_t *rctx = (recorder_context_t *) userdata;

    pa_stream_peek(stream, &data, &size);
#ifdef DEBUG
    fwrite(data, sizeof(uint8_t), size, threshold_file);
#endif
    if (data)
        pa_stream_drop(stream);

    /* We are sampling with 16 bits but size is measured in bytes */
    size >>= 1;
    sum = sqsum((const uint8_t *) data, size);
    acc_sqsum += sum;
    acc_size += size;
    rctx->threshold = sqrt(acc_sqsum / acc_size);
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

    retval = pa_stream_connect_record(rctx->recording_stream, NULL, NULL, 0);
    if (retval < 0){
        Log(LOG_ERR, "pa_stream_connect_playback failed\n");
        goto exit;
    }

exit:
    return retval;
}

static int init_filename(recorder_context_t *rctx)
{
    int retval = 0;
    char *length_filename;
    FILE *fh;

    fh = open_file(rctx->filename, "wb");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the file for recording.\n");
        return -1;
    }
    rctx->recording_file = fh;

    length_filename = generate_length_filename(rctx->filename);
    fh = open_file(length_filename, "w");
    if (fh == NULL){
        Log(LOG_ERR, "Failed to open the file for the recording's length.\n");
        return -1;
    }
    rctx->length_file = fh;

    Log(LOG_INFO, "Writing into %s and %s.\n", rctx->filename, length_filename);

    free(length_filename);

    return retval;
}

recorder_context_t *init_recorder_context(char *filename)
{
    recorder_context_t *retval;
    retval = malloc(sizeof(recorder_context_t));
    retval->filename = filename;
    retval->dirty_filename = false;
    retval->pa_ready = 0;
    retval->pa_ss.rate = 16000;
    retval->pa_ss.channels = 1;
    retval->pa_ss.format = PA_SAMPLE_S16LE;
    retval->mute = false;
    retval->is_recording = false;
    retval->data_length = 0;
    retval->counter_silence = 0;
    retval->counter_idle = 0;
    retval->high_activity = 0;
    retval->total_activity = 0;

    return retval;
}

int start_recording(recorder_context_t *rctx)
{
    int retval = 0;
    time_t current_time;

    Log(LOG_INFO, "Starting recorder.\n");
    if ((retval = init_filename(rctx))){
        Log(LOG_ERR, "Failed in the initialization of the recording file.\n");
        goto exit;
    }

    if ((retval = init_pa(rctx))){
        Log(LOG_ERR, "Failed in the initializaion of pulse audio.\n");
        goto exit;
    }
    Log(LOG_INFO, "PulseAudio connected.\n");
    init_recorder_handles(rctx);

    Log(LOG_INFO, "Calibrating threshold.\n");
    printf("*****  ATTENTION  *****\n");
    printf("Keep quiet for the next %d seconds please.\n", QUIET_TIME);
    pa_stream_set_read_callback(rctx->recording_stream, detect_threshold_cb, rctx);
    rctx->timestamp = time(NULL);
    current_time = rctx->timestamp;
#ifdef DEBUG
    threshold_file = fopen("/tmp/threshold.pcm", "wb");
#endif
    while (difftime(current_time, rctx->timestamp) < QUIET_TIME){
        pa_mainloop_iterate(rctx->pa_ml, 0, &retval);
        current_time = time(NULL);
        if (retval < 0){
            Log(LOG_ERR, "There was a problem calculating the threshold power.\n");
            goto exit;
        }
    }
    Log(LOG_DEBUG, "Threshold: %f\n", rctx->threshold);

    Log(LOG_INFO, "Entering into the mainloop.\n");
    printf("\nNow you can start talking.\n");
    rctx->timestamp = time(NULL);
    pa_stream_set_read_callback(rctx->recording_stream, stream_request_cb, rctx);
    pa_mainloop_run(rctx->pa_ml, &retval);

exit:
    stop_recording(rctx);
    return retval;
}

int stop_recording(recorder_context_t *rctx)
{
    int retval = 0;

    Log(LOG_INFO, "Stopping recorder.\n");
#ifdef DEBUG
    fclose(threshold_file);
#endif
    if (rctx->recording_file)
        fclose(rctx->recording_file);
    if (rctx->length_file)
        fclose(rctx->length_file);
    pa_mainloop_quit(rctx->pa_ml, retval);
    pa_context_disconnect(rctx->pa_ctx);
    pa_context_unref(rctx->pa_ctx);
    pa_mainloop_free(rctx->pa_ml);
    free(rctx);
    Log(LOG_INFO, "DONE.\n");
    return retval;
}

//TODO: this may be broken because of the length_file
int change_recording_file(recorder_context_t *rctx, char *new_filename)
{
    FILE *file;

    if ((file = fopen(new_filename, "wb")) == NULL){
        Log(LOG_ERR, "The filename change is invalid.\n");
        return -1;
    }else{
        rctx->filename = new_filename;
        rctx->dirty_filename = true;
        fclose(file);
        return 0;
    }
}

int mute_recorder(recorder_context_t *rctx)
{
    rctx->mute = true;
    return 0;
}

int unmute_recorder(recorder_context_t *rctx)
{
    rctx->mute = false;
    return 0;
}

int toggle_recorder(recorder_context_t *rctx)
{
    rctx->mute = !rctx->mute;
    return 0;
}

int main(int argc, char*argv[])
{
    char *filename = argc == 1 ? "/tmp/noapp.raw" : argv[1];
    recorder_context_t *rctx;
    rctx = init_recorder_context(filename);
    return start_recording(rctx);
}
