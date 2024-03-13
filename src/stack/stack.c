#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct stack *stack_init(void)
{
    struct stack *res = calloc(1, sizeof(struct stack));
    return res;
}

void stack_destroy(struct stack *stack)
{
    while (stack->peek)
    {
        struct list *tmp = stack->peek;
        stack->peek = stack->peek->next;

        ast_destroy(tmp->data);
        free(tmp);
    }
    free(stack);
}

void stack_push(struct stack *stack, struct ast *elt)
{
    struct list *tmp = calloc(1, sizeof(struct list));

    tmp->data = elt;

    if (stack->peek)
        tmp->next = stack->peek;

    stack->peek = tmp;
    stack->size++;
}

struct ast *stack_pop(struct stack *stack)
{
    struct list *tmp = stack->peek;
    if (!tmp)
        return NULL;

    stack->size--;
    stack->peek = stack->peek->next;

    struct ast *res = tmp->data;
    free(tmp);
    return res;
}
