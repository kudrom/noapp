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
