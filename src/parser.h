#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "queue/queue.h"
#include "stack/stack.h"

/**
 * \brief Parse the queue of AST into one AST
 *
 * \param queue The queue of AST resulting from lexing
 * \return The parsed AST
 */
struct ast *parser(struct queue *queue);

#endif /* ! PARSER_H */
