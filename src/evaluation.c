#include "evaluation.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static bool evaluation_act(char *pwd, struct ast *ast, bool *tmp)
{
    *tmp = true;
    switch (ast->type)
    {
    case PRINT:
        printf("%s\n", pwd);
        return true;
    case DELETE:
        if (!remove(pwd))
        {
            fprintf(stderr, "myfind: cannot delete '%s'\n", pwd);
            return false;
        }
        return true;
    case EXEC:
        /*FALLTHROUGH*/
    case EXECDIR:
        return !exec(pwd, ast->data.argv, ast->type == EXECDIR);
    default:
        return false;
    }
}

bool evaluation(char *pwd, struct ast *ast, bool *tmp)
{
    if (!ast)
        return true;

    switch (ast->type)
    {
    case OR:
        return evaluation(pwd, ast->data.oper.left, tmp)
            || evaluation(pwd, ast->data.oper.right, tmp);
    case AND:
        return evaluation(pwd, ast->data.oper.left, tmp)
            && evaluation(pwd, ast->data.oper.right, tmp);
    case NOT:
        return !evaluation(pwd, ast->data.oper.left, tmp);
    case NAME:
        return name(pwd, ast->data.argv);
    case TYPE:
        return type(pwd, ast->data.argv);
    case NEWER:
        return newer(pwd, ast->data.argv);
    case PERM:
        return perm(pwd, ast->data.argv);
    case USER:
        return user(pwd, ast->data.argv);
    case GROUP:
        return group(pwd, ast->data.argv);
    default:
        return evaluation_act(pwd, ast, tmp);
    }
}

static char *build_pwd(char *path, char *file)
{
    size_t n = strlen(path);
    char *pwd = malloc(n + strlen(file) + sizeof(char) * 2);

    strcpy(pwd, path);

    if (n != 0 && *(pwd + n - 1) != '/')
        strcat(pwd, "/");
    strcat(pwd, file);
    return pwd;
}

bool find(char *pwd, struct ast *ast, struct option *opt)
{
    struct stat sb;
    if (lstat(pwd, &sb))
    {
        fprintf(stderr, "myfind: Invalid file\n");
        return false;
    }

    bool tmp = false;
    if (!opt->d && evaluation(pwd, ast, &tmp) && !tmp)
        printf("%s\n", pwd);

    DIR *dir = opendir(pwd);
    if (dir
        && (S_ISDIR(sb.st_mode) || (S_ISLNK(sb.st_mode) && (opt->L || opt->H))))
    {
        opt->H = false;
        struct dirent *entry = readdir(dir);

        for (; entry; entry = readdir(dir))
        {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            {
                char *res = build_pwd(pwd, entry->d_name);
                if (!find(res, ast, opt))
                {
                    closedir(dir);
                    free(res);
                    return false;
                }
                free(res);
            }
        }
    }

    if (opt->d && evaluation(pwd, ast, &tmp) && !tmp)
        printf("%s\n", pwd);

    closedir(dir);
    return true;
}

bool is_expr(char c)
{
    return c == '-' || c == '!' || c == '(' || c == ')';
}

bool find_files(char *argv[], size_t i, struct ast *ast, struct option *opt)
{
    while (argv[i] && !is_expr(argv[i][0]))
    {
        bool tmp = opt->H;
        if (!find(argv[i], ast, opt))
            return false;
        if (tmp)
            opt->H = true;
        i++;
    }
    return true;
}
