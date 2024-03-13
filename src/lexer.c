#include "lexer.h"

#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static bool correct_arg(struct ast *ast, char *arg)
{
    if (!arg)
        return false;

    switch (ast->type)
    {
    case TYPE:
        return !strcmp(arg, "f") || !strcmp(arg, "d") || !strcmp(arg, "c")
            || !strcmp(arg, "b") || !strcmp(arg, "p") || !strcmp(arg, "l")
            || !strcmp(arg, "s");
    case NEWER: {
        struct stat sb2;
        return !stat(arg, &sb2);
    }
    case PERM:
        return ((arg[0] == '-' || arg[0] == '/')
                && my_atoi_octal(arg + 1) != 778)
            || my_atoi_octal(arg) != 778;
    case USER:
        return getpwnam(arg) != NULL;
    case GROUP:
        return getgrnam(arg) != NULL;
    default:
        return true;
    }
}

static size_t get_delim(char **arg)
{
    size_t i = 0;
    while (arg[i] && strcmp(arg[i], ";"))
    {
        i++;
    }
    return i;
}

static void lexer_exec(struct ast *ast, char **arg, size_t n)
{
    char *tmp = calloc(100, sizeof(char));

    for (size_t i = 0; i < n; i++)
    {
        strcat(tmp, arg[i]);
        strcat(tmp, " ");
    }

    ast->data.argv = tmp;
}

struct queue *lexer(char *argv[], size_t start, struct option *opt)
{
    struct queue *queue = queue_init();

    for (size_t i = start; argv[i]; i++)
    {
        struct ast *ast = ast_init(argv[i]);

        if (!ast)
        {
            fprintf(stderr, "myfind: Invalid token : %s\n", argv[i]);
            queue_destroy(queue);
            return NULL;
        }
        if (!is_oper(ast) && !is_act(ast))
        {
            if (correct_arg(ast, argv[i + 1]))
            {
                ast->data.argv = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "myfind: Invalid arguments: %s\n", argv[i + 1]);
                queue_destroy(queue);
                ast_destroy(ast);
                return NULL;
            }
        }
        else if (ast->type == DELETE)
            opt->d = true;
        else if (ast->type == EXEC || ast->type == EXECDIR)
        {
            size_t n = get_delim(argv + i + 1);
            if (!argv[n + i + 1])
            {
                fprintf(stderr, "myfind: Invalid arguments\n");
                queue_destroy(queue);
                ast_destroy(ast);
                return NULL;
            }
            lexer_exec(ast, argv + i + 1, n);
            i += n + 1;
        }

        queue_push(queue, ast);
    }
    return queue;
}
