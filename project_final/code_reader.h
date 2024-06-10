/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include "linked_list.h"


#pragma once


struct LineOfCode {
    int idx;
    char *code;
    char *label;
} typedef LineOfCode;

int line_of_code_to_string(LineOfCode * line, char * dest);

void line_of_code_free(LineOfCode * line_of_code);

LineOfCode *line_of_code_init(char *label, char *code, int idx);

/**
 * @brief
 *  Read code from the given file path into list of lines of code.
 *  Ignore comments and blank lines.
 */
int read_lines_of_code(char *file_path, ListNode **res);
