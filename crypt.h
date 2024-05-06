#pragma once
#include <stdint.h>
#include <pthread.h>

struct crypt_context
{
    uint8_t* key;
    uint32_t key_size;
    pthread_mutex_t mutex;
};

struct crypt_library_version
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
};

int crypt_buffer(struct crypt_context *context, uint8_t *output, const uint8_t *input, unsigned length);
struct crypt_library_version get_version();