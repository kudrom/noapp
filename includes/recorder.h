#ifndef RECORDER_H
    #define RECORDER_H
    #include <stdio.h>
    #include <errno.h>
    #include <string.h>
    #include <math.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <time.h>
    #include <stdbool.h>
    #include <pulse/pulseaudio.h>
    #define QUIET_TIME 3
    #define BREAKPOINT 0.9
#endif

/*
 * Main structure of the recorder subsystem.
 */
struct recorder_context{
    char *filename;
    FILE *recording_file;

    int pa_ready;
    bool mute;
    bool started;
    bool is_recording;
    bool dirty_filename;
    time_t timestamp;
    double threshold;

    pa_mainloop *pa_ml;
    pa_context *pa_ctx;
    pa_stream *recording_stream;
    pa_sample_spec pa_ss;
};
typedef struct recorder_context recorder_context_t;

// TODO: Add a parameter to the next stage?
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




int bypass_next_stage();

