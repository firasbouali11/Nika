#include <stdlib.h>

#include "test/unity.h"
#include "io/file_reader.h"
#include "test/file_reader_spec.h"

void read_nika_file()
{
    char *code = read_file("test/data/file.nika");
    char *expected_output = "int x = 23;"
                            "\n\n"
                            "str s = \"i need love\"";
    TEST_ASSERT_EQUAL_STRING(expected_output, code);
    free(code);
}

void read_code_from_params()
{
    char *expected_output = "int x = 23;"
                            "\n\n"
                            "str s = \"i need love\"";
    char *code = read_file(expected_output);

    TEST_ASSERT_EQUAL_STRING(expected_output, code);
    free(code);
}

void run_file_reader_tests(void)
{
    RUN_TEST(read_nika_file);
    RUN_TEST(read_code_from_params);
}