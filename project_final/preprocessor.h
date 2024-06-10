/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"


#pragma once

struct Macro {
    char *name;
    ListNode *lines_of_code;
} typedef Macro;

/**
 * Create a new macro instance on the heap.
 */
Macro *new_macro(char *name, ListNode *lines_of_code);

int preprocess_lines_of_code(ListNode *lines_of_code, char *dest_path);