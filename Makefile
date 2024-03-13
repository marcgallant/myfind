# Simple Makefile

# Compilers vars
CC= gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla \
	-Isrc
LDFLAGS=
LDLIBS=

SRC= src/ast/ast.c \
     src/ast/ast_function.c \
     src/queue/queue.c \
     src/stack/stack.c \
     src/option/option.c \
     src/lexer.c \
     src/parser.c \
     src/evaluation.c \
     src/main.c

OBJ= $(SRC:.c=.o)

.PHONY: all clean clean

myfind: $(OBJ)
	$(CC) -g $(SRC) -L -lmalloc -o myfind

all: myfind

check: myfind
	cd tests && ./basics.sh

clean:
	$(RM) $(OBJ) myfind

# END Makefile
