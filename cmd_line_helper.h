#pragma once
#include <stdio.h>


typedef enum 
{
    ARG_NONE,
    ARG_HELP,
    ARG_KEY,
    ARG_KEY_FILE,
    ARG_OUTPUT_FILE,
    ARG_INPUT_FILE
} ArgumentType;

typedef struct 
{
    char *key;
    char *key_file;
    char *output_file;
    char *input_file;
} Arguments;


Arguments parse_arguments(int argc, char *argv[]);