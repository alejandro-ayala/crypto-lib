
#include "acutest/include/acutest.h"
#include "cmd_line_app/cmd_line_helper.h"
#include "crypt_lib/crypt.h"
#include <stdbool.h>
#include <stdint.h>

struct crypt_context context;
uint8_t *buffer;
FILE *test_results_file;

#define DECODE_AND_PRINT(c)                                          \
    {                                                                \
        buffer = reserve_buffer(sizeof(c));                          \
        crypt_buffer(&context, buffer, c, sizeof(c) / sizeof(c[0])); \
        fwrite(buffer, 1, sizeof(c), test_results_file);             \
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

static uint8_t *read_decoded_input()
{
    uint8_t *file_content;
    long file_size;
    if (test_results_file != NULL)
    {
        fseek(test_results_file, 0, SEEK_END);
        file_size = ftell(test_results_file);
        fseek(test_results_file, 0, SEEK_SET);

        file_content = malloc(file_size);
        fread(file_content, 1, file_size, test_results_file);

        fclose(test_results_file);
    }
    return file_content;
}

void check_input_key(void)
{
    char *args[] = {"program_name", "-k", "123", "-o", "output.txt"};
    int argc = sizeof(args) / sizeof(args[0]);
    Arguments parsed_args = parse_arguments(argc, args);
    TEST_ASSERT(strcmp(parsed_args.key, args[2]) == 0);
}

void check_key_size_lower_min(void)
{
    char input_key[260] = {'\0'};
    bool is_valid_key = validate_key(input_key);
    TEST_ASSERT(is_valid_key == false);
}

void check_key_size_higher_max(void)
{
    char input_key[260];
    memset(input_key, 'a', sizeof(input_key) - 1);
    input_key[259] = '\0';
    bool is_valid_key = validate_key(input_key);
    TEST_ASSERT(is_valid_key == false);
}

void check_key_size_valid(void)
{
    char input_key[260] = {1, 2, 3};
    bool is_valid_key = validate_key(input_key);
    TEST_ASSERT(is_valid_key == true);
}

void parse_arguments_invalid_key(void)
{
    char input_key[257] = {0};
    char *args[] = {"program_name", "-k", input_key, "-o", "output.txt"};
    int argc = sizeof(args) / sizeof(args[0]);
    Arguments parsed_args = parse_arguments(argc, args);
    TEST_CHECK(strcmp(parsed_args.key, args[2]) != 0);
    TEST_ASSERT(strlen(parsed_args.key) == 0);
}

void automatic_test_sample(void)
{
    uint8_t key[] = {0xc1, 0xab, 0xe5, 0xec, 0x1e, 0x7a};
    printf("Keysss: %s\n", key);
    printf("sdasd\n");
    uint8_t coded1[] = {0x85, 0xc9, 0x84, 0x80, 0x46, 0x16, 0xaf, 0xca,

                        0xc9, 0x81, 0x43, 0xe1, 0xac, 0xdd, 0xcb, 0x81,

                        0x45, 0xa9, 0xa3, 0xca, 0xcd, 0x9b, 0x41, 0xfc,

                        0xb3, 0xd5, 0x8c, 0x8f, 0x1c, 0x99

    };
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
    context.key = &key[0];
    context.key_size = sizeof(key) / sizeof(key[0]);
    uint8_t *expected_decoded_input1 = "Decoding seems to be correct.\n";
    uint8_t *expected_decoded_input2_3 = "Status should be kept, so different code might yield same decoded string.\n";
    uint8_t *expected_decoded_input4 = "A";
    uint8_t *expected_decoded_input5 = "\nMust work for single characters as well.\n";

    test_results_file = fopen("test_results_file.txt", "w+");
    DECODE_AND_PRINT(coded1);
    uint8_t *file_content = read_decoded_input();
    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input1);
    TEST_CHECK(strcmp(file_content, expected_decoded_input1) == 0);
    free(file_content);

    test_results_file = fopen("test_results_file.txt", "w+");
    DECODE_AND_PRINT(coded2);
    file_content = read_decoded_input();
    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input2_3);
    TEST_CHECK(strcmp(file_content, expected_decoded_input2_3) == 0);
    free(file_content);

    test_results_file = fopen("test_results_file.txt", "w+");
    DECODE_AND_PRINT(coded3);
    file_content = read_decoded_input();
    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input2_3);
    TEST_CHECK(strcmp(file_content, expected_decoded_input2_3) == 0);
    free(file_content);

    test_results_file = fopen("test_results_file.txt", "w+");
    DECODE_AND_PRINT(coded4);
    file_content = read_decoded_input();
    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input4);
    TEST_CHECK(strcmp(file_content, expected_decoded_input4) == 0);
    free(file_content);

    test_results_file = fopen("test_results_file.txt", "w+");
    DECODE_AND_PRINT(coded5);
    file_content = read_decoded_input();
    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input5);
    TEST_CHECK(strcmp(file_content, expected_decoded_input5) == 0);
    free(file_content);
}

void automatic_test_encrypt_and_decrypt(void)
{
    uint8_t original_key[] = {0xc1, 0xab, 0xe5, 0xec, 0x1e, 0x7a};
    const uint8_t original_data[] = "aslkjdslkadsalkdjwsadlñakdlñkasñol eqrkop342rkeflñfñ,.\n";
    const size_t data_lenght = sizeof(original_data) / sizeof(original_data[0]);
    uint8_t encrypted_data[data_lenght];
    const size_t key_lenght = sizeof(original_key) / sizeof(original_key[0]);

    uint8_t key[key_lenght];
    memcpy(key, original_key, key_lenght);
    context.key = malloc(key_lenght);
    memcpy(context.key, original_key, key_lenght);
    context.key_size = key_lenght;
    context.iteration = 0;

    test_results_file = fopen("test_results_file.txt", "w+");
    DECODE_AND_PRINT(original_data);
    uint8_t *file_content = read_decoded_input();
    memcpy(encrypted_data, file_content, data_lenght);
    free(file_content);

    test_results_file = fopen("test_results_file.txt", "w+");
    // context.key = &original_key[0];
    memcpy(context.key, original_key, key_lenght);
    context.key_size = key_lenght;
    context.iteration = 0;
    DECODE_AND_PRINT(encrypted_data);
    file_content = read_decoded_input();
    printf("Comparing '%s' to '%s':\n", file_content, original_data);
    TEST_CHECK(strcmp(file_content, original_data) == 0);
    free(file_content);
    free(context.key);
}

void check_library_version(void)
{
    struct crypt_library_version version = get_version();
    struct crypt_library_version expected_version = {1, 0, 0};
    TEST_ASSERT(version.major == expected_version.major);
    TEST_ASSERT(version.minor == expected_version.minor);
    TEST_ASSERT(version.patch == expected_version.patch);
}

TEST_LIST = {
    {"check_input_key", check_input_key},
    {"check_key_size_valid", check_key_size_valid},
    {"check_key_size_higher_max", check_key_size_higher_max},
    {"check_key_size_lower_min", check_key_size_lower_min},
    {"check_library_version", check_library_version},
    {"automatic_test_sample", automatic_test_sample},
    {"automatic_test_encrypt_and_decrypt", automatic_test_encrypt_and_decrypt},
    {NULL, NULL}};
