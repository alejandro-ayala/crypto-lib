#include "crypt.h"
#include "common/version.h"
#include <stdio.h>
#include <stdlib.h>

struct crypt_library_version get_version()
{
    struct crypt_library_version version;
    sscanf(CRYP_LIB_VERSION, "%hhu.%hhu.%hhu", &version.major, &version.minor, &version.patch);
    return version;
}

int crypt_buffer(struct crypt_context *context, uint8_t *output, const uint8_t *input, unsigned length)
{

    uint8_t *key = context->key;
    unsigned i = context->iteration;
    pthread_mutex_lock(&context->mutex);

    printf("Input data: ");
    for (int i = 0; i < length; i++)
    {
        printf("0x%x,", input[i]);
    }
    printf("\n");

    for (unsigned index_input = 0; index_input < length; index_input++)
    {
        key[i] = (key[i] + i) % 256;
        output[index_input] = input[index_input] ^ key[i];
        i = (i + 1) % context->key_size;
    }

    printf("Output data: ");
    for (int i = 0; i < length; i++)
    {
        printf("0x%x,", output[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&context->mutex);
    context->iteration = i;
    return 0;
}