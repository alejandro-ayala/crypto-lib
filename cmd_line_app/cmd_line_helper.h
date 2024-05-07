#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef enum 
{
    ARG_NONE,
    ARG_HELP,
    ARG_KEY,
    ARG_KEY_FILE,
    ARG_OUTPUT_FILE,
    ARG_INPUT_FILE,
    ARG_VERSION
} ArgumentType;

typedef struct 
{
    char *key;
    char *key_file;
    char *output_file;
    char *input_file;
    bool request_api_version;
} Arguments;


Arguments parse_arguments(int argc, char *argv[]);
bool validate_key(const char *key);