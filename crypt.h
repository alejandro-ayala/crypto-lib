#pragma once
#include <stdint.h>

struct crypt_context
{
    uint8_t* key;
    uint8_t key_size;
};

int crypt_buffer(struct crypt_context *context, uint8_t *output, const uint8_t *input, unsigned length);