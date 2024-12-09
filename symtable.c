/****************************************
 * Project 6 for CS 271
 *
 * Patrick Punch
 * Fall 2024
 *
 ****************************************/

#include "symtable.h"

Symbol *hashArray[SYMBOL_TABLE_SIZE];

int hash(char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % SYMBOL_TABLE_SIZE;
}

void symtable_insert(char *key, hack_addr addr)
{
    Symbol *item = (Symbol *)malloc(sizeof(Symbol));
    item->name = strdup(key);
    item->addr = addr;

    int hashIndex = hash(key);

    while (hashArray[hashIndex] != NULL){
        ++hashIndex;
        hashIndex %= SYMBOL_TABLE_SIZE;
    }
    hashArray[hashIndex] = item;
}

Symbol *symtable_find(char *key)
{
    int hashIndex = hash(key);

    while (hashArray[hashIndex] != NULL){
        if (strcmp(hashArray[hashIndex]->name, key) == 0){
            return hashArray[hashIndex];
        }
        ++hashIndex;
        hashIndex %= SYMBOL_TABLE_SIZE;
    }
    return NULL;
}

void symtable_display_table()
{
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++){
        if (hashArray[i] != NULL){
            printf("Hashtable[%d] -> (%s, %d)\n", i, hashArray[i]->name, hashArray[i]->addr);
        } else {
            printf("Index %d: ~~\n", i);
        }
    }
}

void symtable_print_labels()
{
    int i = 0;
    for (i = 0; i < SYMBOL_TABLE_SIZE; i++)
    {
        if (hashArray[i] != NULL){
            printf("{%s,%d}\n", hashArray[i]->name, hashArray[i]->addr);
        }
    }
}
