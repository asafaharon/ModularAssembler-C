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


int assemble(
        ListNode *lines_of_code,
        ListNode *symbol_table,
        ListNode **res_words,
        ListNode **res_data_words
);

int build_symbol_table(ListNode *lines_of_code, ListNode **res_symbol_table);