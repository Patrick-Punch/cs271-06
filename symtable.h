/****************************************
 * Project 6 for CS 271
 *
 * Patrick Punch
 * Fall 2024
 *
 ****************************************/
#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SYMBOL_TABLE_SIZE 1000

typedef int16_t hack_addr;

typedef struct Symbol 
{
    char *name;
    hack_addr addr;
} Symbol;

int hash(char *str);

struct Symbol *symtable_find(char *key);

void symtable_insert(char *key, hack_addr addr);

void symtable_display_table();

void symtable_print_labels();


#endif
