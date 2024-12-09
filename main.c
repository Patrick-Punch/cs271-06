/****************************************
 * Project 6 for CS 271
 * 
 * Patrick Punch
 * Fall 2024
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"
#include "hack.h"

#define MAX_INSTRUCTION_COUNT 30000
int main(int argc, const char *argv[])
{	
	int num_instructions;

	if (argc != 2){
		// incorrect number of arguments
		exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
	}
	const char *file_name = argv[1];
	FILE *file = fopen(file_name, "r");
	if (file == NULL)
	{
		exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
	}
	instruction *instructions = malloc(MAX_INSTRUCTION_COUNT * sizeof(instruction));
	num_instructions = parse(file, instructions);
	// printf("Number of instructions parsed: %d\n", num_instructions);

	// symtable_print_labels();
	// symtable_display_table();
	fclose(file);
	free(instructions);
	return 1;
}
