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


enum SymbolAttr {
    Code, Entry, External, Data
} typedef SymbolAttr;


struct Symbol {
    int value;
    char *label;
    ListNode *usage_addresses;
    ListNode *attributes;
} typedef Symbol;


/**
 * Create a symbol instance on the heap
 */
Symbol *create_symbol(char *label, int address, ListNode *attributes);

bool is_external(Symbol symbol);

bool is_entry(Symbol symbol);