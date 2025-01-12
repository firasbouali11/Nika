#include "test/unity.h"
#include "test/lexer_spec.h"
#include "core/lexer.h"

#include <stdlib.h>
#include <string.h>

#define KEYWORDS "+-*/()\" =;:{}<>,[]"
#define TEST_ASSERT_EQUAL_TOKEN(a, b) test_assert_equal_token(a, b)

Set *set_from_string(char *s)
{
    Set *set = set_new();
    unsigned int n = strlen(s);
    for (int i = 0; i < n; i++)
        set_add(set, &s[i], sizeof(char));
    return set;
}

static void print_tokens(List *tokens)
{
    for (int i = 0; i < tokens->size; i++)
    {
        Token *token = (Token *)list_get(tokens, i);
        printf("token(%d, %s)\n", token->type, token->value);
    }
}

static void test_assert_equal_token(Token *a, Token *b)
{
    TEST_ASSERT_EQUAL_STRING(a->value, b->value);
    TEST_ASSERT_EQUAL_INT(a->type, b->type);
}

void initilize_lexer()
{
    char *code = "int x = 23;\n int y = 22; \n int z = y+x ; ";
    Set *keywords = set_from_string(KEYWORDS);
    Lexer *lexer = init_lexer(code, keywords);
    TEST_ASSERT_EQUAL_CHAR('i', lexer->c);
    TEST_ASSERT_EQUAL_STRING(code, lexer->content);
    TEST_ASSERT_EQUAL_INT(0, lexer->i);
    free(keywords);
    free(lexer);
}

void extract_tokens_for_variable_assignment()
{
    char *code = "int x = 23;"
                 "fl y = 22.3;"
                 " str s = \"clear\" ; \n";
    Set *keywords = set_from_string(KEYWORDS);
    Lexer *lexer = init_lexer(code, keywords);
    List *tokens = create_tokens(lexer);
    Token *token1 = init_token(34, "int");
    Token *token2 = init_token(8, "x");
    Token *token3 = init_token(4, "=");
    Token *token4 = init_token(2, "23");
    Token *token5 = init_token(10, ";");
    Token *token6 = init_token(35, "fl");
    Token *token7 = init_token(8, "y");
    Token *token8 = init_token(4, "=");
    Token *token9 = init_token(1, "22.3");
    Token *token10 = init_token(10, ";");
    Token *token11 = init_token(36, "str");
    Token *token12 = init_token(8, "s");
    Token *token13 = init_token(4, "=");
    Token *token14 = init_token(0, "clear");
    Token *token15 = init_token(10, ";");
    Token *token16 = init_token(41, " ");
    TEST_ASSERT_EQUAL_TOKEN(token1, list_get(tokens, 0));
    TEST_ASSERT_EQUAL_TOKEN(token2, list_get(tokens, 1));
    TEST_ASSERT_EQUAL_TOKEN(token3, list_get(tokens, 2));
    TEST_ASSERT_EQUAL_TOKEN(token4, list_get(tokens, 3));
    TEST_ASSERT_EQUAL_TOKEN(token5, list_get(tokens, 4));
    TEST_ASSERT_EQUAL_TOKEN(token6, list_get(tokens, 5));
    TEST_ASSERT_EQUAL_TOKEN(token7, list_get(tokens, 6));
    TEST_ASSERT_EQUAL_TOKEN(token8, list_get(tokens, 7));
    TEST_ASSERT_EQUAL_TOKEN(token9, list_get(tokens, 8));
    TEST_ASSERT_EQUAL_TOKEN(token10, list_get(tokens, 9));
    TEST_ASSERT_EQUAL_TOKEN(token11, list_get(tokens, 10));
    TEST_ASSERT_EQUAL_TOKEN(token12, list_get(tokens, 11));
    TEST_ASSERT_EQUAL_TOKEN(token13, list_get(tokens, 12));
    TEST_ASSERT_EQUAL_TOKEN(token14, list_get(tokens, 13));
    TEST_ASSERT_EQUAL_TOKEN(token15, list_get(tokens, 14));
    TEST_ASSERT_EQUAL_TOKEN(token16, list_get(tokens, 15));
    for (int i = 0; i < tokens->size; i++)
    {
        Token *token = (Token *)list_get(tokens, i);
        free(token->value);
        free(token);
    }
    list_free(tokens);
}

void run_lexer_tests(void)
{
    RUN_TEST(initilize_lexer);
    RUN_TEST(extract_tokens_for_variable_assignment);
}