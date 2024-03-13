# myfind

myfind is a project that emulates find(1).

## Project Description

This project was made as my first project during my third year (ING1) at EPITA.
It took me one week and has the purpose to reproduce the comportement of find(1), which is to print the directory tree rooted at each given starting-point if the current file is evaluating the given expression from left to right, according to the  rules of precedence until the outcome is known, at which point find moves on to the next  file  name.
If no starting-point is specified, ‘.' is assumed.

## Makefile
- Produce the myfind binary
```sh
make myfind
```
- Call the myfind rule producing the binary
```sh
make all
```
- Produce the myfind binary and start the testsuite
```sh
make check
```
- Deletes everything produced by make
```sh
make clean
```

## Usage

```sh
./myfind [starting-point...] [expressions]
```

## Supported options

- d: myfind should process each directory’s content before the directory itself. By default, myfind visits directories in pre-order (before
their content);
- H: myfind does not follow symbolic links, except while processing command line arguments;
- L: myfind follows symbolic links;
- P: myfind never follows symbolic links. This is the default behavior

## Supported expression

It supports the following expression :
- print: Prints the path of the currently examined file. It always returns true
- name: Takes a pattern as parameter and returns true if the current filename matches the parameter, handling globbing
- type takes a parameter and returns true if the type of the current file matches the parameter. You
must handle the following types: b (special files in block mode), c (special files in character mode), d (directories), f (regular files), l (symbolic links), p (named pipes or FIFO), s (sockets).
- o is an operator placed between two expressions, as follows: expr1 -o expr2. If expr1 is evaluated
to true, expr2 will not be executed.
- a is an operator placed between two expressions, as follows: expr1 -a expr2. If expr1 is evaluated
to false, expr2 will not be executed.
- newer takes a file as parameter and returns true if the currently-examined file has a last modification
date more recent than the file given as argument.
- perm takes a mode in octal as parameter and returns true if the current file’s permission bits match
exactly the mode.
- perm - takes a mode in octal as parameter and returns true if all the permission bits in mode are
set for the file.
- perm / takes a mode in octal as parameter and return true if any of the current file’s permission bits
is set in mode.
- user takes a username and returns true if the file is owned by the user username.
- group takes a groupname and returns true if the file belongs to the group groupname.
- !
- ( and )
- exec The -exec action executes the command passed by argument, delimited by a semicolon. Every {}
string encountered in the command must be replaced by the current filename. It returns true if the
command returned 0, false otherwise
- execdir behavior is similar to -exec, except it executes the command in the current file’s directory.
{} placeholders will be replaced by the current file name, preceded by ./ (relative file path).
- delete deletes files and returns true if it succeeded. If the deletion fails, an error message is dis-
played. Use of -delete implies use of -d.
