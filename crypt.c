#include "crypt.h"
#include <stdio.h>


int crypt_buffer(struct crypt_context *context, uint8_t *output, const uint8_t *input, unsigned length)
{
    uint8_t *key = context->key;
    unsigned i = 0;
    //printf("crypt_buffer... size: %d  -- key_size: %d\n", length, context->key_size);
    for (unsigned index_input = 0; index_input < length; index_input++)
    {
        key[i] = (key[i] + i) % 256;
        output[index_input] = input[index_input] ^ key[i];
        //printf("[%d] --> Key: 0x%x -- input: 0x%x --output: 0x%x \n",i, key[i], input[i], output[i]);
        //printf("0x%x,", output[index_input]);
        i = (i + 1) % context->key_size;
    }
    //printf("--------------------\n");
    return 0;
}