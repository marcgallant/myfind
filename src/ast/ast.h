#ifndef AST_H
#define AST_H

#define _POSIX_C_SOURCE 200809L

#include <fnmatch.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum expr_type
{
    /*OPER*/
    B_OPEN,
    OR,
    AND,
    NOT,
    B_CLOSE,
    /* EXPR*/
    NAME,
    TYPE,
    NEWER,
    PERM,
    USER,
    GROUP,
    /*ACTION*/
    PRINT,
    DELETE,
    EXEC,
    EXECDIR
};

struct ast
{
    enum expr_type type;
    union data
    {
        struct operation
        {
            struct ast *left;
            struct ast *right;
        } oper;
        char *argv;
    } data;
};

/**
 * \param ast The AST to evaluate
 * \return Whether the AST is a operator
 */
bool is_oper(struct ast *ast);

/**
 * \param ast The AST to evaluate
 * \return Whether the AST is a action
 */
bool is_act(struct ast *ast);

/**
 * \brief Destroy the AST
 *
 * Free all elements contained and the AST itself
 *
 * \param ast
 */
void ast_destroy(struct ast *ast);

/**
 * \brief -name flag function
 *
 * \param pwd Current file or directory
 * \param args Pattern to test
 * \return Whether the current file name match the given pattern
 */
bool name(char *pwd, char *args);

/**
 * \brief -type flag function
 *
 * \param pwd Current file or directory
 * \param args Type to test
 * \return Whether the current file type match the given type
 */
bool type(char *pwd, char *args);

/**
 * \brief -newer flag function
 *
 * \param pwd Current file or directory
 * \param args File to test
 * \return Whether the current file is newer than the given file
 */
bool newer(char *pwd, char *args);

/**
 * \brief Convert string s into octal value
 *
 * \param s String to convert
 * \return Octal value or 778 on invalid character
 */
unsigned long my_atoi_octal(const char *s);

/**
 * \brief -perm flag function
 *
 * \param pwd Current file or directory
 * \param args Perm to test
 * \return Whether the current file as the given perm
 */
bool perm(char *pwd, char *args);

/**
 * \brief -user flag function
 *
 * \param pwd Current file or directory
 * \param args Username to test
 * \return Whether the current file is owned by the given username
 */
bool user(char *pwd, char *args);

/**
 * \brief -group flag function
 *
 * \param pwd Current file or directory
 * \param args Groupname to test
 * \return Whether the current file belongs to the given groupname
 */
bool group(char *pwd, char *args);

/**
 * \brief -exec flag function
 *
 * The -exec action executes the command passed by argument, delimited by a
 * semicolon. Every {} string encountered in the command must be replaced by the
 * current filename. It returns true if the command returned 0, false otherwise
 *
 * \param pwd Current file or directory
 * \param args The command to execute
 * \param is_execdir Whether is it -exec or -execdir
 *
 * \return Command return
 */
int exec(char *pwd, char *args, bool is_execdir);

/**
 * \brief Allocate an AST on the heap
 *
 * \param key String representing the AST type
 * \return The allocated AST
 */
struct ast *ast_init(char *key);

#endif /* ! AST_H */
