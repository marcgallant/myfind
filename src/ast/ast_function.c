#include "ast.h"

bool name(char *pwd, char *args)
{
    char *tmp = basename(pwd);
    return !fnmatch(args, tmp, 0);
}

bool type(char *pwd, char *args)
{
    struct stat sb;
    lstat(pwd, &sb);

    if (!strcmp(args, "f"))
        return S_ISREG(sb.st_mode);
    else if (!strcmp(args, "d"))
        return S_ISDIR(sb.st_mode);
    else if (!strcmp(args, "c"))
        return S_ISCHR(sb.st_mode);
    else if (!strcmp(args, "b"))
        return S_ISBLK(sb.st_mode);
    else if (!strcmp(args, "p"))
        return S_ISFIFO(sb.st_mode);
    else if (!strcmp(args, "l"))
        return S_ISLNK(sb.st_mode);
    return S_ISSOCK(sb.st_mode);
}

bool newer(char *pwd, char *args)
{
    struct stat sb1;
    struct stat sb2;
    stat(pwd, &sb1);
    stat(args, &sb2);
    return sb1.st_mtim.tv_sec > sb2.st_mtim.tv_sec;
}

unsigned long my_atoi_octal(const char *s)
{
    unsigned long res = 0;
    for (size_t i = 0; *(s + i) != '\0'; i++)
    {
        if (*(s + i) < '0' || *(s + i) > '7')
            return 778;

        res *= 8;
        res += *(s + i) - '0';
    }
    return 511 & res;
}

bool perm(char *pwd, char *args)
{
    struct stat sb;
    stat(pwd, &sb);
    unsigned long mode = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    switch (args[0])
    {
    case '-': {
        unsigned long tmp = my_atoi_octal(args + 1);
        return (tmp & mode) == tmp;
    }
    case '/':
        return (my_atoi_octal(args + 1) & mode) > 0;
    default:
        return my_atoi_octal(args) == mode;
    }
}

bool user(char *pwd, char *args)
{
    struct passwd *tmp = getpwnam(args);
    struct stat sb;
    stat(pwd, &sb);
    struct passwd *pw = getpwuid(sb.st_uid);
    return pw && !strcmp(pw->pw_name, tmp->pw_name);
}

bool group(char *pwd, char *args)
{
    struct group *tmp = getgrnam(args);
    struct stat sb;
    stat(pwd, &sb);
    struct group *gr = getgrgid(sb.st_gid);
    return gr && !strcmp(gr->gr_name, tmp->gr_name);
}

static int exec_fork(char *pwd, char *argc[], bool is_execdir)
{
    pid_t pid = fork();
    if (pid == -1)
        exit(0);

    if (pid == 0)
    {
        if (is_execdir)
            chdir(dirname(pwd));

        execvp(argc[0], argc);
        exit(0);
    }
    else
    {
        int status = 1;
        waitpid(pid, &status, 0);
        return status;
    }
}

int exec(char *pwd, char *args, bool is_execdir)
{
    char *rest[100] = { NULL };
    char *argscpy = strdup(args);
    size_t i = 0;

    char *tmp;
    if (is_execdir)
    {
        tmp = calloc(strlen(pwd) + 3, sizeof(char));
        strcat(tmp, "./");
        strcat(tmp, basename(pwd));
    }
    else
    {
        tmp = calloc(strlen(pwd) + 3, sizeof(char));
        strcpy(tmp, pwd);
    }

    char *saveptr;
    char *token = strtok_r(argscpy, " ", &saveptr);

    while ((token != NULL))
    {
        if (!strcmp(token, "{}"))
            rest[i] = strdup(tmp);
        else
            rest[i] = strdup(token);

        token = strtok_r(NULL, " ", &saveptr);
        i++;
    }
    free(tmp);
    free(argscpy);

    int err = exec_fork(pwd, rest, is_execdir);

    i = 0;
    while (rest[i])
    {
        free(rest[i]);
        i++;
    }

    return err;
}
