#include "crypt_lib/crypt.h"
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

char *read_input(int *total_buff_size)
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
  *total_buff_size = buff_size;
  return buffer;
}

void write_ascii_to_file(const char *filename, const uint8_t *data, size_t size)
{
  FILE *file = fopen(filename, "w");
  if (file == NULL)
  {
    printf("Error opening file: %s\n", filename);
    return;
  }

  for (size_t i = 0; i < size; i++)
  {
    fprintf(file, "%c", data[i]);
  }

  fclose(file);
}

void read_key(struct crypt_context *context, const char *key_file, const char *key)
{
  if (args.key_file != NULL)
  {
    printf("Key file: %s\n", args.key_file);
    read_file(args.key_file, &(context->key), &(context->key_size));
  }
  else
  {
    printf("No key file. Using key from command line\n");
    context->key = (uint8_t *)args.key;
    context->key_size = strlen(args.key);
  }
    printf("Key size: %d\n", context->key_size);
    //printf("Key: %s\n", context->key);
    for(size_t i = 0; i < context->key_size; i++)
    {
      printf("0x%x,", context->key[i]);
    }
}

void assign_output_file(const char *output_file_name)
{
  if (output_file_name != NULL)
  {
    printf("Output file: %s\n", output_file_name);
    output_file = fopen(output_file_name, "w");
  }
  else
  {
    printf("No output file. Printing data to std output\n");
    output_file = stdout;
  }
}

void read_input_data(const char *input_file, char **buffer, int *buffer_size)
{
  if (input_file != NULL)
  {
    printf("Input file: %s\n", input_file);
    read_file(input_file, buffer, buffer_size);
  }
  else
  {
    printf("No input file. Reading data from std input\n");
    *buffer = read_input(buffer_size);
  }

  for (int i = 0; i < *buffer_size; i++)
  {
    printf("0x%x,", (*buffer)[i]);
  }
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
  assign_output_file(args.output_file);
  read_input_data(args.input_file, (char **)&buffer, (int *)&buffer_size);
  
  uint8_t buffer_array[buffer_size];
  memcpy(buffer_array, buffer, buffer_size);
  free(buffer);
  DECODE_AND_PRINT(buffer_array);

  //
}