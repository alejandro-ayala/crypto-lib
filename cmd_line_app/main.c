#include "crypt_lib/crypt.h"
#include "cmd_line_helper.h"
#include "file_handler.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct crypt_context context;
uint8_t *buffer;
uint32_t buffer_size;
Arguments args;
FILE *output_file;

#define DECODE_AND_PRINT(c)                                      \
  {                                                              \
    buffer = reserve_buffer(sizeof(c));                          \
    crypt_buffer(&context, buffer, c, sizeof(c) / sizeof(c[0])); \
    fwrite(buffer, 1, sizeof(c), output_file);                   \
    clean_up_buffer(buffer);                                     \
  }

static void *reserve_buffer(const uint32_t size)
{
  return malloc(size);
}

static void clean_up_buffer(void *buff)
{
  free(buff);
}

int main(int argc, char *argv[])
{

  args = parse_arguments(argc, argv);

  if (args.request_api_version)
  {
    struct crypt_library_version version = get_version();
    printf("Version: %d.%d.%d\n",  version.major, version.minor, version.patch);
    return 0;
  }

  read_key(&context, args.key_file, args.key);
  assign_output_file(&output_file, args.output_file);
  read_input_data(args.input_file, (char **)&buffer, (int *)&buffer_size);
  
  uint8_t buffer_array[buffer_size];
  memcpy(buffer_array, buffer, buffer_size);
  free(buffer);
  DECODE_AND_PRINT(buffer_array);

  //
}