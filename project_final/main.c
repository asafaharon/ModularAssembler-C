/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <string.h>
#include <stdio.h>
#include "linked_list.h"

#include "string_utils.h"
#include "instruction_info.h"
#include "code_reader.h"
#include "assembler.h"
#include "file_writers.h"
#include "preprocessor.h"

#define FILENAME_STRING_SIZE 4096


/**
 * Add the externals file extention to the given file path
 */
void format_externals_filename(char *file_path) { strncat(file_path, ".ext", FILENAME_STRING_SIZE - 1); }

/**
 *
 * Add the object file extension to the given file path
 */
void format_object_filename(char *file_path) { strncat(file_path, ".obj", FILENAME_STRING_SIZE - 1); }

/**
 * Add the 'am' file extension to the given file path
 */
void format_macro_expanded_filename(char *file_path) { strncat(file_path, ".am", FILENAME_STRING_SIZE - 1); }

/**
 * Add the 'as' file extension to the given file path.
 */
void format_assembly_filename(char *file_path) { strncat(file_path, ".as", FILENAME_STRING_SIZE - 1); }

int assemble_file(char *file_name) {
    char file_path[FILENAME_STRING_SIZE];

    ListNode *symbol_table;
    ListNode *lines_of_code;
    ListNode *assembled_data;
    ListNode *assembled_instructions;

    /*
     * Initialize information about all the instructions.
     */
    new_instruction_info_table();

    strcpy(file_path, file_name);
    format_assembly_filename(file_path);

    if (read_lines_of_code(file_path, &lines_of_code) != 0) {
        printf("ERROR - Couldn't read from the given path\n");
        return 1;
    }

    strcpy(file_path, file_name);
    format_macro_expanded_filename(file_path);

    if (preprocess_lines_of_code(lines_of_code, file_path) != 0) {
        printf("ERROR - Failed expanding macros.\n");
        return 1;
    }

    free_list(lines_of_code, (void (*)(void *)) line_of_code_free);
    if (read_lines_of_code(file_path, &lines_of_code) != 0) {
        printf("ERROR - Couldn't read from the preprocessed path.\n");
    }

    if (build_symbol_table(lines_of_code, &symbol_table) != 0) {
        printf("ERROR - building symbol table failed :(\n");
    }

    if (assemble(lines_of_code, symbol_table, &assembled_instructions, &assembled_data) != 0) {
        printf("ERROR - Failed during the assembling process.\n");
    }

    strcpy(file_path, file_name);
    format_object_filename(file_path);

    if (dump_object_file(assembled_instructions, assembled_data, file_path) != 0) {
        printf("ERROR - Could'nt write object file\n");
        return 1;
    }

    strcpy(file_path, file_name);
    strncat(file_path, ".ent", FILENAME_STRING_SIZE - 1);

    if (dump_entries_file(symbol_table, file_path) != 0) {
        printf("ERROR - Couldn't write entries file\n");
    }

    strcpy(file_path, file_name);
    format_externals_filename(file_path);

    if (dump_externals_file(symbol_table, file_path) != 0) {
        printf("ERROR - Couldn't write externals file\n");
    }

    return 0;
}

/**
 * Assembler's entry point.
 *
 * @param argc: Arguments count
 * @param argv: Arguments vector
 * @return: zero on success, nonzero on failure.
 */
int main(int argc, char **argv) {
    int i;

    /*
     * In case not enough arguments were provided.
     */
    if (argc <= 1) {
        printf("usage: %s [asm_file1] [asm_file2] ...", argv[0]);
    }

    /*
     * Iterate all the given files and assemble them.
     */
    for (i = 1; i < argc; i++) {
        assemble_file(argv[i]);
    }

    return 0;
}
