#include <noapp.h>

/*
 * Taken from  http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
 */
static int daemonize()
{
    umask(0);

    if (setsid() < 0){
        return -1;
    }

    if (chdir("/") < 0){
        return -1;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 0;
}

static int start_recorder(noapp_config_t *config)
{
    execl(config->recorder, "noapp's recorder", config->reco_fifo, NULL);
    return errno;
}

static int start_recognizer(noapp_config_t *config)
{
    execl(config->recognizer, "noapp's recognizer", config->reco_fifo, NULL);
    return errno;
}

static int load_config(noapp_config_t *config)
{
    config->recorder = "/home/kudrom/src/noapp/build/bin/recorder";
    config->recognizer = "/home/kudrom/src/noapp/build/bin/recognizer";
    if (make_fifo("/tmp/reco_fifo", -1) != 0){
        Log(LOG_ERR, "Failed when creating fifo for recorder-recognizer.\n");
        return -1;
    }
    config->reco_fifo = "/tmp/reco_fifo";
    if (make_fifo("/tmp/reco_fifo.length", -1) != 0){
        Log(LOG_ERR, "Failed when creating fifo for recorder-recognizer's length.\n");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t recorder_pid, recognizer_pid;
    noapp_config_t config;
    int retval = 0;

    if (load_config(&config) != 0){
        Log(LOG_ERR, "Failed when loading noapp's config.\n");
        retval = -1;
        goto exit;
    }

    recorder_pid = fork();
    if (recorder_pid < 0){
        Log(LOG_ERR, "Failed when forking for recorder.\n");
        retval = -1;
        goto exit;
    }else if(recorder_pid == 0){
        if (daemonize() != 0){
            Log(LOG_ERR, "Failed when daemonizing recorder.\n");
            retval = -1;
        }else{
            retval = start_recorder(&config);
            Log(LOG_ERR, "execl failed in recorder with errno %s.", strerror(retval));
        }
        goto exit;
    }
    Log(LOG_INFO, "Started recorder.\n");

    recognizer_pid = fork();
    if (recognizer_pid < 0){
        Log(LOG_ERR, "Failed when forking for recognizer.\n");
        retval = -1;
        goto exit;
    }else if(recognizer_pid == 0){
        if (daemonize() != 0){
            Log(LOG_ERR, "Failed when daemonizing recognizer.\n");
            retval = -1;
        }else{
            retval = start_recognizer(&config);
            Log(LOG_ERR, "execl failed in recognizer with errno %s.", strerror(retval));
        }
        goto exit;
    }
    Log(LOG_INFO, "Started recognizer.\n");

#ifdef DEBUG
    Log(LOG_DEBUG, "recorder_pid: %d\n", recorder_pid);
    Log(LOG_DEBUG, "recognizer_pid: %d\n", recognizer_pid);
#endif

exit:
    return retval;
}
