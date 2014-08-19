#define _GNU_SOURCE
#include "utils.h"

/*
 * The returned string must be freed by the caller
 */
char *generate_length_filename(char *base_filename)
{
    size_t size;
    char *length_filename, suffix[8] = ".length";

    size = strlen(base_filename) + 7 + 1;
    length_filename = (char *) malloc(size);
    strncpy(length_filename, base_filename, strlen(base_filename));
    length_filename[strlen(base_filename)] = '\0';
    strncat(length_filename, suffix, strlen(suffix));

    return length_filename;
}

FILE *open_file(char *filename, char *mode)
{
    FILE *fh;
    int err;

    fh = fopen(filename, mode);
    if ((err = ferror(fh)) != 0){
        Log(LOG_ERR, "Failed to open %s with error %d.\n", filename, err);
        return NULL;
    }
    if (feof(fh) != 0){
        Log(LOG_ERR, "Failed to open %s because eof has been reached.\n", filename);
        return NULL;
    }

    return fh;
}

int make_fifo(char *filename, int max_size)
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

int change_fifo_capacity(char *filename, int capacity)
{
    FILE *fh;
    pid_t child;
    child = fork();
    if (child < 0){
        Log(LOG_ERR, "Failed to create a child to change fifo capacity.\n");
        return -1;
    }else if (child == 0){
        fh = fopen(filename, "r");
        fclose(fh);
        exit(0);
    }
    fh = fopen(filename, "w");
    fcntl(fileno(fh), F_SETPIPE_SZ, capacity);
    wait(0);

    return 0;
}
