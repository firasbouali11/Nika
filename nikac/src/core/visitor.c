#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stdc.h"
#include "core/parser.h"
#include "core/visitor.h"

static Value *initValue(void *data, int type)
{
    Value *val = malloc(sizeof(Value));
    val->data = data;
    val->type = type;
    return val;
}

static double getConextForBinaryOp(Map *cache, Value *val)
{
    double context;
    if (val->type == TOKEN_INT || val->type == TOKEN_BOOL)
        context = *((int *)val->data);
    else if (val->type == TOKEN_FLOAT)
        context = *((double *)val->data);
    else if (val->type == TOKEN_VAR)
    {
        Value *defined_var = (Value *)map_get(cache, val->data, (strlen((char *)val->data) + 1) * sizeof(char));
        if (defined_var == NULL)
        {
            fprintf(stderr, "Error: '%s' is undefined\n", (char *)val->data);
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
        int *int_value = malloc(sizeof(int));
        *int_value = atoi(node->token->value);
        return initValue(int_value, TOKEN_INT);
    }
    else if (node->token->type == TOKEN_FLOAT)
    {
        double *float_value = malloc(sizeof(double));
        *float_value = strtod(node->token->value, NULL);
        return initValue(float_value, TOKEN_FLOAT);
    }
    else if (node->token->type == TOKEN_BOOL)
    {
        bool *boolean_value = malloc(sizeof(bool));
        if (strcmp(node->token->value, "FALSE") == 0)
            *boolean_value = FALSE;
        else
            *boolean_value = TRUE;
        return initValue(boolean_value, TOKEN_BOOL);
    }
    else if (node->token->type == TOKEN_VAR || node->token->type == TOKEN_STRING)
    {
        return initValue(node->token->value, node->token->type);
    }
    else if (node->token->type == TOKEN_OSB)
    {
        List *l = list_new();
        for (int i = 0; i < node->body->size; i++)
        {
            ASTNode *elt = (ASTNode *)list_get(node->body, i);
            Value *val = execute(elt, cache, functions);
            list_add(l, val);
        }
        return initValue(l, TOKEN_OSB);
    }
    else if (node->token->type == TOKEN_FUNCTION)
    {
        ASTNode *function_ast = (ASTNode *)map_get(functions, node->token->value, (strlen(node->token->value) + 1) * sizeof(char));
        for (int i = 0; i < node->args->size; i++)
        {
            ASTNode *arg = (ASTNode *)list_get(node->args, i);
            Value *val = execute(arg, cache, functions);
            char *var_name = ((ASTNode *)list_get(function_ast->args, i))->token->value;
            map_add(cache, var_name, val, (strlen(var_name) + 1) * sizeof(char));
        }
        Value *val = NULL;
        for (int i = 0; i < function_ast->body->size; i++)
        {
            ASTNode *node = (ASTNode *)list_get(function_ast->body, i);
            val = execute(node, cache, functions);
        }
        if (val->type == TOKEN_VAR)
            return (Value *)map_get(cache, val->data, (strlen(val->data) + 1) * sizeof(char));

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
                for (int i = 0; i < node->body->size; i++)
                    execute(node->body->array[i], cache, functions);
            else
                execute(node->left, cache, functions);
        }
        else if (node->token->type == TOKEN_FOR)
        {
            if (cond->type == TOKEN_INT)
            {
                int *data = (int *)cond->   data;
                int start = data[0];
                int end = data[1];
                for (int i = start; i < end; i++)
                {
                    double *_i = malloc(sizeof(double));
                    *_i = (double)i;
                    char *var_name = node->condition->left->token->value;
                    int key_size = (strlen(var_name) + 1) * sizeof(char);
                    Value *val = initValue(_i, TOKEN_FLOAT);
                    map_add(cache, var_name, val, key_size);
                    for (int j = 0; j < node->body->size; j++)
                        execute(list_get(node->body, j), cache, functions);
                    map_delete(cache, var_name, key_size);
                    free(_i);
                    free(val);
                }
            }
            else if (cond->type == TOKEN_VAR)
            {
                char *list_var = (char *)cond->data;
                char *var_name = node->condition->left->token->value;
                Value *val = (Value *)map_get(cache, list_var, (strlen(list_var) + 1) * sizeof(char));
                List *l = val->data;
                for (int i = 0; i < l->size; i++)
                {
                    Value *a = (Value *)list_get(l, i);
                    int key_size = (strlen(var_name) + 1) * sizeof(char);
                    map_add(cache, var_name, a, key_size);
                    for (int j = 0; j < node->body->size; j++)
                        execute(list_get(node->body, j), cache, functions);
                    map_delete(cache, var_name, key_size);
                }
            }
        }
        return NULL;
    }

    if (node->token->type == TOKEN_PRINT_FUNCTION)
    {
        char *var_name = (char *)l->data;
        Value *var = map_get(cache, var_name, (strlen(var_name) + 1) * sizeof(char));
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

    if (node->token->type == TOKEN_ELSE){
        for (int i = 0; i < node->body->size; i++)
            execute(list_get(node->body, i), cache, functions);
        return NULL;
    }

    if (node->token->type == TOKEN_EQUAL)
    {
        char *var_name = (char *)l->data;
        map_add(cache, var_name, r, (strlen(var_name) + 1) * sizeof(char));
        return r;
    }
    if (node->token->type == TOKEN_COLON)
        return r;

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