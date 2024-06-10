/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include "linked_list.h"
#include "instruction_info.h"

#pragma once


struct Instruction {
    ListNode *operands;
    int instruction_counter;
    InstructionInfo *info;
} typedef Instruction;


/**
 * Parse an instruction string into an Instruction struct instance.
 */
int parse_instruction(char *string, ListNode *symbol_table, int instruction_counter, Instruction *res,
                      char *error_msg);


/**
 * Convert an instruction to words
 */
int instruction_to_words(Instruction instruction, ListNode **res_words);

/**
 * Count the binary words required to represent the given instruction.
 */
int count_instruction_words(char *string);