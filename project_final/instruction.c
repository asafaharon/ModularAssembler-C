/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "linked_list.h"
#include "utils.h"
#include "instruction_info.h"
#include "instruction.h"
#include "operand.h"
#include "constants.h"


/**
 * Parse an operand from a string into an operand object.
 */
int parse_operands_string(char *operands_string, ListNode *symbol_table, ListNode **res, char *error_msg) {
    int parse_operand_status = 0;
    ListNode *parsed_operands = new_list();
    ListNode *operand_strings_list;
    Operand *operand;

    split_commas(operands_string, &operand_strings_list);
    LIST_ITER_TILL_END(operand_strings_list, char*, {
        parse_operand_status |= parse_operand_string(data, symbol_table, &operand, error_msg);
        list_append(parsed_operands, operand);
    })
    *res = parsed_operands;
    free_list(operand_strings_list, free);
    return parse_operand_status;
}

int count_instruction_words(char *string) {
    ListNode *split_res;
    int instruction_words = WORDS_COUNT_FOR_INSTRUCTION;
    char *operand_strings;
    split_whitespaces(string, &split_res, 1);
    if (list_len(split_res) >= 2) {
        operand_strings = split_res->next->data;
        instruction_words = WORDS_COUNT_FOR_INSTRUCTION + operands_extra_binary_words(operand_strings);
    }
    return instruction_words;
}

int instruction_operands_legal(ListNode *operands, InstructionInfo *instruction_info, char *error_msg) {
    Operand *operand;
    ListNode *legal_modes = instruction_info->legal_modes;
    ListNode *current_legal_modes;
    bool is_mode_valid = true;
    bool is_current_mode_valid;
    int operands_count = list_len(operands);
    if (operands_count < instruction_info->min_operands || operands_count > instruction_info->max_operands) {
        strcpy(error_msg, "Operands amount is illegal.");
        return 1;
    }
    while (operands != NULL && !contains_zero_elements(operands)) {
        operand = operands->data;
        current_legal_modes = legal_modes->data;
        is_current_mode_valid = false;
        while (current_legal_modes != NULL && !contains_zero_elements(current_legal_modes)) {
            is_current_mode_valid = is_current_mode_valid || *(int *) current_legal_modes->data == operand->mode;
            current_legal_modes = current_legal_modes->next;
        }
        is_mode_valid = is_mode_valid && is_current_mode_valid;
        operands = operands->next;
        legal_modes = legal_modes->next;
    }
    if (!is_mode_valid) {
        strcpy(error_msg, "Addressing mode is illegal for one (or more) operands.");
        return 1;
    }

    return 0;
}

int parse_instruction(char *string, ListNode *symbol_table, int instruction_counter, Instruction *res,
                      char *error_msg) {
    ListNode *split_res;
    char *instruction_name;
    InstructionInfo *instruction_info;
    char *operand_strings = NULL;
    ListNode *parsed_operands;
    replace_whitespace_with_space(string);
    split(string, " ", &split_res, 1);
    if (list_len(split_res) >= 2) {
        operand_strings = split_res->next->data;
    }
    instruction_name = split_res->data;
    if (get_instruction_info_record(instruction_name, &instruction_info) != 0) {
        sprintf(error_msg, "'%s' is an invalid instruction", instruction_name);
        free_list(split_res, free);
        return 1;
    }
    free_list(split_res, free);
    if (operand_strings == NULL) {
        parsed_operands = new_list();
    } else if (parse_operands_string(operand_strings, symbol_table, &parsed_operands, error_msg) != 0) {
        return 1;
    }
    if (instruction_operands_legal(parsed_operands, instruction_info, error_msg) != 0) {
        return 1;
    }
    res->operands = parsed_operands;
    res->info = instruction_info;
    res->instruction_counter = instruction_counter;
    return 0;
}

int encode_word_1(Instruction instruction, ListNode *binary_words) {
    Operand* operand1;
    Operand* operand2;
    int res_word;
    int operands_count;
    res_word = instruction.info->opcode << OPCODE_INDEX;
    operands_count = list_len(instruction.operands);
    if (operands_count == 1) {
        operand1 = (Operand*)instruction.operands->data;
        res_word |= (int)operand1->mode << DEST_OPERAND_ADDRESSING_MODE_INDEX;
    }
    if (operands_count == 2) {
        operand1 = (Operand*)instruction.operands->data;
        operand2 = (Operand*)instruction.operands->next->data;
        res_word |= (int)operand1->mode << SOURCE_OPERAND_ADDRESSING_MODE_INDEX;
        res_word |= (int)operand2->mode << DEST_OPERAND_ADDRESSING_MODE_INDEX;
    }
    res_word |= AbsoluteAddress << ADDRESS_TYPE_INDEX;
    list_append(binary_words, int_init(res_word));
    return 0;
}

