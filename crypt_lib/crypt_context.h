#pragma once
#include <stdint.h>
#include <pthread.h>

struct crypt_context
{
    uint8_t* key;
    uint32_t key_size;
    unsigned iteration;
    pthread_mutex_t mutex;
};
