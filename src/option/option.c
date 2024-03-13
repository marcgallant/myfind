#include "option.h"

#include <stdlib.h>
#include <string.h>

struct option *option_init(void)
{
    struct option *res = calloc(1, sizeof(struct option));
    return res;
}

bool set_option(char *argv, struct option *opt)
{
    if (!strcmp(argv, "-d"))
    {
        opt->d = true;
        return true;
    }
    else if (!strcmp(argv, "-H"))
    {
        opt->H = true;
        opt->L = false;
        return true;
    }
    else if (!strcmp(argv, "-L"))
    {
        opt->L = true;
        opt->H = false;
        return true;
    }
    else if (!strcmp(argv, "-P"))
    {
        opt->L = false;
        opt->H = false;
        return true;
    }

    return false;
}
