#pragma once

#include "Parser.h"
#include "../../lib/headers/Map.h"

typedef struct
{
    void *data;
    int type;
} Value;


Value *execute(ASTNode *node, Map* cache);