int immediate_words(Operand *operand, ListNode *binary_words) {
    int binary_word = 0;
    binary_word |= operand->value << ADDRESS_TYPE_BITS_COUNT;
    binary_word |= AbsoluteAddress << ADDRESS_TYPE_INDEX;
    list_append(binary_words, int_init(binary_word));
    return 0;
}

int direct_words(Operand *operand, ListNode *binary_words) {
    int binary_word = 0;
    if (is_external(*operand->symbol)) {
        binary_word |= ExternalAddress;
        list_append(binary_words, int_init(binary_word));
    } else {
        binary_word |= RelocatableAddress << ADDRESS_TYPE_INDEX;
        binary_word |= operand->value << ADDRESS_TYPE_BITS_COUNT;
        list_append(binary_words, int_init(binary_word));
    }
    return 0;
}

int index_extra_words(Operand *operand, ListNode *binary_words) {
    int binary_idx_word = 0;
    direct_words(operand, binary_words);
    binary_idx_word |= AbsoluteAddress << ADDRESS_TYPE_INDEX;
    binary_idx_word |= operand->idx << ADDRESS_TYPE_BITS_COUNT;
    list_append(binary_words, int_init(binary_idx_word));
    return 0;
}

int encode_reg_operand(Operand* operand, bool is_source, ListNode *binary_words) {
    int binary_word = 0;
    binary_word |= AbsoluteAddress << ADDRESS_TYPE_INDEX;
    if (is_source) {
        binary_word |= operand->value << SRC_REG_OP_IDX;
    }
    else {
        binary_word |= operand->value << DEST_REG_OP_IDX;
    }
    list_append(binary_words, int_init(binary_word));
    return 0;
}

int encode_two_regs_operands(Operand* source_operand, Operand* dest_operand, ListNode *binary_words) {
    int binary_word = 0;
    binary_word |= AbsoluteAddress << ADDRESS_TYPE_INDEX;
    binary_word |= source_operand->value << SRC_REG_OP_IDX;
    binary_word |= dest_operand->value << DEST_REG_OP_IDX;
    list_append(binary_words, int_init(binary_word));
    return 0;
}

int encode_operand_extra_words(Operand *operand, ListNode *binary_words) {
    switch (operand->mode) {
        case Immediate:
            immediate_words(operand, binary_words);
            break;
        case Direct:
            direct_words(operand, binary_words);
            break;
        case Index:
            index_extra_words(operand, binary_words);
        default:
            break;
    }
    return 0;
}

bool dest_reg_operand(ListNode *operands) {
    int operands_count = list_len(operands);
    return (operands_count == 1 && ((Operand*)operands->data)->mode == RegisterDirect) ||
    (operands_count == 2 && ((Operand*)operands->next->data)->mode == RegisterDirect);
}

bool src_reg_operand(ListNode *operands) {
    int operands_count = list_len(operands);
    return (operands_count == 2 && ((Operand*)operands->data)->mode == RegisterDirect);
}

bool both_operands_are_regs(ListNode *operands) {
    return src_reg_operand(operands) && dest_reg_operand(operands);
}

int calc_operands_extra_words(Instruction instruction, ListNode *binary_words) {
    ListNode *operands = instruction.operands;
    if (both_operands_are_regs(operands)) {
        encode_two_regs_operands((Operand *) operands->data, (Operand *) operands->next->data, binary_words);
        return 0;
    }
    else if (src_reg_operand(operands)) {
        encode_reg_operand((Operand *) operands->data, true, binary_words);
    }
    LIST_ITER_TILL_END(operands, Operand*, {
        encode_operand_extra_words(data, binary_words);
    })
    if (dest_reg_operand(operands)) {
        encode_reg_operand((Operand *) operands->data, false, binary_words);
    }
    return 0;
}

int zero_operands_instruction(Instruction instruction, ListNode *binary_words) {
    return encode_word_1(instruction, binary_words);
}

int one_or_more_operand_instruction(Instruction instruction, ListNode *binary_words) {
    encode_word_1(instruction, binary_words);
    calc_operands_extra_words(instruction, binary_words);
    return 0;
}

int instruction_to_words(Instruction instruction, ListNode **res_words) {
    ListNode *binary_words = new_list();
    int operands_count = list_len(instruction.operands);
    if (operands_count == 0) {
        zero_operands_instruction(instruction, binary_words);
    } else {
        one_or_more_operand_instruction(instruction, binary_words);
    }
    *res_words = binary_words;
    return 0;
}