/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdbool.h>
#include "linked_list.h"

#pragma once

typedef int LegalModes[4][8];

struct InstructionInfo {
    int max_operands;
    int min_operands;
    char *name;
    int opcode;

    /**
     * List of lists of addressing modes. Each list contains the legal addressing mode for
     * each operand.
     */
    ListNode *legal_modes;
} typedef InstructionInfo;


/**
 * Create a new instruction info object from the given parameters.
 */
InstructionInfo *new_instruction_info(char *name, int opcode, int min_operands, int max_operands,
                                      LegalModes legal_addressing_modes);


/**
 * Create the static instruction info table.
 */
int new_instruction_info_table();


/**
*  Get an instruction info record from the static instruction info table
 */
int get_instruction_info_record(char *instruction_name, InstructionInfo **res);

/**
* Return true if the given instruction is legal.
 */
bool is_legal_instruction(char *instruction_name);
