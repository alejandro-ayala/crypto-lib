#include "crypt.h"
#include "cmd_line_helper.h"

#include <stdio.h>
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
    crypt_buffer(&context, buffer, c, sizeof(c) / sizeof(c[0])); \
    fwrite(buffer, 1, sizeof(c), output_file);              \
  }

static void debug_key()
{
  printf("Key values:\n");
  for (int i = 0; i < context.key_size; i++)
  {
    printf("%02x ", context.key[i]);
  }
  printf("\n");
}

static void *reserve_buffer(const uint32_t size)
{
  return malloc(size);
}

static void clean_up_buffer(void *buff)
{
  free(buff);
}

void read_file(const char *file_name, uint8_t **buffer, uint32_t *size)
{
  FILE *file = fopen(file_name, "rb");
  if (file == NULL)
  {
    fprintf(stderr, "Error: Could not open file: %s\n", file_name);
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *buffer = malloc(*size);
  fread(*buffer, 1, *size, file);
  fclose(file);
}

char *read_input()
{
  int buff_size = 1;
  char *buffer = malloc(buff_size);
  char c;

  if (buffer == NULL)
  {
    printf("Error reserving memory for input buffer\n");
    return NULL;
  }

  printf("Introduce input data and press ENTER to finish:\n");

  while ((c = getchar()) != '\n')
  {
    buffer[buff_size - 1] = c;
    buff_size++;
    buffer = realloc(buffer, buff_size);

    if (buffer == NULL)
    {
      printf("Error rellocating memory for buffer\n");
      return NULL;
    }
  }

  buffer[buff_size - 1] = '\0';
  return buffer;
}

int main(int argc, char *argv[])
{
  args = parse_arguments(argc, argv);
  if (args.key_file != NULL)
  {
    printf("Key file: %s\n", args.key_file);
    read_file(args.key_file, &context.key, &context.key_size);
    printf("Key size: %d\n", context.key_size);
    printf("Key: %s\n", context.key);
  }

  if (args.input_file != NULL)
  {
    printf("Input file: %s\n", args.input_file);
    read_file(args.input_file, &buffer, &buffer_size);
    printf("Buffer size: %d\n", buffer_size);
    printf("Buffer: %s\n", buffer);
  }
  else
  {
    printf("No input file. Reading data from std input\n");
    buffer = read_input();
    printf("Entered data: %s\n", buffer);
  }

  if(args.output_file != NULL)
  {
    printf("Output file: %s\n", args.output_file);
    output_file = fopen(args.output_file, "w");
  }
  else
  {
    printf("No output file. Printing data to std output\n");
    output_file = stdout;
  }

  uint8_t key[] = {0xc1, 0xab, 0xe5, 0xec, 0x1e, 0x7a};
  uint8_t coded2[] = {0x92, 0xc5, 0x90, 0x8a, 0x43, 0xeb, 0xe1, 0xc1,

                      0x9b, 0x6e, 0x4f, 0xf1, 0xa5, 0x93, 0x97, 0x61,

                      0x1e, 0xc9, 0xa4, 0xc4, 0x83, 0x2b, 0x62, 0xd4,

                      0xae, 0x95, 0x9d, 0x63, 0x20, 0xca, 0xa4, 0xc4,

                      0x9e, 0x63, 0x3e, 0x91, 0xa2, 0xd8, 0x99, 0x75,

                      0x6e, 0xdb, 0xa8, 0xdf, 0x97, 0x67, 0x72, 0xc2,

                      0xa8, 0xdc, 0x6d, 0x72, 0x76, 0xb3, 0xa0, 0xd7,

                      0x66, 0x39, 0x3e, 0xa0, 0xa2, 0xd4, 0x61, 0x79,

                      0x3a, 0xea, 0xb2, 0xc8, 0x75, 0x76, 0x0c, 0xa8,

                      0xef, 0xb7

  };


  uint8_t coded3[] = {0x5a, 0x56, 0x07, 0xa0, 0xb4, 0xcd, 0x2b, 0x56,

                      0x02, 0xb6, 0xb4, 0xd3, 0x69, 0x08, 0x0c, 0xbb,

                      0xe1, 0xab, 0x6a, 0x5b, 0x06, 0xcf, 0xe1, 0xb2,

                      0x7e, 0x0e, 0x12, 0x81, 0xa7, 0xa4, 0x76, 0x43,

                      0x1f, 0x83, 0xb5, 0xe3, 0x76, 0x5b, 0x1a, 0x97,

                      0xe1, 0xa9, 0x7e, 0x50, 0xea, 0x83, 0xe1, 0xbc,

                      0x70, 0x5f, 0xea, 0x98, 0xe1, 0xb5, 0x7a, 0x50,

                      0xef, 0x21, 0xa5, 0xa2, 0x7e, 0x2f, 0xea, 0x63,

                      0xa5, 0xe8, 0x6c, 0x37, 0xe0, 0x62, 0xaf, 0xae,

                      0x0f, 0x4c

  };

  uint8_t coded4[] = {0xd7};

  uint8_t coded5[] = {0x1a, 0x8c, 0xbf, 0x50, 0x3d, 0xba, 0x62, 0xae,

                      0xb9, 0x4e, 0x6c, 0xf8, 0x75, 0xb3, 0xec, 0x54,

                      0x26, 0xcc, 0x78, 0xad, 0xa8, 0x09, 0x31, 0xce,

                      0x45, 0xb3, 0xaf, 0x48, 0x21, 0xcf, 0x5b, 0xb2,

                      0xef, 0x4c, 0x2b, 0x8e, 0x59, 0xa4, 0xbc, 0x43,

                      0x75, 0xb8

  };

  //<main code, variable declaration, library initialization,...>
  context.key_size = sizeof(key) / sizeof(key[0]);
  context.key = &key[0];
  //memcpy(context.key, key, context.key_size);

  printf("Decoding and printing coded2...\n");
  debug_key();
  buffer = reserve_buffer(sizeof(coded2));
  DECODE_AND_PRINT(coded2);
  clean_up_buffer(buffer);

  // printf("Decoding and printing coded3...\n");
  // debug_key();
  // buffer = reserve_buffer(sizeof(coded3));
  // DECODE_AND_PRINT(coded3);
  // clean_up_buffer(buffer);

  // printf("Decoding and printing coded4...\n");
  // buffer = reserve_buffer(sizeof(coded4));
  // debug_key();
  // DECODE_AND_PRINT(coded4);
  // clean_up_buffer(buffer);

  // printf("Decoding and printing coded5...\n");
  // buffer = reserve_buffer(sizeof(coded4));
  // debug_key();
  // DECODE_AND_PRINT(coded5);
  // clean_up_buffer(buffer);
  //<main code, library termination, cleanup, ...>

  fclose(output_file);
}