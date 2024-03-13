#ifndef OPTION_H
#define OPTION_H

#include <stdbool.h>

struct option
{
    bool d;
    bool H;
    bool L;
};

/**
 * Initialize an option structure on the head
 *
 * \return The allocated option
 */
struct option *option_init(void);

/**
 * \brief Set all options in the structure
 *
 * \param argv The option flag
 * \param opt The structure containing options
 * \return whether argv is a option flag
 */
bool set_option(char *argv, struct option *opt);

#endif /* ! OPTION_H */
