#include <stdio.h>
#include "../include/utils.h"

int main(int argc, char *argv[])
{
    int value, fd;
    char *filename, answer;

    if (argc != 3){
        printf("Usage: change_max_size_fifo filename new_value.\n");
        return -1;
    }

    filename = argv[1];
    sscanf(argv[2], "%d", &value);

    printf("filename: %s\n", filename);
    printf("new_value: %d\n", value);
    printf("Proceed? [Y|n]: ");
    scanf("%c", &answer);

    if (answer == 'Y' || answer == 'y' || answer == '\n'){
        change_fifo_capacity(filename, value);
        printf("DONE\n");
    }else{
        printf("Aborted\n");
    }

    return 0;
}
