#include "crypt.h"
#include <stdio.h>
#include <stdlib.h>
#include "version.h"

struct crypt_library_version get_version()
{
    struct crypt_library_version version;
    sscanf(CRYP_LIB_VERSION, "%hhu.%hhu.%hhu", &version.major, &version.minor, &version.patch);

    printf("Version: %d.%d.%d\n", version.major, version.minor, version.patch);
    return version;
}

int crypt_buffer(struct crypt_context *context, uint8_t *output, const uint8_t *input, unsigned length)
{
    uint8_t *key = context->key;
    unsigned i = 0;
    
    pthread_mutex_lock(&context->mutex);
    
    for (unsigned index_input = 0; index_input < length; index_input++)
    {
        key[i] = (key[i] + i) % 256;
        output[index_input] = input[index_input] ^ key[i];
        i = (i + 1) % context->key_size;
    }
    
    pthread_mutex_unlock(&context->mutex);
    
    return 0;
}