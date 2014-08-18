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

static int setup_fifo(char *filename)
{
    int retval = 0;

    if ((retval = mkfifo(filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0){
        switch (errno){
            case EEXIST:
                Log(LOG_ERR, "The named file %s already exists.\n", filename);
                break;
            case ENOSPC:
                Log(LOG_ERR, "The directory or file system cannot be extended for %s.\n", filename);
                break;
            case EROFS:
                Log(LOG_ERR, "The directory for %s is read-only.\n", filename);
                break;
            default:
                Log(LOG_ERR, "Some nasty error happended around mkfifo for %s.\n", filename);
                break;
        }
        retval = -1;
    }

    return retval;
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
    if (setup_fifo("/tmp/reco_fifo") != 0){
        Log(LOG_ERR, "Failed when creating fifo for recorder-recognizer.\n");
        return -1;
    }
    if (setup_fifo("/tmp/reco_fifo.length") != 0){
        Log(LOG_ERR, "Failed when creating fifo's length for recorder-recognizer.\n");
        return -1;
    }
    config->reco_fifo = "/tmp/reco_fifo";

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
