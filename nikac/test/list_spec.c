
#include <stdlib.h>

#include "test/list_spec.h"
#include "test/unity.h"
#include "lib/list.h"

void initialize_list()
{
    List *l = list_new();
    TEST_ASSERT_EQUAL_INT(0, l->size);
    TEST_ASSERT_EQUAL_INT(100, l->capacity);
    list_free(l);
}

void add_and_get_elements_in_list()
{
    List *l = list_new();
    list_add(l, "firas");
    list_add(l, "saief");
    list_add(l, "youssef");
    list_add(l, "nadine");
    list_add(l, &(int){45});

    TEST_ASSERT_EQUAL_INT(5, l->size);
    TEST_ASSERT_EQUAL_STRING("firas", (char *)list_get(l, 0));
    TEST_ASSERT_EQUAL_STRING("saief", (char *)list_get(l, 1));
    TEST_ASSERT_EQUAL_STRING("youssef", (char *)list_get(l, 2));
    TEST_ASSERT_EQUAL_STRING("nadine", (char *)list_get(l, 3));
    TEST_ASSERT_EQUAL_INT(45, *(int *)list_get(l, 4));
    list_free(l);
}

void insert_element_in_list_by_position()
{
    List *l = list_new();
    list_add(l, "firas");
    list_add(l, "saief");
    list_insert(l, 1, "youssef");
    TEST_ASSERT_EQUAL_INT(3, l->size);
    TEST_ASSERT_EQUAL_STRING("firas", (char *)list_get(l, 0));
    TEST_ASSERT_EQUAL_STRING("saief", (char *)list_get(l, 2));
    TEST_ASSERT_EQUAL_STRING("youssef", (char *)list_get(l, 1));
    list_free(l);
}

void delete_elements_from_list()
{
    List *l = list_new();
    list_add(l, "firas");
    list_add(l, "saief");
    list_add(l, "youssef");
    list_add(l, "nadine");
    TEST_ASSERT_EQUAL_INT(4, l->size);
    list_delete(l, 2);
    TEST_ASSERT_NULL(list_get(l, 3));
    TEST_ASSERT_EQUAL_INT(3, l->size);
    list_free(l);
}

void run_list_tests(void)
{
    RUN_TEST(initialize_list);
    RUN_TEST(add_and_get_elements_in_list);
    RUN_TEST(insert_element_in_list_by_position);
    RUN_TEST(delete_elements_from_list);
}