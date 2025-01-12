#include <stdlib.h>

#include "test/unity.h"
#include "test/map_spec.h"
#include "lib/map.h"
#include "lib/list.h"
#include "utils/type_utils.h"

static void free_map(Map *m)
{
    for (int i = 0; i < m->capacity; i++)
    {
        Pair *pair = m->array[i];
        while (pair)
        {
            Pair *next_pair = pair->next;
            free(pair);
            pair = next_pair;
        }
    }
    free(m->array);
    free(m);
}

void initialize_map()
{
    Map *m = map_new();
    TEST_ASSERT_EQUAL_INT(0, m->size);
    TEST_ASSERT_EQUAL_INT(100, m->capacity);
    for (int i = 0; i < m->capacity; i++)
        TEST_ASSERT_NULL(m->array[i]);
    free_map(m);
}

void insert_elements_in_map()
{
    int default_size_char = 10 * sizeof(char);
    Map *m = map_new();
    map_add(m, "name", "firas", default_size_char);
    map_add(m, "age", &(int){26}, default_size_char);
    map_add(m, "status", "married", default_size_char);
    map_add(m, "income", &(float){12314.544f}, default_size_char);
    map_add(m, &(int){11}, "magic number", sizeof(int));

    TEST_ASSERT_EQUAL_INT(5, m->size);
    TEST_ASSERT_EQUAL_STRING("firas", (char *)map_get(m, "name", default_size_char));
    TEST_ASSERT_EQUAL_STRING("married", (char *)map_get(m, "status", default_size_char));
    TEST_ASSERT_EQUAL_INT(26, *(int *)map_get(m, "age", default_size_char));
    TEST_ASSERT_EQUAL_FLOAT(12314.544f, *(float *)map_get(m, "income", default_size_char));
    TEST_ASSERT_EQUAL_STRING("magic number", (char *)map_get(m, &(int){11}, sizeof(int)));
    free_map(m);
}

void override_elements_in_map()
{
    int default_size_char = 10 * sizeof(char);
    int *age = Int(36);
    Map *m = map_new();
    map_add(m, "name", "firas", default_size_char);
    map_add(m, "age", age, default_size_char);

    TEST_ASSERT_EQUAL_INT(2, m->size);

    void *old_age = map_add(m, "age", &(int){33}, default_size_char);
    if (old_age)
        free(old_age);
    TEST_ASSERT_EQUAL_INT(2, m->size);
    TEST_ASSERT_EQUAL_INT(33, *(int *)map_get(m, "age", default_size_char));
    free_map(m);
}

void delete_elements_from_map()
{
    int default_size_char = 10 * sizeof(char);
    Map *m = map_new();
    map_add(m, "name", "firas", default_size_char);
    map_add(m, "age", &(int){26}, default_size_char);
    map_add(m, "status", "married", default_size_char);
    TEST_ASSERT_EQUAL_INT(3, m->size);
    Pair *deleted_pair = map_delete(m, "name", default_size_char);
    TEST_ASSERT_EQUAL_STRING("name", (char *)deleted_pair->first);
    TEST_ASSERT_EQUAL_STRING("firas", (char *)deleted_pair->second);
    TEST_ASSERT_EQUAL_INT(2, m->size);
    free(deleted_pair);
    free_map(m);
}

void check_resizability_of_map()
{
    List *garbage = list_new();
    Map *m = map_new();
    for (int i = 640; i < 640 + 150; i++)
    {
        int *key = Int(i);
        int *value = Int(i + 1);
        list_add(garbage, key);
        list_add(garbage, value);
        map_add(m, key, value, sizeof(int));
    }
    TEST_ASSERT_EQUAL_INT(701, *(int *)map_get(m, &(int){700}, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(200, m->capacity);
    TEST_ASSERT_EQUAL_INT(150, m->size);
    for (int i = 0; i < garbage->size; i++)
    {
        free(list_get(garbage, i));
    }
    list_free(garbage);
    free_map(m);
}

void run_map_tests(void)
{
    RUN_TEST(initialize_map);
    RUN_TEST(insert_elements_in_map);
    RUN_TEST(override_elements_in_map);
    RUN_TEST(delete_elements_from_map);
    RUN_TEST(check_resizability_of_map);
}