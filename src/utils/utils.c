#include "utils.h"

/*
 * The returned string must be freed by the caller
 */
char *generate_length_filename(char *base_filename)
{
    size_t size;
    char *length_filename;

    size = strlen(base_filename) + 7;
    length_filename = (char *) malloc(size);
    strncpy(length_filename, base_filename, strlen(base_filename));
    strncat(length_filename, ".length", 7+1);

    return length_filename;
}

int open_file(FILE *fh)
{
    int retval = 0;

    return retval;
}
