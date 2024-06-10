/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include "symbols.h"

#pragma once

enum AddrMode {
    Immediate = 0,
    Direct = 1,
    Index = 2,
    RegisterDirect = 3
} typedef AddrMode;


enum AddrType {
    AbsoluteAddress = 0,
    ExternalAddress = 1,
    RelocatableAddress = 2
};


struct Operand {
    AddrMode mode;
    int value;
    int idx;
    Symbol* symbol;
} typedef Operand;


/**
 * Create a new operand instance on the heap
 */
Operand *new_operand(AddrMode mode, int value, Symbol* symbol, int idx);

/**
 * Parse a single operand to an Operand object.
 */
int parse_operand_string(char *operand_string, ListNode *symbol_table, Operand **res, char *error_msg);


/**
 * Count extra binary words for the given operand string.
 */
int extra_operand_binary_words(char* operand_string);

/**
 * Get the extra words from the Addressing mode
 */
int get_extra_binary_words_from_mode(AddrMode mode);

/**
 * Get extra words required for a given operands string.
 */
int operands_extra_binary_words(char* operands_string);
