
#include "acutest.h"
#include "cmd_line_helper.h"
#include "crypt.h"

struct crypt_context context;
uint8_t *buffer;
FILE *test_results_file;

static void *reserve_buffer(const uint32_t size)
{
    return malloc(size);
}

static void clean_up_buffer(void *buff)
{
    free(buff);
}

#define DECODE_AND_PRINT(c)                                          \
    {                                                                \
        buffer = reserve_buffer(sizeof(c));                          \
        crypt_buffer(&context, buffer, c, sizeof(c) / sizeof(c[0])); \
        fwrite(buffer, 1, sizeof(c), test_results_file);             \
        clean_up_buffer(buffer);                                     \
    }

void check_input_key(void)
{
    char *args[] = {"program_name", "-k", "123", "-o", "output.txt"};
    int argc = sizeof(args) / sizeof(args[0]);
    Arguments parsed_args = parse_arguments(argc, args);
    TEST_ASSERT(strcmp(parsed_args.key, args[2]) == 0);
}

void check_key_size_invalid(void)
{
    char input_key[257] = {0};
    char *args[] = {"program_name", "-k", input_key, "-o", "output.txt"};
    int argc = sizeof(args) / sizeof(args[0]);
    Arguments parsed_args = parse_arguments(argc, args);
    TEST_CHECK(strcmp(parsed_args.key, args[2]) != 0);
    TEST_ASSERT(strlen(parsed_args.key) == 0);
}

void automatic_test_coded1(void)
{
    uint8_t key[] = {0xc1, 0xab, 0xe5, 0xec, 0x1e, 0x7a};

    uint8_t coded1[] = {0x85, 0xc9, 0x84, 0x80, 0x46, 0x16, 0xaf, 0xca,

                        0xc9, 0x81, 0x43, 0xe1, 0xac, 0xdd, 0xcb, 0x81,

                        0x45, 0xa9, 0xa3, 0xca, 0xcd, 0x9b, 0x41, 0xfc,

                        0xb3, 0xd5, 0x8c, 0x8f, 0x1c, 0x99

    };
    context.key = &key[0];
    context.key_size = sizeof(key) / sizeof(key[0]);
    uint8_t *expected_decoded_input = "Decoding seems to be correct.\n";

    test_results_file = fopen("test_results_file.txt", "w+");

    DECODE_AND_PRINT(coded1);
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

    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input);
    TEST_CHECK(strcmp(file_content, expected_decoded_input) == 0);
    free(file_content);
}

void automatic_test_coded2(void)
{
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
    context.key = &key[0];
    context.key_size = sizeof(key) / sizeof(key[0]);
    uint8_t *expected_decoded_input = "Status should be kept, so different code might yield same decoded string.\n";

    test_results_file = fopen("test_results_file2.txt", "w+");

    DECODE_AND_PRINT(coded2);
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

    printf("Comparing '%s' to '%s':\n", file_content, expected_decoded_input);
    TEST_CHECK(strcmp(file_content, expected_decoded_input) == 0);
    free(file_content);
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
    {"check_key_size_invalid", check_key_size_invalid},
    {"check_library_version", check_library_version},
    //{"automatic_test_coded1", automatic_test_coded1},
    {"automatic_test_coded2", automatic_test_coded2},
    {NULL, NULL}};
