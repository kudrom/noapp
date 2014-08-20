#ifndef UTILS_H
    #define UTILS_H
    #include <errno.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    #include "log.h"

    /*
     * Generate the length filename for a base filename
     */
    char *generate_length_filename(char *base_filename);

    /*
     * Wrapper around syslog and stdio to open files with a common behaviour
     */
    FILE *open_file(char *filename, char *mode);

    /*
     * Wrapper around mkfifo
     */
    int make_fifo(char *filename, int max_size);

    /*
     * Getter for the capacity of a UNIX fifo
     */
    int get_fifo_capacity(char *filename);

    /*
     * Setter for the capacity of a UNIX fifo
     */
    int set_fifo_capacity(char *filename, int capacity);
#endif
