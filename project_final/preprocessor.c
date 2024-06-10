/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "preprocessor.h"
#include "utils.h"
#include "code_reader.h"
#include "linked_list.h"
#include "instruction.h"


#define MACRO_NAME_SIZE 128
#define LINE_OF_CODE_SIZE 2048


Macro *new_macro(char *name, ListNode *lines_of_code) {
    Macro *macro = malloc(sizeof(Macro));
    macro->name = new_string(name);
    macro->lines_of_code = lines_of_code;
    return macro;
}


int _get_macro_name(LineOfCode *line_of_code, char *res_name) {
    ListNode *split_res;
    char *macro_name;
    split_whitespaces(line_of_code->code, &split_res, -1);

    if (list_len(split_res) != 2) {
        printf("Line %d: Macro syntax error.\n", line_of_code->idx + 1);
        return 1;
    }

    macro_name = split_res->next->data;
    if (is_legal_instruction(macro_name)) {
        printf("Line %d: Macro cannot be named like the '%s' instruction.\n",
               line_of_code->idx + 1,
               macro_name);
        return 1;
    }

    strcpy(res_name, macro_name);
    free_list(split_res, free);
    return 0;
}

/**
 * @return
 *  Macro that matches the given name. NULL on macro not found.
 */
Macro *_get_macro_by_name(ListNode *macros, char *name) {
    Macro *macro = NULL;
    LIST_ITER_TILL_END(macros, Macro*, {
        if (compare_strings(data->name, name)) {
            macro = data;
        }
    });

    return macro;
}


int _processed_code(ListNode *lines_of_code, ListNode **res_lines) {
    char macro_name[MACRO_NAME_SIZE];
    Macro *current_macro;

    ListNode *macros = new_list();
    ListNode *macro_lines = new_list();
    ListNode *processed_lines = new_list();

    bool is_inside_macro = false;

    LIST_ITER_TILL_END(lines_of_code, LineOfCode *, {
        if (startswith(data->code, "macro")) {
            if (_get_macro_name(data, macro_name) != 0) {
                return 1;
            }
            is_inside_macro = true;
        } else if (compare_strings(data->code, "endmacro")) {
            is_inside_macro = false;
            list_append(macros, new_macro(macro_name, macro_lines));
        } else if (is_inside_macro) {
            list_append(macro_lines, data);
        } else {
            if ((current_macro = _get_macro_by_name(macros, data->code)) != NULL) {
                list_concat(processed_lines, current_macro->lines_of_code);
            } else {
                list_append(processed_lines, data);
            }
        }
    })

    *res_lines = processed_lines;
    return 0;
}

int preprocess_lines_of_code(ListNode *lines_of_code, char *dest_path) {
    FILE *descriptor;
    ListNode *processed_lines_of_code;
    char line_of_code_string[LINE_OF_CODE_SIZE];

    if ((descriptor = fopen(dest_path, "w")) == NULL) {
        printf("Could not open %s\n", dest_path);
        return 1;
    }

    if (_processed_code(lines_of_code, &processed_lines_of_code) != 0) {
        return 1;
    }

    LIST_ITER_TILL_END(processed_lines_of_code, LineOfCode*, {
        line_of_code_to_string(data, line_of_code_string);
        fprintf(descriptor, "%s\n", line_of_code_string);
    })

    fflush(descriptor);
    fclose(descriptor);

    return 0;
}