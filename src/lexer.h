#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

#include "ast/ast.h"
#include "option/option.h"
#include "queue/queue.h"
#include "stack/stack.h"

/**
 * \brief Lex the argument into a queue of AST
 *
 * \param argv List of argument
 * \param start Start of argv for lexing
 * \param opt Option structure
 *
 * \return The queue or NULL on error
 */
struct queue *lexer(char *argv[], size_t start, struct option *opt);

#endif /* ! LEXER_H */
