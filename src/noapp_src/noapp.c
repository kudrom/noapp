#ifndef NOAPP_H
    #include <noapp.h>
#endif

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
                printf("The named file %s already exists.\n", filename);
                break;
            case ENOSPC:
                printf("The directory or file system cannot be extended for %s.\n", filename);
                break;
            case EROFS:
                printf("The directory for %s is read-only.\n", filename);
                break;
            default:
                printf("Some nasty error happended around mkfifo for %s.\n", filename);
                break;
        }
        retval = -1;
    }

    return retval;
}

static void start_recorder(noapp_config_t *config)
{
    execl(config->recorder, config->recorder, config->reco_fifo);
}

static int load_config(noapp_config_t *config)
{
    config->recorder = "/home/kudrom/src/noapp/build/bin/recorder";
    if (setup_fifo("/tmp/reco_fifo") != 0){
        printf("Failed when creating fifo for recorder-recognizer.\n");
        return -1;
    }
    config->reco_fifo = "/tmp/reco_fifo";

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t recorder_pid, recognizer_pid;
    noapp_config_t config;

    if (load_config(&config) != 0){
        printf("Failed when loading noapp's config.\n");
        return -1;
    }

    recorder_pid = fork();
    if (recorder_pid < 0){
        printf("Failed when forking for recorder.\n");
        return -1;
    }else if(recorder_pid == 0){
        sleep(5);
        if (daemonize() != 0){
            printf("Failed when daemonicing recorder.\n");
            return -1;
        }
        start_recorder(&config);
    }

#ifdef DEBUG
    printf("recorder_pid: %d\n", recorder_pid);
#endif

    return 0;
}
