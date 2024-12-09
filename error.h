/****************************************
 * Project 6 for CS 271
 *
 * Patrick Punch
 * Fall 2024
 *
 ****************************************/
#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdarg.h>

typedef enum exitcode
{
    /*
     * Error code 1 represents that given file does not exist.
     * Error code 2 represents that given file cannot be opened.
     * Error code 3 represents that given file exceeds the limit of instructions.
     * Error code 4 represents that given file contains an invalid label.
     * Error code 5 represents that given file contains duplicate labels.
     * Error code 6 represents that given file contains an invalid A instruciton.
     * Error code 7 represents that given file contains an invalid C (dest) instruciton.
     * Error code 8 represents that given file contains an invalid C (comp) instruciton.
     * Error code 9 represents that given file contains an invalid C (jump) instruciton.
     */
    EXIT_INCORRECT_ARGUMENTS,
    EXIT_CANNOT_OPEN_FILE,
    EXIT_TOO_MANY_INSTRUCTIONS,
    EXIT_INVALID_LABEL,
    EXIT_SYMBOL_ALREADY_EXISTS,
    EXIT_INVALID_A_INSTR,
    EXIT_INVALID_C_DEST,
    EXIT_INVALID_C_COMP,
    EXIT_INVALID_C_JUMP
} exitcode;

void exit_program(exitcode code, ...);

#endif