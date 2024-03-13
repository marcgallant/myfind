#include "ast.h"

bool is_oper(struct ast *ast)
{
    return ast->type < 5;
}

bool is_act(struct ast *ast)
{
    return ast->type > 10;
}

void ast_destroy(struct ast *ast)
{
    if (!ast)
        return;

    if (is_oper(ast))
    {
        ast_destroy(ast->data.oper.left);
        ast_destroy(ast->data.oper.right);
    }

    if (ast->type == EXEC || ast->type == EXECDIR)
        free(ast->data.argv);

    free(ast);
}

struct function
{
    char *name;
    enum expr_type type;
};

/**
 * \brief Lookup table
 */
struct function functions[15] = {
    { .name = "-o", .type = OR },           { .name = "-a", .type = AND },
    { .name = "!", .type = NOT },           { .name = "(", .type = B_OPEN },
    { .name = ")", .type = B_CLOSE },       { .name = "-name", .type = NAME },
    { .name = "-type", .type = TYPE },      { .name = "-newer", .type = NEWER },
    { .name = "-perm", .type = PERM },      { .name = "-user", .type = USER },
    { .name = "-group", .type = GROUP },    { .name = "-print", .type = PRINT },
    { .name = "-delete", .type = DELETE },  { .name = "-exec", .type = EXEC },
    { .name = "-execdir", .type = EXECDIR }
};

struct ast *ast_init(char *key)
{
    struct ast *res = calloc(1, sizeof(struct ast));
    for (int i = 0; i < 15; ++i)
    {
        if (!strcmp(key, functions[i].name))
        {
            res->type = functions[i].type;
            return res;
        }
    }
    free(res);
    return NULL;
}
