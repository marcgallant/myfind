#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool add_children(struct stack *stack, struct ast *oper)
{
    if (oper)
    {
        switch (oper->type)
        {
        case OR:
        /* FALLTHROUGH */
        case AND: {
            struct ast *a = stack_pop(stack);
            if (!a)
                break;

            struct ast *b = stack_pop(stack);
            if (!b)
            {
                free(a);
                break;
            }
            oper->data.oper.right = a;
            oper->data.oper.left = b;
            stack_push(stack, oper);
            return true;
        }
        default: // NOT
        {
            struct ast *a = stack_pop(stack);
            if (!a)
                break;
            oper->data.oper.left = a;
            stack_push(stack, oper);
            return true;
        }
        }
    }
    ast_destroy(oper);
    fprintf(stderr, "myfind: Invalid arguments\n");
    return false;
}

static bool is_sup(struct ast *a, struct ast *b)
{
    if ((a->type == OR || a->type == AND))
        return a->type <= b->type;
    else
        return a->type < b->type;
}

static bool parser_oper(struct stack *stack_oper, struct stack *stack_expr,
                        struct ast *ast, bool *add_and)
{
    if (ast->type == B_CLOSE)
    {
        while (stack_oper->peek && stack_oper->peek->data->type != B_OPEN)
        {
            if (!add_children(stack_expr, stack_pop(stack_oper)))
                return false;
        }

        if (!stack_oper->peek || stack_oper->peek->data->type != B_OPEN)
        {
            fprintf(stderr, "myfind: Invalid parenthesing\n");
            return false;
        }

        ast_destroy(stack_pop(stack_oper));
        ast_destroy(ast);

        if (stack_oper->peek
            && !add_children(stack_expr, stack_pop(stack_oper)))
            return false;
        *add_and = true;
    }
    else
    {
        if (ast->type != B_OPEN)
        {
            while (stack_oper->peek && is_sup(ast, stack_oper->peek->data))
            {
                if (!add_children(stack_expr, stack_pop(stack_oper)))
                    return false;
            }
        }

        stack_push(stack_oper, ast);
    }
    return true;
}

static struct ast *quit(struct stack *stack_oper, struct stack *stack_expr,
                        struct ast *res)
{
    stack_destroy(stack_oper);
    stack_destroy(stack_expr);
    return res;
}

struct ast *parser(struct queue *queue)
{
    struct stack *stack_oper = stack_init();
    struct stack *stack_expr = stack_init();

    bool add_and = false;
    while (queue->head)
    {
        struct ast *ast = queue_pop(queue);

        if (is_oper(ast))
        {
            add_and = ast->type == B_CLOSE;
            if (!parser_oper(stack_oper, stack_expr, ast, &add_and))
            {
                ast_destroy(ast);
                return quit(stack_oper, stack_expr, NULL);
            }
        }
        else
        {
            stack_push(stack_expr, ast);
            if (add_and
                && !parser_oper(stack_oper, stack_expr, ast_init("-a"),
                                &add_and))
                return quit(stack_oper, stack_expr, NULL);
            add_and = true;
        }
    }

    while (stack_oper->peek)
    {
        if (stack_oper->peek->data->type == B_OPEN)
        {
            fprintf(stderr, "myfind: Invalid parenthesizing\n");
            return quit(stack_oper, stack_expr, NULL);
        }
        if (!add_children(stack_expr, stack_pop(stack_oper)))
            return quit(stack_oper, stack_expr, NULL);
    }

    struct ast *res = NULL;
    if (stack_expr->size == 1)
        res = stack_pop(stack_expr);
    else
        fprintf(stderr, "myfind: Invalid arguments\n");

    return quit(stack_oper, stack_expr, res);
}
