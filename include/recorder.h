#ifndef RECORDER_H
    #define RECORDER_H
    #include <errno.h>
    #include <math.h>
    #include <pulse/pulseaudio.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <sys/types.h>
    #include <time.h>
    #include <unistd.h>
    #include "log.h"
    #include "utils.h"

    #define QUIET_TIME 3

    #define HIGH_BREAKPOINT 1.2
    #define LOW_BREAKPOINT 0.8
    #define SILENCE_BREAKPOINT 3
    #define IDLE_BREAKPOINT 6
    #define HOT_ZONE 6
    #define INTERESTING_RATIO 0.3
    #define NEW_FIFO_CAPACITY 100000

    /*
     * Main structure of the recorder subsystem.
     */
    struct recorder_context{
        char *filename;
        bool dirty_filename;
        FILE *recording_file;

        FILE *length_file;
        unsigned int data_length;

        int counter_silence;
        int counter_idle;

        int high_activity;
        int total_activity;

        int pa_ready;
        bool mute;
        bool is_recording;

        time_t timestamp;
        double threshold;

        pa_mainloop *pa_ml;
        pa_context *pa_ctx;
        pa_stream *recording_stream;
        pa_sample_spec pa_ss;
    };
    typedef struct recorder_context recorder_context_t;

    /*
     * To use the NoApp recorder first you must initialize a correct recording_context
     * and then you can start recording with a call to start_recording(rctx);
     */
    recorder_context_t *init_recorder_context(char *filename);

    /*
     * Start recording the utterances that the user spells to the system.
     */
    int start_recording(recorder_context_t *rctx);

    /*
     *  Stop recording the utterances that the user spells to the system.
     *  It must be called to free memory, it desconnects to pulseAudio.
     */
    int stop_recording(recorder_context_t *rctx);

    /*
     * Changes the target of the recording to the new_filename
     */
    int change_recording_file(recorder_context_t *rctx, char *new_filename);

    /*
     * Stop recording the utterances that the user spells to the system.
     * It doesn't free memory, it is safe to call it multiple times.
     */
    int mute_recorder(recorder_context_t *rctx);

    /*
     * Activates the recorder, it is safe to call it multiple times.
     */
    int unmute_recorder(recorder_context_t *rctx);

    /*
     * Mutes/Unmutes the recorder.
     */
    int toggle_recorder(recorder_context_t *rctx);
#endif
