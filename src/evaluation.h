#ifndef EVALUATION_H
#define EVALUATION_H

#include <stdbool.h>
#include <stddef.h>

#include "ast/ast.h"
#include "option/option.h"

/**
 * \brief Execute the AST on the current file
 *
 * \param pwd Current path
 * \param ast AST to execute
 * \return Whether the current file check the AST conditions
 */
bool evaluation(char *pwd, struct ast *ast, bool *tmp);

/**
 * \brief Recursive function on the file tree
 *
 * Print the file name if the file check the AST conditions depending on the
 * option structure
 *
 * \param argv List of argument
 * \param ast AST to execute
 * \param opt Option Structure
 * \return Whether an error occurred while opening files
 */
bool find(char *argv, struct ast *ast, struct option *opt);

/**
 * \param c The character to evaluate
 * \return Whether the character is a expression
 */
bool is_expr(char c);

/**
 * \brief Find all the starting file in the list of argument (i-e until a expr
 * is find)
 *
 * \param argv The list of argument
 * \param ast Ast to execute
 * \param opt Option structure
 *
 * \return Whether an error occurred while opening files
 */
bool find_files(char *argv[], size_t i, struct ast *ast, struct option *opt);

#endif /* ! EVALUATION_H */
