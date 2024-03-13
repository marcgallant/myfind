#include <stdio.h>
#include <string.h>

#include "evaluation.h"
#include "lexer.h"
#include "option/option.h"
#include "parser.h"

static bool build_ast(char *argv[], size_t i, size_t j, struct option *opt)
{
    struct queue *queue = lexer(argv, j, opt);
    if (!queue)
    {
        free(opt);
        return 1;
    }

    struct ast *ast = parser(queue);
    queue_destroy(queue);
    if (!ast)
    {
        free(opt);
        return 1;
    }

    bool res;
    if (i == j)
        res = find(".", ast, opt);
    else
        res = find_files(argv, i, ast, opt);

    free(opt);
    ast_destroy(ast);
    return !res;
}

int main(int argc, char *argv[])
{
    if (argc == 0)
        return 1;

    struct option *opt = option_init();

    size_t i = 1;
    while (argv[i] && set_option(argv[i], opt))
    {
        i++;
    }

    size_t j = i;
    while (argv[j] && !is_expr(argv[j][0]))
    {
        j++;
    }

    if (!argv[j])
    {
        bool res;
        if (j == i)
            res = find(".", NULL, opt);
        else
            res = find_files(argv, i, NULL, opt);

        free(opt);
        return !res;
    }

    return build_ast(argv, i, j, opt);
}
