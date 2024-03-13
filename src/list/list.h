#ifndef LIST_H
#define LIST_H

#include "../ast/ast.h"

struct list
{
    struct ast *data;
    struct list *next;
};

#endif /* ! LIST_H */
