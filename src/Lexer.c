#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "headers/Lexer.h"

Lexer *initLexer(char *content, Set *keywords)
{
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->i = 0;
    lexer->c = content[0];
    lexer->content = content;
    lexer->keywords = keywords;
    return lexer;
}

void advanceLexer(Lexer *lexer)
{
    lexer->i++;
    lexer->c = lexer->content[lexer->i];
}

Token *getString(Lexer *lexer)
{
    advanceLexer(lexer);
    char *str = malloc(sizeof(char));
    str[0] = '\0';
    int size = 0;
    while (lexer->c != '"')
    {
        str = realloc(str, (size + 1) * sizeof(char));
        str[size] = lexer->c;
        str[size + 1] = '\0';
        size++;
        advanceLexer(lexer);
    }
    Token *token = malloc(sizeof(Token));
    token->type = TOKEN_STRING;
    token->value = str;
    return token;
}

Token *createIdentifier(Lexer *lexer)
{
    unsigned int n = strlen(lexer->content);
    char *str = malloc(sizeof(char));
    str[0] = '\0';
    int point = 0;
    int only_digit = 1;
    while (!containsInSet(lexer->keywords, &(char){lexer->c}, sizeof(char)) && lexer->c != '\0' && lexer->i < n)
    {
        char *s = convertCharToString(lexer->c);
        if (lexer->c == '.')
            point = 1;
        if (!isdigit(lexer->c))
            only_digit = 0;
        str = realloc(str, (strlen(str) + 2) * sizeof(char));
        strncat(str, s, 2 * sizeof(char));
        free(s);
        advanceLexer(lexer);
    }
    Token *token = malloc(sizeof(Token));
    if (strcmp(str, "int") == 0)
        token->type = TOKEN_TYPE_INT;
    else if (strcmp(str, "fl") == 0)
        token->type = TOKEN_TYPE_FLOAT;
    else if (strcmp(str, "bl") == 0)
        token->type = TOKEN_TYPE_BOOL;
    else if (strcmp(str, "str") == 0)
        token->type = TOKEN_TYPE_STRING;
    else if (strcmp(str, "list") == 0)
        token->type = TOKEN_TYPE_LIST;
    else if ((strcmp(str, "fn") == 0))
        token->type = TOKEN_TYPE_FUNCTION;
    else if (point)
        token->type = TOKEN_FLOAT;
    else if (only_digit)
        token->type = TOKEN_INT;
    else if ((strcmp(str, "FALSE") == 0) || (strcmp(str, "TRUE") == 0))
        token->type = TOKEN_BOOL;
    else if ((strcmp(str, "and") == 0))
        token->type = TOKEN_AND;
    else if ((strcmp(str, "or") == 0))
        token->type = TOKEN_OR;
    else if ((strcmp(str, "if") == 0))
        token->type = TOKEN_IF;
    else if ((strcmp(str, "elf") == 0))
        token->type = TOKEN_ELF;
    else if ((strcmp(str, "else") == 0))
        token->type = TOKEN_ELSE;
    else if ((strcmp(str, "while") == 0))
        token->type = TOKEN_WHILE;
    else if ((strcmp(str, "for") == 0))
        token->type = TOKEN_FOR;
    else if ((strcmp(str, "print") == 0))
        token->type = TOKEN_PRINT_FUNCTION;
    else
        token->type = TOKEN_VAR;
    token->value = str;
    return token;
}

Token *initToken(int type, char *value)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->value = value;
    return token;
}

char *convertCharToString(char c)
{
    char *string = malloc(2 * sizeof(char));
    string[0] = c;
    string[1] = '\0';
    return string;
}

char *createString(char *s, int size)
{
    char *string = malloc((size + 1) * sizeof(char));
    strcpy(string, s);
    return string;
}

