#include "test/unity.h"
#include "test/map_spec.h"
#include "test/list_spec.h"
#include "test/set_spec.h"
#include "test/utils_spec.h"
#include "test/linkedlist_spec.h"
#include "test/file_reader_spec.h"
#include "test/lexer_spec.h"
#include "test/parser_spec.h"

void setUp(void) {}
void tearDown(void) {}

int main(void){
    
    UNITY_BEGIN();
    run_list_tests();
    run_map_tests();
    run_set_tests();
    run_utils_tests();
    run_linkedlist_tests();
    run_file_reader_tests();
    run_lexer_tests();
    run_parser_tests();
    return UNITY_END();
}