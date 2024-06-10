/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include "linked_list.h"
#include "instruction.h"
#include "instruction_info.h"


#pragma once


/**
 * Write the external file to the disk.
 */
int dump_externals_file(ListNode *symbol_table, char *file_path);

/**
 * Write the object file to the disk.
 */
int dump_object_file(ListNode *assembled_words, ListNode* assembled_data_words, char *file_path);


/**
 * Write the entries file to the disk.
 */
int dump_entries_file(ListNode *symbol_table, char *file_path);