List *createTokens(Lexer *lexer)
{
    unsigned int n = strlen(lexer->content);
    List *tokens = newList();
    List *tokens_list = newList();
    int lower_level = 0;
    while (lexer->c != '\0' && lexer->i < n)
    {
        if (lexer->c == ' ' || lexer->c == '\n')
        {
            advanceLexer(lexer);
            continue;
        }
        else if (lexer->c == ';')
        {
            if (!lower_level)
            {
                addToList(tokens_list, tokens, sizeof(List));
                tokens = newList();
            }
            else
                addToList(tokens, initToken(TOKEN_SEMICOLON, convertCharToString(';')), sizeof(Token));
        }
        else if (lexer->c == '"')
        {
            Token *t = getString(lexer);
            addToList(tokens, t, sizeof(Token));
        }
        else if (isalnum(lexer->c))
        {
            Token *t = createIdentifier(lexer);
            addToList(tokens, t, sizeof(Token));
            continue;
        }

        switch (lexer->c)
        {
        case '=':
            advanceLexer(lexer);
            if (lexer->c == '=')
            {
                addToList(tokens, initToken(TOKEN_EE, createString("==", 2)), sizeof(Token));
                break;
            }
            else
            {
                addToList(tokens, initToken(TOKEN_EQUAL, convertCharToString('=')), sizeof(Token));
                continue;
            }
        case '+':
            addToList(tokens, initToken(TOKEN_PLUS, convertCharToString('+')), sizeof(Token));
            break;
        case '-':
            advanceLexer(lexer);
            if (lexer->c == '>')
            {
                addToList(tokens, initToken(TOKEN_TO, createString("->", 2)), sizeof(Token));
                break;
            }
            else
            {
                addToList(tokens, initToken(TOKEN_MINUS, convertCharToString('-')), sizeof(Token));
                continue;
            }
        case '*':
            addToList(tokens, initToken(TOKEN_MUL, convertCharToString('*')), sizeof(Token));
            break;
        case '/':
            addToList(tokens, initToken(TOKEN_DIV, convertCharToString('/')), sizeof(Token));
            break;
        case ':':
            addToList(tokens, initToken(TOKEN_COLON, convertCharToString(':')), sizeof(Token));
            break;
        case '(':
            addToList(tokens, initToken(TOKEN_LP, convertCharToString('(')), sizeof(Token));
            break;
        case ')':
            addToList(tokens, initToken(TOKEN_RP, convertCharToString(')')), sizeof(Token));
            break;
        case '[':
            addToList(tokens, initToken(TOKEN_OSB, convertCharToString('[')), sizeof(Token));
            break;
        case ']':
            addToList(tokens, initToken(TOKEN_CSB, convertCharToString(']')), sizeof(Token));
            break;
        case ',':
            addToList(tokens, initToken(TOKEN_COMMA, convertCharToString(',')), sizeof(Token));
            break;
        case '{':
            addToList(tokens, initToken(TOKEN_OB, convertCharToString('{')), sizeof(Token));
            lower_level++;
            break;
        case '}':
            addToList(tokens, initToken(TOKEN_CB, convertCharToString('}')), sizeof(Token));
            lower_level--;
            break;
        case '>':
            advanceLexer(lexer);
            if (lexer->c == '=')
            {
                addToList(tokens, initToken(TOKEN_GTE, createString(">=", 2)), sizeof(Token));
                break;
            }
            else
            {
                addToList(tokens, initToken(TOKEN_GT, convertCharToString('>')), sizeof(Token));
                continue;
            }
        case '<':
            advanceLexer(lexer);
            if (lexer->c == '=')
            {
                addToList(tokens, initToken(TOKEN_LTE, createString("<=", 2)), sizeof(Token));
                break;
            }
            else
            {
                addToList(tokens, initToken(TOKEN_LT, convertCharToString('<')), sizeof(Token));
                continue;
            }
        case '!':
            advanceLexer(lexer);
            if (lexer->c == '=')
                addToList(tokens, initToken(TOKEN_NE, createString("!=", 2)), sizeof(Token));
            break;
        }
        advanceLexer(lexer);
    }
    return tokens_list;
}