#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "headers/Visitor.h"
#include "../lib/headers/Boolean.h"

static Value *initValue(void *data, int type)
{
    Value *val = malloc(sizeof(Value));
    val->data = data;
    val->type = type;
    return val;
}

static double getConextForBinaryOp(Map *cache, Value *l)
{
    double context;
    if (l->type == TOKEN_INT || l->type == TOKEN_BOOL)
        context = *((int *)l->data);
    else if (l->type == TOKEN_FLOAT)
        context = *((double *)l->data);
    else if (l->type == TOKEN_VAR)
    {
        Value *defined_var = (Value *)getFromMap(cache, l->data, (strlen((char *)l->data) + 1) * sizeof(char));
        if (defined_var == NULL)
        {
            fprintf(stderr, "Error: '%s' is undefined\n", (char *)l->data);
            exit(EXIT_FAILURE);
        }
        if (defined_var->type == TOKEN_INT || defined_var->type == TOKEN_BOOL)
            context = *(int *)defined_var->data;
        else if (defined_var->type == TOKEN_FLOAT)
            context = *(double *)defined_var->data;
    }
    return context;
}

Value *execute(ASTNode *node, Map *cache)
{
    if (node == NULL)
        return NULL;

    if (node->token->type == TOKEN_INT)
    {
        int *a = malloc(sizeof(int));
        *a = atoi(node->token->value);
        return initValue(a, TOKEN_INT);
    }
    else if (node->token->type == TOKEN_FLOAT)
    {
        double *a = malloc(sizeof(double));
        *a = strtod(node->token->value, NULL);
        return initValue(a, TOKEN_FLOAT);
    }
    else if (node->token->type == TOKEN_BOOL)
    {
        bool *a = malloc(sizeof(bool));
        if (strcmp(node->token->value, "FALSE") == 0)
            *a = FALSE;
        else
            *a = TRUE;
        return initValue(a, TOKEN_BOOL);
    }
    else if (node->token->type == TOKEN_VAR || node->token->type == TOKEN_STRING)
    {
        return initValue(node->token->value, node->token->type);
    }

    Value *l = NULL, *r = NULL;
    Value *cond = execute(node->condition, cache);
    if (!cond)
    {
        l = execute(node->left, cache);
        r = execute(node->right, cache);
    }
    else
    {
        if (node->token->type == TOKEN_IF || node->token->type == TOKEN_ELF)
        {
            if (*(double *)(cond->data) == 1)
                for (int i = 0; i < node->left->body->size; i++)
                {
                    execute(node->left->body->array[i], cache);
                }
            
            else
                for (int i = 0; i < node->right->body->size; i++)
                {
                    execute(node->right->body->array[i], cache);
                }
        }
        return NULL;
    }

    if (node->token->type == TOKEN_ELSE)
        return execute(node->right, cache);

    if (node->token->type == TOKEN_EQUAL)
    {
        char *var_name = (char *)l->data;
        addToMap(cache, var_name, r, (strlen(var_name) + 1) * sizeof(char));
        return r;
    }

    Value *val = initValue(NULL, TOKEN_FLOAT);
    double *res = malloc(sizeof(double));
    double left_val = getConextForBinaryOp(cache, l);
    double right_val = getConextForBinaryOp(cache, r);

    // run the operation
    if (node->token->type == TOKEN_PLUS)
        *res = left_val + right_val;
    else if (node->token->type == TOKEN_MINUS)
        *res = left_val - right_val;
    else if (node->token->type == TOKEN_DIV)
        *res = left_val / right_val;
    else if (node->token->type == TOKEN_MUL)
        *res = left_val * right_val;
    else if (node->token->type == TOKEN_EE)
        *res = left_val == right_val;
    else if (node->token->type == TOKEN_GT)
        *res = left_val > right_val;
    else if (node->token->type == TOKEN_LT)
        *res = left_val < right_val;
    else if (node->token->type == TOKEN_GTE)
        *res = left_val >= right_val;
    else if (node->token->type == TOKEN_LTE)
        *res = left_val <= right_val;
    else if (node->token->type == TOKEN_AND)
        *res = left_val && right_val;
    else if (node->token->type == TOKEN_OR)
        *res = left_val || right_val;
    val->data = res;
    return val;
}