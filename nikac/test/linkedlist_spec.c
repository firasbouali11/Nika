#include <stdlib.h>

#include "test/unity.h"
#include "test/linkedlist_spec.h"
#include "utils/type_utils.h"
#include "lib/linkedlist.h"

void initialize_linkedlist()
{
    LinkedList *ll = linkedlist_new();
    TEST_ASSERT_NULL(ll->head);
    TEST_ASSERT_NULL(ll->tail);
    TEST_ASSERT_EQUAL_INT(0, ll->length);
    linkedlist_free(ll);
}

void add_elements_to_linkedlist_from_front()
{
    LinkedList *ll = linkedlist_new();
    for (int i = 0; i < 5; i++)
    {
        int *ii = Int(i);
        linkedlist_prepend(ll, ii);
    }
    TEST_ASSERT_EQUAL_INT(5, ll->length);
    Node *curr = ll->head;
    int res = 4;
    // make sure the elemets are reverse sorted because we inserted from the front
    while (curr)
    {
        TEST_ASSERT_EQUAL_INT(res--, *(int *)curr->data);
        free(curr->data);
        curr = curr->next;
    }
    linkedlist_free(ll);
}

void add_elements_to_linkedlist_from_back()
{
    LinkedList *ll = linkedlist_new();
    for (int i = 0; i < 5; i++)
    {
        int *ii = Int(i);
        linkedlist_append(ll, ii);
    }
    TEST_ASSERT_EQUAL_INT(5, ll->length);
    Node *curr = ll->head;
    int res = 0;
    while (curr)
    {
        TEST_ASSERT_EQUAL_INT(res++, *(int *)curr->data);
        free(curr->data);
        curr = curr->next;
    }
    linkedlist_free(ll);
}

void reverse_linkedlist()
{
    LinkedList *ll = linkedlist_new();
    for (int i = 0; i < 5; i++)
    {
        int *ii = Int(i);
        linkedlist_append(ll, ii);
    }
    linkedlist_reverse(ll);
    Node *curr = ll->head;
    int res = 4;
    while (curr)
    {
        TEST_ASSERT_EQUAL_INT(res--, *(int *)curr->data);
        free(curr->data);
        curr = curr->next;
    }
    linkedlist_free(ll);
}

void search_for_element_in_linkedlist()
{
    LinkedList *ll = linkedlist_new();
    linkedlist_append(ll, &(int){23});
    linkedlist_append(ll, &(int){54});
    linkedlist_append(ll, &(int){7});
    linkedlist_append(ll, &(int){0});
    linkedlist_append(ll, &(int){11});
    int res = linkedlist_search(ll, &(int){0}, sizeof(int));
    int res1 = linkedlist_search(ll, &(int){33}, sizeof(int));
    TEST_ASSERT_EQUAL_INT(3, res);   // 3rd index (0-based index)
    TEST_ASSERT_EQUAL_INT(-1, res1); // does not exist
    linkedlist_free(ll);
}

void delete_elements_from_linked_list()
{
    LinkedList *ll = linkedlist_new();
    linkedlist_append(ll, &(int){23});
    linkedlist_append(ll, &(int){54});
    linkedlist_append(ll, &(int){7});
    linkedlist_append(ll, &(int){0});
    linkedlist_append(ll, &(int){11});
    TEST_ASSERT_EQUAL_INT(5, ll->length);
    linkedlist_delete(ll, 2);
    TEST_ASSERT_EQUAL_INT(-1, linkedlist_search(ll, &(int){7}, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(4, ll->length);
    linkedlist_free(ll);
}

void run_linkedlist_tests(void)
{
    RUN_TEST(initialize_linkedlist);
    RUN_TEST(add_elements_to_linkedlist_from_front);
    RUN_TEST(add_elements_to_linkedlist_from_back);
    RUN_TEST(reverse_linkedlist);
    RUN_TEST(search_for_element_in_linkedlist);
    RUN_TEST(delete_elements_from_linked_list);
}