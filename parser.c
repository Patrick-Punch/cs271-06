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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s)
{
	int i = 0;
	char temp[strlen(s) + 1];
	for (char *s2 = s; *s2; s2++){
		if (*s2 == '/' && *(s2 + 1) == '/'){
			break;
		} else if (!isspace(*s2)){
			temp[i++] = *s2;
		}
	}
	temp[i] = '\0';
	strcpy(s, temp);
	return s;
	}

	/* Function: parse
	 * -------------
	 * iterate each line in the file and strip whitespace and comments.
	 *
	 * file: pointer to FILE to parse
	 *
	 * returns: nothing
	 */
	int parse(FILE *file, instruction *instructions)
	{
		char line[MAX_LINE_LENGTH] = {0};
		char label[MAX_LABEL_LENGTH] = {0};
		int line_num = 0;
		int instr_num = 0;
		instruction instr;
		char tmp_line[MAX_LINE_LENGTH];

		add_predefined_symbols();
		while (fgets(line, sizeof(line), file)) {
			line_num++;
			if (instr_num > MAX_INSTRUCTIONS) {
				exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
			}
			strip(line);
			if (!*line) {
				continue;
			}
			else if (is_label(line)) {
				extract_label(line, label);
				strcpy(line, label);
				if (!isalpha(label[0])) {
					exit_program(EXIT_INVALID_LABEL, line_num, line);
				} else if (symtable_find(label) != NULL) {
					exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
				}
				symtable_insert(label, instr_num);
				continue;
			} else if (is_Atype(line)) {
				if (!parse_A_instruction(line, &instr.a_instr)) {
					exit_program(EXIT_INVALID_A_INSTR, line_num, line);
				}
				instr.itype = A_INSTR;
				// Print A instr
				if (instr.a_instr.is_addr) {
					printf("A: %d\n", instr.a_instr.address); // Address
				} else {
					printf("A: %s\n", instr.a_instr.label); // Label name
				}
			} else if (is_Ctype(line)) {	
				strcpy(tmp_line, line);
				parse_C_instruction(tmp_line, &instr.c_instr);
				if (instr.c_instr.dest == DEST_INVALID) {
					exit_program(EXIT_INVALID_C_DEST, line_num, line);
				} else if (instr.c_instr.comp == COMP_INVALID) {
					exit_program(EXIT_INVALID_C_COMP, line_num, line);
				} else if (instr.c_instr.jmp == JMP_INVALID) {
					exit_program(EXIT_INVALID_C_JUMP, line_num, line);
				}
				instr.itype = C_INSTR;
				// Print C instr
				printf("C: d=%d, c=%d, j=%d\n", instr.c_instr.dest, instr.c_instr.comp, instr.c_instr.jmp);
			}
			instructions[instr_num++] = instr;
		}
		return instr_num;
	}

	bool is_Atype(const char *line)
	/* Function: is_Atype
	 * -------------
	 * Determine if the line is an A type assembly command
	 *
	 * line: pointer to the line to check
	 *
	 * returns: True if A type, false otherwise
	 */
	{
		if (line != NULL && line[0] == '@') {
			return true;
		}
		return false;
	}

	bool is_label(const char *line)
	/* Function: is_label
	 * -------------
	 * Determine if the line is an assembly label declaration
	 *
	 * line: pointer to the line to check
	 *
	 * returns: True if it is a label, false otherwise
	 */
	{
		size_t len = strlen(line);
		return (line[0] == '(' && line[len - 1] == ')');
	}

	bool is_Ctype(const char *line)
	/* Function: is_Ctype
	 * -------------
	 * Determine if the line is a C type assembly command
	 *
	 * line: pointer to the line to check
	 *
	 * returns: True if C type, false otherwise
	 */
	{
		if (!is_label(line) && !is_Atype(line)) {
			return true;
		} else {
			return false;
		}
	}

	char *extract_label(const char *line, char *label)
	/* Function: extract_label
	 * -------------
	 * Remove parenthesis from a label, to extract only the label that was declared
	 *
	 * line: pointer to the line to check
	 * 
	 * label: the label to be stripped
	 *
	 * returns: the label without the parenthesis
	 */
	{
		size_t len = strlen(line);
		strncpy(label, line + 1, len - 2);
		label[len - 2] = '\0';
		return label;
	}

	void add_predefined_symbols()
	{
		for (int i =0; i < NUM_PREDEFINED_SYMBOLS; i++) {
			predefined_symbol symbol = predefined_symbols[i];
			symtable_insert(symbol.name, symbol.address);
		}
	}

	bool parse_A_instruction(const char *line, a_instruction *instr)
	{
		if (line[0] != '@') {
			return false; // Not an A instruction
		}
		size_t len = strlen(line);
		char *s = (char *)malloc(len);
		if (!s) {
			return false;
		}
		strcpy(s, line + 1);
		char *s_end = NULL;
		long result = strtol(s, &s_end, 10);
		if (s == s_end) {
			instr->label = (char *)malloc(len);
			if (!instr->label) {
				return false;
			}
			strcpy(instr->label, s); // Copy label to instruction
			instr->is_addr = false; // Mark as a label
		} else if (*s_end != '\0') {
			return false; // Invalid A-instruction
		} else {
			instr->address = (int16_t)result; // Set address
			instr->is_addr = true; // Mark as an address
		}
		return true;
	}


	void parse_C_instruction(char *line, c_instruction *instr)
	{
		instr->jmp = JMP_INVALID;
		instr->comp = COMP_INVALID;
		instr->dest = DEST_INVALID;
		// Split line into dest comp and jmp instructions
		char *temp = strtok(line, ";");
		char *jmp = strtok(NULL, ";");
		// Set jmp value
		instr->jmp = (jmp != NULL) ? str_to_jumpid(jmp) : JMP_NULL;

		char *dest = strtok(temp, "=");
		char *comp = strtok(NULL, "=");

		comp = (comp == NULL) ? dest : comp;
		dest = (comp == dest) ? NULL : dest;
		// Set dest value
		instr->dest = (dest != NULL) ? str_to_destid(dest) : DEST_NULL;
		int a;
		// Set comp value
		instr->comp = (comp != NULL) ? str_to_compid(comp, &a) : COMP_INVALID;
		instr->a = a;
	}

	void assemble(const char *file_name, instruction *instructions, int num_instructions)
	{
		char output_file_name[256];
		snprintf(output_file_name, sizeof(output_file_name), "%s.hack", file_name);
		FILE *output_file = fopen(output_file_name, "w");
		if (output_file == NULL) {
			exit_program(EXIT_CANNOT_OPEN_FILE, file_name);
		}

		int variable_address = 16;

		for (int i = 0; i < num_instructions; i++) {
			opcode op = 0;
			// Handle A instructions
			if (instructions[i].itype == A_INSTR) {
				if (!instructions[i].a_instr.is_addr) { // If it's a label
					Symbol *symbol = symtable_find(instructions[i].a_instr.label);
					if (symbol == NULL) {
						symtable_insert(instructions[i].a_instr.label, variable_address++);
						symbol = symtable_find(instructions[i].a_instr.label);
					}
					op = symbol->addr; // Use the symbol's address
				} else { // If it's an address
					op = (opcode)instructions[i].a_instr.address;
				}
			} else if (instructions[i].itype == C_INSTR) { // Handle C instructions
				op = instruction_to_opcode(instructions[i].c_instr);
			}
			fprintf(output_file, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(op));
		}
		fclose(output_file);
	}

	opcode instruction_to_opcode(c_instruction instr)
	{
		opcode op = 0;
		op |= (7 << 13);
		op |= (instr.a << 12);
		op |= (instr.comp << 6);
		op |= (instr.dest << 3);
		op |= instr.jmp;
		return op;
	}