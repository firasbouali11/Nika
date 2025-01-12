#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core/lexer.h"
#include "utils/string_utils.h"

Lexer *init_lexer(char *content, Set *keywords)
{
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->i = 0;
    lexer->c = content[0];
    lexer->content = content;
    lexer->keywords = keywords;
    return lexer;
}

Token *init_token(int type, char *value)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->value = value;
    return token;
}

void advance_lexer(Lexer *lexer)
{
    lexer->i++;
    lexer->c = lexer->content[lexer->i];
}

static Token *extract_string(Lexer *lexer)
{
    // at this point the lexer is in the first " position
    advance_lexer(lexer);
    char *str = malloc(sizeof(char));
    str[0] = '\0';
    int size = 1;
    while (lexer->c != '"')
    {
        str = realloc(str, (size + 1) * sizeof(char));
        str[size - 1] = lexer->c;
        str[size] = '\0';
        size++;
        advance_lexer(lexer);
    }
    Token *token = init_token(TOKEN_STRING, str);
    return token;
}

static Token *extract_identifier(Lexer *lexer)
{
    unsigned int n = strlen(lexer->content);
    char *str = malloc(sizeof(char));
    str[0] = '\0';
    int is_float = 0;
    int only_digit = 1;
    int size = 1;
    while (!set_contains(lexer->keywords, &lexer->c, sizeof(char)) && lexer->c != '\0' && lexer->i < n)
    {
        if (lexer->c == '.')
            is_float = 1;
        if (!isdigit(lexer->c))
            only_digit = 0;
        str = realloc(str, (size + 1) * sizeof(char));
        str[size - 1] = lexer->c;
        str[size] = '\0';
        size++;
        advance_lexer(lexer);
    }
    int token_type;
    if (strcmp(str, "int") == 0)
        token_type = TOKEN_TYPE_INT;
    else if (strcmp(str, "fl") == 0)
        token_type = TOKEN_TYPE_FLOAT;
    else if (strcmp(str, "bl") == 0)
        token_type = TOKEN_TYPE_BOOL;
    else if (strcmp(str, "str") == 0)
        token_type = TOKEN_TYPE_STRING;
    else if (strcmp(str, "list") == 0)
        token_type = TOKEN_TYPE_LIST;
    else if ((strcmp(str, "fn") == 0))
        token_type = TOKEN_TYPE_FUNCTION;
    else if (is_float)
        token_type = TOKEN_FLOAT;
    else if (only_digit)
        token_type = TOKEN_INT;
    else if ((strcmp(str, "FALSE") == 0) || (strcmp(str, "TRUE") == 0))
        token_type = TOKEN_BOOL;
    else if ((strcmp(str, "and") == 0))
        token_type = TOKEN_AND;
    else if ((strcmp(str, "or") == 0))
        token_type = TOKEN_OR;
    else if ((strcmp(str, "if") == 0))
        token_type = TOKEN_IF;
    else if ((strcmp(str, "elf") == 0))
        token_type = TOKEN_ELF;
    else if ((strcmp(str, "else") == 0))
        token_type = TOKEN_ELSE;
    else if ((strcmp(str, "while") == 0))
        token_type = TOKEN_WHILE;
    else if ((strcmp(str, "for") == 0))
        token_type = TOKEN_FOR;
    else if ((strcmp(str, "print") == 0))
        token_type = TOKEN_PRINT_FUNCTION;
    else
        token_type = TOKEN_VAR;
    Token *token = init_token(token_type, str);
    return token;
}

List *create_tokens(Lexer *lexer)
{
    unsigned int n = strlen(lexer->content);
    List *tokens = list_new();
    while (lexer->c != '\0' && lexer->i < n)
    {
        if (isalnum(lexer->c))
        {
            Token *t = extract_identifier(lexer);
            list_add(tokens, t);
            continue;
        }
        switch (lexer->c)
        {
        case ' ':
        case '\n':
            break;
        case '"':
            Token *string_token = extract_string(lexer);
            list_add(tokens, string_token);
            break;
        case ';':
            list_add(tokens, init_token(TOKEN_SEMICOLON, char_to_string(';')));
            break;
        case '=':
            advance_lexer(lexer);
            if (lexer->c == '=')
            {
                list_add(tokens, init_token(TOKEN_EE, create_string("==", 2)));
                break;
            }
            else
            {
                list_add(tokens, init_token(TOKEN_EQUAL, char_to_string('=')));
                continue;
            }
        case '+':
            list_add(tokens, init_token(TOKEN_PLUS, char_to_string('+')));
            break;
        case '-':
            advance_lexer(lexer);
            if (lexer->c == '>')
            {
                list_add(tokens, init_token(TOKEN_TO, create_string("->", 2)));
                break;
            }
            else
            {
                list_add(tokens, init_token(TOKEN_MINUS, char_to_string('-')));
                continue;
            }
        case '*':
            list_add(tokens, init_token(TOKEN_MUL, char_to_string('*')));
            break;
        case '/':
            list_add(tokens, init_token(TOKEN_DIV, char_to_string('/')));
            break;
        case ':':
            list_add(tokens, init_token(TOKEN_COLON, char_to_string(':')));
            break;
        case '(':
            list_add(tokens, init_token(TOKEN_LP, char_to_string('(')));
            break;
        case ')':
            list_add(tokens, init_token(TOKEN_RP, char_to_string(')')));
            break;
        case '[':
            list_add(tokens, init_token(TOKEN_OSB, char_to_string('[')));
            break;
        case ']':
            list_add(tokens, init_token(TOKEN_CSB, char_to_string(']')));
            break;
        case ',':
            list_add(tokens, init_token(TOKEN_COMMA, char_to_string(',')));
            break;
        case '{':
            list_add(tokens, init_token(TOKEN_OB, char_to_string('{')));
            break;
        case '}':
            list_add(tokens, init_token(TOKEN_CB, char_to_string('}')));
            break;
        case '>':
            advance_lexer(lexer);
            if (lexer->c == '=')
            {
                list_add(tokens, init_token(TOKEN_GTE, create_string(">=", 2)));
                break;
            }
            else
            {
                list_add(tokens, init_token(TOKEN_GT, char_to_string('>')));
                continue;
            }
        case '<':
            advance_lexer(lexer);
            if (lexer->c == '=')
            {
                list_add(tokens, init_token(TOKEN_LTE, create_string("<=", 2)));
                break;
            }
            else
            {
                list_add(tokens, init_token(TOKEN_LT, char_to_string('<')));
                continue;
            }
        case '!':
            advance_lexer(lexer);
            if (lexer->c == '=')
            {
                list_add(tokens, init_token(TOKEN_NE, create_string("!=", 2)));
                break;
            }
            else
                continue;
        }
        advance_lexer(lexer);
    }
    list_add(tokens, init_token(TOKEN_EOF, char_to_string(' ')));
    return tokens;
}