#include "cmd_line_helper.h"
#include "common/string_literals.h"
#include "crypt_lib/crypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN_KEY_LENGTH 1
#define MAX_KEY_LENGTH 256

static void print_help()
{
    fprintf(stderr, "Usage: crypt [-h] [-v] -k <key> | -f <key_file> [-o <output_file>] [<input_file>]\n");
    fprintf(stderr, "Valid arguments:\n");
    fprintf(stderr, "  -%c, show the help\n", HELP_SHORT_OPTION);
    fprintf(stderr, "  -%c <key> \n", KEY_SHORT_OPTION);
    fprintf(stderr, "  -%c <key_file> \n", KEY_FILE_SHORT_OPTION);
    fprintf(stderr, "  -%c <output_file>\n", OUTPUT_FILE_SHORT_OPTION);
    fprintf(stderr, "  -%c, to get the current version of the API\n", VERSION_API_OPTION);
    fprintf(stderr, "  <input_file>\n");
    exit(EXIT_SUCCESS);
}

static void print_version()
{
    struct crypt_library_version version = get_version();
    fprintf(stdout, "API version %d.%d.%d\n", version.major, version.minor, version.patch);
    exit(EXIT_SUCCESS);
}

bool validate_key(const char *key)
{
    size_t key_length = strlen(key);
    bool is_valid = true;
    if (key_length < MIN_KEY_LENGTH || key_length > MAX_KEY_LENGTH)
    {
        printf("invalid key length\n");
        is_valid = false;
    }
    return is_valid;
}

Arguments parse_arguments(int argc, char *argv[])
{
    int option;
    Arguments args = {NULL, NULL, NULL, NULL, false};

    while ((option = getopt(argc, argv, "hvk:f:o:")) != -1)
    {
        switch (option)
        {
        case HELP_SHORT_OPTION:
            print_help();
            break;
        case KEY_SHORT_OPTION:
            printf("Key: %s\n", optarg);
            const bool is_key_valid = validate_key(optarg);
            if(!is_key_valid)
            {
                fprintf(stderr, "Error: Invalid key length.\n");
                exit(EXIT_FAILURE);
            }
            args.key = optarg;
            break;
        case KEY_FILE_SHORT_OPTION:
            //printf("key file: %s\n", optarg);
            args.key_file = malloc(strlen(optarg) + 1);
            if (args.key_file == NULL)
            {
                fprintf(stderr, "Error: reserving memory for key_file argument\n");
                exit(EXIT_FAILURE);
            }
            strcpy(args.key_file, optarg);
            break;
        case OUTPUT_FILE_SHORT_OPTION:
            if (strcmp(optarg, "-") != 0)
            {
                args.output_file = optarg;
            }
            break;
        case VERSION_API_OPTION:
            print_version();
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