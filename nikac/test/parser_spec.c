#include <stdlib.h>
#include <string.h>

#include "test/unity.h"
#include "test/parser_spec.h"
#include "core/lexer.h"
#include "core/parser.h"

#define KEYWORDS "+-*/()\" =;:{}<>,[]"

static Set *set_from_string(char *s)
{
    Set *set = set_new();
    unsigned int n = strlen(s);
    for (int i = 0; i < n; i++)
        set_add(set, &s[i], sizeof(char));
    return set;
}

static ASTNode *build_ast(char *code)
{

    Set *keywords = set_from_string(KEYWORDS);
    Lexer *lexer = init_lexer(code, keywords);
    List *tokens = create_tokens(lexer);
    Parser *parser = parser_init(tokens);
    TEST_ASSERT_NOT_NULL(parser->tokens);
    ASTNode *ast = parse(parser);
    return ast;
}

void initalize_parser()
{
    Parser *parser = parser_init(NULL);
    TEST_ASSERT_NULL(parser->current_token);
    TEST_ASSERT_NULL(parser->tokens);
    TEST_ASSERT_EQUAL_INT(-1, parser->i);
    free(parser);
}

void create_mathematical_expression_ast()
{
    char *code = "int x = 12;"
                 "int y = (x+1) / 3;";
    ASTNode* ast = build_ast(code);
}

void create_comparison_expression_ast()
{
    char *code = "bool a = 23 > 11 or 22 <=23;";
    ASTNode* ast = build_ast(code);
}

void run_parser_tests(void)
{
    RUN_TEST(initalize_parser);
    RUN_TEST(create_mathematical_expression_ast);
    RUN_TEST(create_comparison_expression_ast);
}