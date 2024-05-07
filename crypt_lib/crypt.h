#pragma once
#include <stdint.h>
#include "crypt_context.h"
#include "common/version.h"

int crypt_buffer(struct crypt_context *context, uint8_t *output, const uint8_t *input, unsigned length);
struct crypt_library_version get_version();