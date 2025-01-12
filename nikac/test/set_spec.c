#include "lib/set.h"
#include "lib/boolean.h"
#include "test/unity.h"
#include "test/set_spec.h"
#include "utils/type_utils.h"

#include <stdlib.h>

void free_set(Set *s)
{
    for (int i = 0; i < s->capacity; i++)
    {
        SetElt *node = s->array[i];
        while (node)
        {
            SetElt *next_node = node->next;
            free(node);
            node = next_node;
        }
    }
    free(s->array);
    free(s);
}
void initialize_set()
{
    Set *set = set_new();
    TEST_ASSERT_EQUAL_INT(100, set->capacity);
    TEST_ASSERT_EQUAL_INT(0, set->size);
    for (int i = 0; i < set->capacity; i++)
        TEST_ASSERT_NULL(set->array[i]);
    free_set(set);
}

void add_elements_to_set()
{
    Set *s = set_new();
    int default_size = 10 * sizeof(char);
    set_add(s, "firas", default_size);
    set_add(s, "nadine", default_size);
    set_add(s, &(int){23}, sizeof(int));

    TEST_ASSERT_EQUAL_INT(3, s->size);
    free_set(s);
}

void check_if_elements_in_set()
{
    Set *s = set_new();
    int default_size = 10 * sizeof(char);
    set_add(s, "firas", default_size);
    set_add(s, "nadine", default_size);
    set_add(s, &(int){23}, sizeof(int));
    TEST_ASSERT_EQUAL_INT(TRUE, set_contains(s, "firas", default_size));
    TEST_ASSERT_EQUAL_INT(FALSE, set_contains(s, "saief", default_size));
    TEST_ASSERT_EQUAL_INT(TRUE, set_contains(s, &(int){23}, sizeof(int)));
    free_set(s);
}

void delete_elements_from_set()
{
    Set *s = set_new();
    int default_size = 10 * sizeof(char);
    set_add(s, "firas", default_size);
    set_add(s, "nadine", default_size);
    set_add(s, "nadine", default_size);

    TEST_ASSERT_EQUAL_INT(2, s->size);
    set_delete(s, "nadine", default_size);
    TEST_ASSERT_EQUAL_INT(1, s->size);
    TEST_ASSERT_EQUAL_INT(FALSE, set_contains(s, "nadine", default_size));
    free_set(s);
}

void check_resizability_of_set()
{
    Set *s = set_new();
    for (int ii = 0; ii < 700; ii++)
    {
        int *j = Int(ii);
        set_add(s, j, sizeof(int));
    }
    TEST_ASSERT_EQUAL_INT(1600, s->capacity);
    TEST_ASSERT_EQUAL_INT(700, s->size);
    TEST_ASSERT_EQUAL_INT(TRUE, set_contains(s, &(int){123}, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(FALSE, set_contains(s, &(int){709}, sizeof(int)));
    for (int i = 0; i < s->capacity; i++)
    {
        SetElt *node = s->array[i];
        while (node)
        {
            free(node->data);
            node = node->next;
        }
    }
    free_set(s);
}
void run_set_tests(void)
{
    RUN_TEST(initialize_set);
    RUN_TEST(add_elements_to_set);
    RUN_TEST(check_if_elements_in_set);
    RUN_TEST(delete_elements_from_set);
    RUN_TEST(check_resizability_of_set);
}