/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"
#include "string_utils.h"
#include "int_utils.h"
#include "operand.h"
#include "instruction_info.h"


static ListNode *instruction_info_list = NULL;

/**
 *Initialize a list of legal addressing modes.
 */
ListNode *legal_addressing_modes_init(LegalModes legal_addressing_modes) {
    int i;
    int j;
    ListNode *legal_modes;
    ListNode *legal_modes_items = new_list();
    for (i = 0; legal_addressing_modes[i][0] != -1; i++) {
        legal_modes = new_list();
        for (j = 0; legal_addressing_modes[i][j] != -1; j++) {
            list_append(legal_modes, int_init(legal_addressing_modes[i][j]));
        }
        list_append(legal_modes_items, legal_modes);
    }
    return legal_modes_items;
}

InstructionInfo *new_instruction_info(char *name, int opcode, int min_operands, int max_operands,
                                      LegalModes legal_addressing_modes) {
    InstructionInfo *res = malloc(sizeof(InstructionInfo));

    res->name = new_string(name);
    res->opcode = opcode;
    res->min_operands = min_operands;
    res->max_operands = max_operands;
    res->legal_modes = legal_addressing_modes_init(legal_addressing_modes);

    return res;
}

int add_legal_instruction(char *name, int opcode, int min_operands, int max_operands,
                          LegalModes legal_modes) {
    list_append(
            instruction_info_list,
            new_instruction_info(
                    name,
                    opcode,
                    min_operands,
                    max_operands,
                    legal_modes
            )
    );
    return 0;
}


int new_instruction_info_table() {
    LegalModes mov_modes = {{Immediate, Direct, Index,          RegisterDirect, -1},
                            {Direct,    Index,  RegisterDirect, -1},
                            {-1}};
    LegalModes cmp_modes = {{Immediate, Direct, Index, RegisterDirect, -1},
                            {Immediate, Direct, Index, RegisterDirect, -1},
                            {-1}};
    LegalModes add_modes = {{Immediate, Direct, Index,          RegisterDirect, -1},
                            {Direct,    Index,  RegisterDirect, -1},
                            {-1}};
    LegalModes sub_modes = {{Immediate, Direct, Index,          RegisterDirect, -1},
                            {Direct,    Index,  RegisterDirect, -1},
                            {-1}};
    LegalModes lea_modes = {{Direct, Index},
                            {Direct, Index, RegisterDirect, -1},
                            {-1}};
    LegalModes clr_modes = {
            {Direct, Index, RegisterDirect, -1},
            {-1}};
    LegalModes not_modes = {
            {Direct, Index, RegisterDirect, -1},
            {-1}};
    LegalModes inc_modes = {
            {Direct, Index, RegisterDirect, -1},
            {-1}};
    LegalModes dec_modes = {
            {Direct, Index, RegisterDirect, -1},
            {-1}};
    LegalModes jmp_modes = {
            {Direct, Index, -1},
            {-1}};
    LegalModes bne_modes = {
            {Direct, Index, -1},
            {-1}};
    LegalModes jsr_modes = {
            {Direct, Index, -1},
            {-1}};
    LegalModes get_modes = {
            {Direct, Index, RegisterDirect, -1},
            {-1}};
    LegalModes prn_modes = {
            {Immediate, Direct, Index, RegisterDirect, -1},
            {-1}};
    LegalModes legal_modes = {{-1}};
    instruction_info_list = new_list();
    add_legal_instruction("hlt", 15, 0, 0, legal_modes);
    add_legal_instruction("rts", 14, 0, 0, legal_modes);
    add_legal_instruction("mov", 0, 2, 2, mov_modes);
    add_legal_instruction("add", 2, 2, 2, add_modes);
    add_legal_instruction("cmp", 1, 2, 2, cmp_modes);
    add_legal_instruction("lea", 6, 2, 2, lea_modes);
    add_legal_instruction("sub", 3, 2, 2, sub_modes);
    add_legal_instruction("not", 4, 1, 1, not_modes);
    add_legal_instruction("clr", 5, 1, 1, clr_modes);
    add_legal_instruction("dec", 8, 1, 1, dec_modes);
    add_legal_instruction("inc", 7, 1, 1, inc_modes);
    add_legal_instruction("bne", 10, 1, 1, bne_modes);
    add_legal_instruction("jmp", 9, 1, 1, jmp_modes);
    add_legal_instruction("get", 11, 1, 1, get_modes);
    add_legal_instruction("prn", 12, 1, 1, prn_modes);
    add_legal_instruction("jsr", 13, 1, 1, jsr_modes);
    return 0;
}

int get_instruction_info_record(char *instruction_name, InstructionInfo **res) {
    LIST_ITER_TILL_END(instruction_info_list, InstructionInfo*, {
        if (compare_strings(data->name, instruction_name)) {
            *res = data;
            return 0;
        }
    })
    return 1;
}


bool is_legal_instruction(char *instruction_name) {
    bool res = false;
    LIST_ITER_TILL_END(instruction_info_list, InstructionInfo*, {
        if (compare_strings(data->name, instruction_name)) {
            res = true;
        }
    })

    return res;
}