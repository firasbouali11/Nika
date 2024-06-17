#pragma once

#include "Parser.h"
#include "stdc.h"

typedef struct
{
    void *data;
    int type;
} Value;

Value *execute(ASTNode *node, Map *cache, Map *functions);