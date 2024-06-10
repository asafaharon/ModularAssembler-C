/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include "linked_list.h"
#include "symbol_table.h"
#include "string_utils.h"


int get_symbol(ListNode* symbol_table, char* label, Symbol **res) {
    LIST_ITER_TILL_END(symbol_table, Symbol*, {
        if (compare_strings(label, data->label)) {
            *res = data;
            return 0;
        }
    })
    return 1;
}