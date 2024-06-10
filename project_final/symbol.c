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
#include "symbol.h"
#include "linked_list.h"
#include "string_utils.h"


Symbol *create_symbol(char *label, int address, ListNode *attributes) {
    Symbol *res = malloc(sizeof(Symbol));
    res->label = new_string(label);
    res->value = address;
    res->attributes = attributes;
    res->usage_addresses = new_list();
    return res;
}

bool _symbol_has_attribute(Symbol symbol, SymbolAttr attribute) {
    SymbolAttr current_attribute;
    bool res = false;
    ListNode *attrs = symbol.attributes;
    LIST_ITER_TILL_END(attrs, SymbolAttr*, {
        current_attribute = *data;
        res = res || current_attribute == attribute;
    })
    return res;
}

bool is_external(Symbol symbol) {
    return _symbol_has_attribute(symbol, External);
}

bool is_entry(Symbol symbol) {
    return _symbol_has_attribute(symbol, Entry);
}