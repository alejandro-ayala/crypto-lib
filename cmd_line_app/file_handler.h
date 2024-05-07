#pragma once
#include "crypt_lib/crypt_context.h"
#include <stdint.h>
#include <stdio.h>

void read_input_data(const char *input_file, char **buffer, int *buffer_size);
void read_key(struct crypt_context *context, const char *key_file, const char *key);
void assign_output_file(FILE **output_file, const char *output_file_name);