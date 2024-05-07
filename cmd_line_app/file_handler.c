#include "file_handler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void read_file(const char *file_name, uint8_t **buffer, uint32_t *size)
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

static char *read_input(int *total_buff_size)
{
  int buff_size = 1;
  char *buffer = malloc(buff_size);
  char c;

  if (buffer == NULL)
  {
    printf("Error reserving memory for input buffer\n");
    return NULL;
  }

  printf("Introduce input data and press ctrl+D to finish\n");

  while ((c = getchar()) != EOF)
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

void read_key(struct crypt_context *context, const char *key_file, const char *key)
{
  if (key_file != NULL)
  {
    printf("Key file: %s\n", key_file);
    read_file(key_file, &(context->key), &(context->key_size));
  }
  else
  {
    printf("No key file. Using key from command line\n");
    context->key = (uint8_t *)key;
    context->key_size = strlen(key);
  }
    printf("Key size: %d\n", context->key_size);
    for(size_t i = 0; i < context->key_size; i++)
    {
      printf("0x%x,", context->key[i]);
    }
    printf("\n");
}

void assign_output_file(FILE **output_file, const char *output_file_name)
{
  if (output_file_name != NULL)
  {
    printf("Output file: %s\n", output_file_name);
    *output_file = fopen(output_file_name, "w");
  }
  else
  {
    printf("No output file. Printing data to std output\n");
    *output_file = stdout;
  }
}

void read_input_data(const char *input_file, char **buffer, int *buffer_size)
{
  if (input_file != NULL)
  {
    printf("Input file: %s\n", input_file);
    read_file(input_file, (uint8_t**)buffer, buffer_size);
  }
  else
  {
    printf("No input file. Reading data from std input\n");
    *buffer = read_input(buffer_size);
  }
}

static void write_ascii_to_file(const char *filename, const uint8_t *data, size_t size)
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