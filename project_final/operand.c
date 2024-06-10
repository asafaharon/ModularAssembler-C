/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linked_list.h"
#include "operand.h"
#include "utils.h"
#include "constants.h"
#include "symbol_table.h"


Operand *new_operand(AddrMode mode, int value, Symbol *symbol, int idx) {
    Operand *res = malloc(sizeof(Operand));
    res->mode = mode;
    res->value = value;
    res->symbol = symbol;
    res->idx = idx;
    return res;
}

int get_register_idx(char *operand_string) {
    char register_idx[128];
    int operand_length = strlen(operand_string);
    slice(operand_string, 1, operand_length + 1, register_idx);
    return atoi(register_idx);
}

int operand_addr_mode(char *operand_string, AddrMode *res) {
    if (re_match("#[\\+\\-]?[0-9]*", operand_string)) {
        *res = Immediate;
    } else if (re_match("^.*\\..*$", operand_string)) {
        *res = Index;
    } else if (re_match("^r[0-9]{0,2}", operand_string) && get_register_idx(operand_string) < REGISTERS_COUNT) {
        *res = RegisterDirect;
    } else {
        *res = Direct;
    }

    return 0;
}

int parse_immediate_operand(char *operand_string, Operand **res) {
    int operand_length = strlen(operand_string);
    int value;
    slice(operand_string, 1, operand_length + 1, operand_string);
    value = atoi(operand_string);
    *res = new_operand(Immediate, value, NULL, 0);
    return 0;
}

int parse_direct_operand(char *operand_string, ListNode *symbol_table, Operand **res, char *error_msg) {
    Symbol *symbol;
    if (get_symbol(symbol_table, operand_string, &symbol) != 0) {
        sprintf(error_msg, "'%s' symbol could not be found.", operand_string);
        return 1;
    }
    *res = new_operand(Direct, symbol->value, symbol, 0);
    return 0;
}

int parse_reg_operand(char *operand_string, Operand **res) {
    *res = new_operand(RegisterDirect, get_register_idx(operand_string), NULL, 0);
    return 0;
}

int parse_index_operand(char *operand_string, ListNode *symbol_table, Operand **res, char *error_msg) {
    ListNode *split_res;
    char *raw_symbol;
    char *raw_idx;
    Symbol *symbol;
    split(operand_string, ".", &split_res, -1);
    raw_symbol = split_res->data;
    if (get_symbol(symbol_table, raw_symbol, &symbol) != 0) {
        sprintf(error_msg, "'%s' symbol could not be found.", raw_symbol);
        return 1;
    }
    raw_idx = split_res->next->data;
    *res = new_operand(Index, symbol->value, symbol, atoi(raw_idx));
    free_list(split_res, free);
    return 0;
}

int parse_operand_string(char *operand_string, ListNode *symbol_table, Operand **res, char *error_msg) {
    AddrMode mode;
    operand_addr_mode(operand_string, &mode);
    switch (mode) {
        case Immediate:
            parse_immediate_operand(operand_string, res);
            break;
        case Direct:
            if (parse_direct_operand(operand_string, symbol_table, res, error_msg) != 0) {
                return 1;
            }
            break;
        case RegisterDirect:
            parse_reg_operand(operand_string, res);
            break;
        case Index:
            if (parse_index_operand(operand_string, symbol_table, res, error_msg) != 0) {
                return 1;
            }
            break;
    }
    return 0;
}

int extra_operand_binary_words(char *operand_string) {
    AddrMode mode;
    operand_addr_mode(operand_string, &mode);
    return get_extra_binary_words_from_mode(mode);
}


int get_extra_binary_words_from_mode(AddrMode mode) {
    switch (mode) {
        case Immediate:
            return 1;
        case Direct:
            return 1;
        case RegisterDirect:
            return 1;
        case Index:
            return 2;
    }
    return 0;
}

int operands_extra_binary_words(char *operand_strings) {
    ListNode *operand_strings_list;
    int operands_extra_words = 0;
    AddrMode mode;
    bool register_operands_only = true;
    split_commas(operand_strings, &operand_strings_list);
    LIST_ITER_TILL_END(operand_strings_list, char*, {
        strip(data, data);
    })
    LIST_ITER_TILL_END(operand_strings_list, char*, {
        operand_addr_mode(data, &mode);
        if (mode != RegisterDirect) {
            register_operands_only = false;
        }
    })
    if (register_operands_only) {
        operands_extra_words = 1;
    } else {
        LIST_ITER_TILL_END(operand_strings_list, char*, {
            operands_extra_words += extra_operand_binary_words(data);
        })
    }
    free_list(operand_strings_list, free);
    return operands_extra_words;
}