#include <stdio.h>
#include "../include/utils.h"

static void print_help()
{
    printf("Usage: change_max_size_fifo <command> filename [new_capacity]\n"
            "    Where:\n"
            "      command can be set or get\n"
            "      set needs the new_capacity argument.\n");
}

int main(int argc, char *argv[])
{
    int value, fd;
    char *filename, answer, *command;

    if (argc < 3){
        print_help();
        return -1;
    }

    command = argv[1];
    filename = argv[2];

    if (strcmp(command, "set") == 0){
        if (argc != 4){
            print_help();
            return -1;
        }

        sscanf(argv[3], "%d", &value);

        printf("YOU SHOULDN'T USE THIS COMMAND IF THE FILENAME IS ALREADY OPEN\n");
        printf("filename: %s\n", filename);
        printf("new_value: %d\n", value);
        printf("Proceed? [Y|n]: ");
        scanf("%c", &answer);

        if (answer == 'Y' || answer == 'y' || answer == '\n'){
            set_fifo_capacity(filename, value);
            printf("DONE\n");
        }else{
            printf("Aborted\n");
        }
    }else if (strcmp(command, "get") == 0){
        value = get_fifo_capacity(filename);
        printf("Capacity of %s is %d\n", filename, value);
    }else{
        print_help();
        return -1;
    }

    return 0;
}
