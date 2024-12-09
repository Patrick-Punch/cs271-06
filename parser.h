/****************************************
 * Project 6 for CS 271
 *
 * Patrick Punch
 * Fall 2024
 *
 ****************************************/
#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include "hack.h"

typedef int16_t hack_addr;
typedef int16_t opcode;

#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH (MAX_LINE_LENGTH - 2)
#define MAX_HACK_ADDRESS INT16_MAX
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS

/** function prototypes **/
char *strip(char *s);

bool is_Atype(const char *);

bool is_label(const char *);

bool is_Ctype(const char *);

char *extract_label(const char *line, char *label);

enum instr_type
{
    INVALID_INSTR = -1,
    A_INSTR,
    C_INSTR
};

typedef struct c_instruction
{
    opcode a:1;
    opcode comp:7;
    opcode dest:4;
    opcode jmp:4;
} c_instruction;

typedef struct a_instruction
{
    union {
        hack_addr address;
        char *label;
    };
    bool is_addr;
} a_instruction;

typedef struct instruction
{
    union {
        a_instruction a_instr;
        c_instruction c_instr;
    };
    enum instr_type itype;
} instruction;

void add_predefined_symbols();

bool parse_A_instruction(const char *line, a_instruction *instr);

void parse_C_instruction(char *line, c_instruction *instr);

int parse(FILE *file, instruction *instructions);

void assemble(const char *file_name, instruction *instructions, int num_instructions);

opcode instruction_to_opcode(c_instruction instr);

#endif