#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "headers/stdc.h"
#include "headers/Parser.h"
#include "headers/Visitor.h"

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

Value *execute(ASTNode *node, Map *cache, Map *functions)
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
    else if (node->token->type == TOKEN_OSB)
    {
        List *l = newList();
        for (int i = 0; i < node->body->size; i++)
        {
            ASTNode *elt = (ASTNode *)getFromList(node->body, i);
            Value *val = execute(elt, cache, functions);
            addToList(l, val, sizeof(Value));
        }
        return initValue(l, TOKEN_OSB);
    }
    else if (node->token->type == TOKEN_FUNCTION)
    {
        ASTNode *function_ast = (ASTNode *)getFromMap(functions, node->token->value, (strlen(node->token->value) + 1) * sizeof(char));
        for (int i = 0; i < node->args->size; i++)
        {
            ASTNode *arg = (ASTNode *)getFromList(node->args, i);
            Value *val = execute(arg, cache, functions);
            char *var_name = ((ASTNode *)getFromList(function_ast->args, i))->token->value;
            addToMap(cache, var_name, val, (strlen(var_name) + 1) * sizeof(char));
        }
        Value *val = NULL;
        for (int i = 0; i < function_ast->body->size; i++)
        {
            ASTNode *node = (ASTNode *)getFromList(function_ast->body, i);
            val = execute(node, cache, functions);
        }
        if (val->type == TOKEN_VAR)
            return (Value *)getFromMap(cache, val->data, (strlen(val->data) + 1) * sizeof(char));

        return val;
    }

    Value *l = NULL, *r = NULL;
    Value *cond = execute(node->condition, cache, functions);
    if (!cond)
    {
        l = execute(node->left, cache, functions);
        r = execute(node->right, cache, functions);
    }
    else
    {
        if (node->token->type == TOKEN_IF || node->token->type == TOKEN_ELF)
        {
            if (*(double *)(cond->data) == 1)
                for (int i = 0; i < node->left->body->size; i++)
                    execute(node->left->body->array[i], cache, functions);
            else
                for (int i = 0; i < node->right->body->size; i++)
                    execute(node->right->body->array[i], cache, functions);
        }
        else if (node->token->type == TOKEN_FOR)
        {
            if (cond->type == TOKEN_INT)
            {
                int *data = (int *)cond->data;
                int start = data[0];
                int end = data[1];
                for (int i = start; i < end; i++)
                {
                    double *_i = malloc(sizeof(double));
                    *_i = (double)i;
                    char *var_name = node->condition->left->token->value;
                    int key_size = (strlen(var_name) + 1) * sizeof(char);
                    Value *val = initValue(_i, TOKEN_FLOAT);
                    addToMap(cache, var_name, val, key_size);
                    for (int j = 0; j < node->left->body->size; j++)
                        execute(getFromList(node->left->body, j), cache, functions);
                    deleteFromMap(cache, var_name, key_size);
                    free(_i);
                    free(val);
                }
            }
            else if (cond->type == TOKEN_VAR)
            {
                char *list_var = (char *)cond->data;
                char *var_name = node->condition->left->token->value;
                Value *val = (Value *)getFromMap(cache, list_var, (strlen(list_var) + 1) * sizeof(char));
                List *l = val->data;
                for (int i = 0; i < l->size; i++)
                {
                    Value *a = (Value *)getFromList(l, i);
                    int key_size = (strlen(var_name) + 1) * sizeof(char);
                    addToMap(cache, var_name, a, key_size);
                    for (int j = 0; j < node->left->body->size; j++)
                        execute(getFromList(node->left->body, j), cache, functions);
                    deleteFromMap(cache, var_name, key_size);
                }
            }
        }
        return NULL;
    }

    if (node->token->type == TOKEN_PRINT_FUNCTION)
    {
        char *var_name = (char *)l->data;
        Value *var = getFromMap(cache, var_name, (strlen(var_name) + 1) * sizeof(char));
        switch (var->type)
        {
        case TOKEN_FLOAT:
            printf("%f\n", *(double *)var->data);
            break;
        case TOKEN_BOOL:
        {
            double value = *(double *)var->data;
            char *resp = (value == 1) ? "TRUE" : "FALSE";
            puts(resp);
        }
        break;
        case TOKEN_INT:
            printf("%d\n", *(int *)var->data);
            break;
        case TOKEN_STRING:
            puts((char *)var->data);
            break;
        default:
            printf("<%p>\n", var->data);
            break;
        }
        return NULL;
    }

    if (node->token->type == TOKEN_ELSE)
        return execute(node->right, cache, functions);

    if (node->token->type == TOKEN_EQUAL)
    {
        char *var_name = (char *)l->data;
        addToMap(cache, var_name, r, (strlen(var_name) + 1) * sizeof(char));
        return r;
    }
    if (node->token->type == TOKEN_COLON)
    {
        return r;
    }

    // this part contains the binary operators;
    double left_val = getConextForBinaryOp(cache, l);
    double right_val = getConextForBinaryOp(cache, r);

    if (node->token->type == TOKEN_TO)
    {
        int start = left_val;
        int end = right_val;
        int *l = malloc(2 * sizeof(int));
        l[0] = start;
        l[1] = end;
        return initValue(l, TOKEN_INT);
    }

    Value *val = initValue(NULL, TOKEN_FLOAT);
    double *res = malloc(sizeof(double));

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