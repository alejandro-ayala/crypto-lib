#include "cmd_line_helper.h"
#include "string_literals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MIN_KEY_LENGTH 1
#define MAX_KEY_LENGTH 256

static void print_help()
{
    fprintf(stderr, "Usage: crypt [-h] -k <key> | -f <key_file> [-o <output_file>] [<input_file>]\n");
    fprintf(stderr, "Valid arguments:\n");
    fprintf(stderr, "  -%s, --%s\n", HELP_SHORT_OPTION, HELP_LONG_OPTION);
    fprintf(stderr, "  -%s, --%s <key> \n", KEY_SHORT_OPTION, KEY_LONG_OPTION);
    fprintf(stderr, "  -%s, --%s <key_file> \n", KEY_FILE_SHORT_OPTION, KEY_FILE_LONG_OPTION);
    fprintf(stderr, "  -%s, --%s <output_file>\n", OUTPUT_FILE_SHORT_OPTION, OUTPUT_FILE_LONG_OPTION);
    fprintf(stderr, "  <input_file>\n");
    exit(EXIT_SUCCESS);
}

static void print_error(char argv)
{
    fprintf(stderr, "Error: Invalid value after argument: %c \n", argv);
    exit(1);
}

static bool validate_key(const char *key)
{
    size_t key_length = strlen(key);
    printf("Key length: %zu\n", key_length);
    if (key_length < MIN_KEY_LENGTH || key_length > MAX_KEY_LENGTH)
    {
        printf("invalid key length\n");
        return false;
    }
    {
        return true;
    }
}

Arguments parse_arguments(int argc, char *argv[])
{
    int option;
    Arguments args = {NULL, NULL, NULL, NULL};

    while ((option = getopt(argc, argv, "hk:f:o:")) != -1)
    {
        switch (option)
        {
        case 'h':
            print_help();
            break;
        case 'k':
            printf("Key: %s\n", optarg);
            const bool is_key_valid = validate_key(optarg);
            if(!is_key_valid)
            {
                fprintf(stderr, "Error: Invalid key length\n");
                exit(EXIT_FAILURE);
            }
            args.key = optarg;
            break;
        case 'f':
            printf("key file: %s\n", optarg);
            args.key_file = malloc(strlen(optarg) + 1);
            if (args.key_file == NULL)
            {
                fprintf(stderr, "Error: reserving memory for key_file argument\n");
                exit(EXIT_FAILURE);
            }
            strcpy(args.key_file, optarg);

            break;
        case 'o':
            if (strcmp(optarg, "-") != 0)
            {
                args.output_file = optarg;
            }
            break;
        default:
            print_help();
            break;
        }
    }

    if ((args.key == NULL && args.key_file == NULL) || (args.key != NULL && args.key_file != NULL))
    {
        fprintf(stderr, "Error: You must provide either a key (-k) or a key file (-f).\n");
        print_help();
    }

    if (optind < argc)
    {
        args.input_file = argv[optind];
    }

    return args;
}