#include <string.h>
#include <stdlib.h>

#include "test/unity.h"
#include "test/utils_spec.h"
#include "utils/compare_utils.h"
#include "utils/type_utils.h"

typedef struct
{
    int i;
    char *s;
} HelperStruct;

static HelperStruct *create(int i, char *s)
{
    HelperStruct *a = malloc(sizeof(HelperStruct));
    a->i = i;
    a->s = s;
    return a;
}

static int custom_comparison(void *aa, void *bb)
{
    HelperStruct *a = (HelperStruct *)aa;
    HelperStruct *b = (HelperStruct *)bb;
    return (a->i == b->i) && (strcmp(a->s, b->s) == 0);
}

void test_equality_with_simple_types()
{
    HelperStruct *a = create(23, "testing");
    HelperStruct *b = create(23, "testing");
    TEST_ASSERT_EQUAL_INT(TRUE, equal(&(int){32}, &(int){32}, sizeof(int), NULL));
    TEST_ASSERT_EQUAL_INT(FALSE, equal(&(int){123}, &(int){32}, sizeof(int), NULL));
    TEST_ASSERT_EQUAL_INT(TRUE, equal("firas", "firas", 6 * sizeof(char), NULL));
    TEST_ASSERT_EQUAL_INT(TRUE, equal(a, b, sizeof(HelperStruct), NULL));
    b->s = "not testing";
    TEST_ASSERT_EQUAL_INT(FALSE, equal(a, b, sizeof(HelperStruct), NULL));
    free(a);
    free(b);
}

void test_equality_with_custom_function()
{
    char * s1 = String("testing");
    char * s2 = String("testing");
    HelperStruct *a = create(23, s1);
    HelperStruct *b = create(23, s2);
    TEST_ASSERT_EQUAL_INT(FALSE, equal(a, b, sizeof(HelperStruct), NULL)); // false because we can't compare dynamic values with memcmp
    TEST_ASSERT_EQUAL_INT(TRUE, equal(a, b, 0, custom_comparison));
    free(s1);
    free(s2);
    free(a);
    free(b);
}

void run_utils_tests(void)
{
    RUN_TEST(test_equality_with_simple_types);
    RUN_TEST(test_equality_with_custom_function);
}