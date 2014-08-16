#ifndef UTILS_H
    #define UTILS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "log.h"

    /*
     * Generate the length filename for a base filename
     */
    char *generate_length_filename(char *base_filename);

    /*
     * Wrapper around syslog and stdio to open files with a common behaviour
     */
    FILE *open_file(char *filename, char *mode);
#endif
