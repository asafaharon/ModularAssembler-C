/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <string.h>
#include <stdio.h>
#include "linked_list.h"
#include "code_reader.h"
#include "instruction.h"
#include "instruction_info.h"
#include "utils.h"
#include "symbols.h"
#include "constants.h"
#include "assembler.h"
#include "linked_list.h"
#include "operand.h"

int calc_data_instruction_binary_words(char *instruction) {
    int total_words;
    ListNode *split_res;
    ListNode *operands_split_res;

    replace_whitespace_with_space(instruction);
    split(instruction, " ", &split_res, 1);

    if (list_len(split_res) != 2) {
        printf("ERROR - Wrong usage of '.data' token.\n");
        exit(1);
    }

    split_commas(split_res->next->data, &operands_split_res);

    total_words = list_len(operands_split_res);
    free_list(split_res, free);
    free_list(operands_split_res, free);

    return total_words;
}

int calc_string_instruction_binary_words(char *instruction) {
    ListNode *split_res;
    char *string_value;
    int res;
    replace_whitespace_with_space(instruction);
    split(instruction, " ", &split_res, 1);
    if (list_len(split_res) != 2 || !re_match("^\\s*\".*\"\\s*$", split_res->next->data)) {
        printf("ERROR - Wrong usage in '.string' token.\n");
        exit(1);
    }
    string_value = split_res->next->data;
    strip(string_value, string_value);
    res = strlen(string_value) - 1;

    free_list(split_res, free);
    return res;
}

int calc_struct_instruction_binary_words(char *instruction) {
    ListNode *split_res;
    ListNode *commas_split_res;
    char *string_value;
    int res;
    split_whitespaces(instruction, &split_res, 1);
    split_commas(split_res->next->data, &commas_split_res);
    if (list_len(split_res) != 2 ||
        !re_match("^\\s*[\\+\\-]*[0-9]*,\\s*\".*\"\\s*$", split_res->next->data)) {
        printf("ERROR - Wrong usage in '.struct' token.\n");
        exit(1);
    }
    string_value = commas_split_res->next->data;
    strip(string_value, string_value);
    res = strlen(string_value);
    free_list(split_res, free);
    free_list(commas_split_res, free);
    return res;
}

int create_prefab_labels(ListNode *lines_of_code, char *prefix, ListNode **res_labels) {
    char *code;
    char *label;
    ListNode *entry_labels = new_list();
    LIST_ITER_TILL_END(lines_of_code, LineOfCode *, {
        code = data->code;
        if (startswith(code, prefix)) {
            replace_whitespace_with_space(code);
            code = strstr(code, " ");
            label = new_string(code);
            strip(code, label);
            list_append(entry_labels, label);
        }
    })
    *res_labels = entry_labels;
    return 0;
}

int create_entries(ListNode *lines_of_code, ListNode **res_list) {
    return create_prefab_labels(lines_of_code, ".entry", res_list);
}

int create_externals(ListNode *lines_of_code, ListNode **res_list) {
    return create_prefab_labels(lines_of_code, ".extern", res_list);
}

Symbol *new_symbol(char *label, int address, SymbolAttr attribute, ListNode *entries) {
    ListNode *attributes = new_list();
    if (string_in_list(entries, label)) {
        list_append(attributes, int_init(Entry));
    }
    list_append(attributes, int_init(attribute));
    return create_symbol(
            label,
            address,
            attributes
    );
}

int create_externals_symbols(ListNode *lines_of_code, ListNode *symbol_table) {
    ListNode *externals;
    create_externals(lines_of_code, &externals);
    LIST_ITER_TILL_END(externals, char*, {
        ListNode *attributes = new_list();
        list_append(attributes, int_init(External));
        list_append(symbol_table, create_symbol(new_string(data), 0, attributes));
    })
    free_list(externals, free);
    return 0;
}

int build_symbol_table(ListNode *lines_of_code, ListNode **res_symbol_table) {
    int data_counter = 0;
    int instruction_counter = INSTRUCTION_COUNTER_IV;
    char *code;
    char *label;
    Symbol *symbol;
    ListNode *entries;
    ListNode *symbol_table = new_list();
    create_externals_symbols(lines_of_code, symbol_table);
    create_entries(lines_of_code, &entries);
    LIST_ITER_TILL_END(lines_of_code, LineOfCode*, {
        if (!startswith(data->code, ".")) {
            if (data->label != NULL) {
                symbol = new_symbol(data->label, instruction_counter, Code, entries);
                list_append(symbol_table, symbol);
            }
            instruction_counter += count_instruction_words(data->code);
        }
    })
    LIST_ITER_TILL_END(lines_of_code, LineOfCode*, {
        code = data->code;
        label = data->label;

        if (startswith(code, ".data")) {
            if (label != NULL) {
                symbol = new_symbol(label, instruction_counter + data_counter, Data, entries);
                list_append(symbol_table, symbol);
            }
            data_counter += calc_data_instruction_binary_words(code);
        } else if (startswith(code, ".string")) {
            if (label != NULL) {
                symbol = new_symbol(label, instruction_counter + data_counter, Data, entries);
                list_append(symbol_table, symbol);
            }

            data_counter += calc_string_instruction_binary_words(code);
        } else if (startswith(code, ".struct")) {
            if (label != NULL) {
                symbol = new_symbol(label, instruction_counter + data_counter, Data, entries);
                list_append(symbol_table, symbol);
            }
            data_counter += calc_struct_instruction_binary_words(code);
        }
    })
    *res_symbol_table = symbol_table;
    return 0;
}


/**
 * Encode a string to binary words.
 */
int convert_string_to_binary_words(char *raw_string, ListNode *binary_words) {
    int i;
    int binary_word;
    if (!re_match("^\\s*\".*\"\\s*$", raw_string)) {
        printf("ERROR - Invalid string\n");
        exit(1);
    }
    slice(raw_string, 1, strlen(raw_string) - 1, raw_string);
    for (i = 0; i < strlen(raw_string); i++) {
        binary_word = (int) raw_string[i];
        list_append(binary_words, int_init(binary_word));
    }
    list_append(binary_words, int_init(0));
    return 1;
}

int convert_string_instruction(char *instruction, ListNode *binary_words) {
    ListNode *split_res;
    replace_whitespace_with_space(instruction);
    split(instruction, " ", &split_res, 1);
    if (list_len(split_res) != 2) {
        printf("ERROR - Wrong usage of '.string' token.\n");
        exit(1);
    }
    convert_string_to_binary_words(split_res->next->data, binary_words);
    free_list(split_res, free);
    return 0;
}

int convert_data_instruction(char *instruction, ListNode *binary_words) {

    int binary_word;
    ListNode *split_res;
    ListNode *operands_split_res;
    replace_whitespace_with_space(instruction);
    split(instruction, " ", &split_res, 1);
    if (list_len(split_res) != 2) {
        printf("ERROR - Wrong usage of '.data' token.\n");
        exit(1);
    }
    split_commas(split_res->next->data, &operands_split_res);
    LIST_ITER_TILL_END(operands_split_res, char*, {
        binary_word = atoi(data);
        list_append(binary_words, int_init(binary_word));
    })
    free_list(split_res, free);
    free_list(operands_split_res, free);
    return 0;
}

int convert_struct_instruction(char *instruction, ListNode *binary_words) {
    char *raw_number;
    char *raw_string;
    ListNode *split_res;
    ListNode *operands_split_res;
    split_whitespaces(instruction, &split_res, 1);
    if (list_len(split_res) != 2) {
        printf("ERROR -  Wrong usage of '.struct' token.\n");
        exit(1);
    }
    split_commas(split_res->next->data, &operands_split_res);
    if (list_len(operands_split_res) != 2) {
        printf("ERROR - .struct can have 2 operands. 1st should be an integer and the 2nd must "
                  "be a string.\n");
        exit(1);
    }
    raw_number = operands_split_res->data;
    raw_string = operands_split_res->next->data;
    strip(raw_number, raw_number);
    strip(raw_string, raw_string);

    list_append(binary_words, int_init(atoi(raw_number)));
    convert_string_to_binary_words(raw_string, binary_words);

    free_list(split_res, free);
    free_list(operands_split_res, free);
    return 0;
}

int save_external_symbol_usage(ListNode *operands, int instruction_counter) {
    int usage_address = instruction_counter + 1;
    LIST_ITER_TILL_END(operands, Operand *, {
        if (data->symbol != NULL && is_external(*data->symbol)) {
            list_append(data->symbol->usage_addresses, int_init(usage_address));
        }
        usage_address += get_extra_binary_words_from_mode(data->mode);
    })
    return 0;
}

int convert_ordinary_instruction(LineOfCode *line_of_code, int instruction_counter, ListNode *symbol_table,
                                 ListNode *binary_instructions, char *error_msg) {
    Instruction instruction;
    ListNode *encode_res;
    if (parse_instruction(
            line_of_code->code,
            symbol_table,
            instruction_counter,
            &instruction,
            error_msg) != 0) {
        return 1;
    }
    save_external_symbol_usage(instruction.operands, instruction_counter);
    instruction_to_words(instruction, &encode_res);
    LIST_ITER_TILL_END(encode_res, int*, {
        list_append(binary_instructions, int_init(*data));
    })
    free_list(encode_res, free);
    return 0;
}

int assemble(
        ListNode *lines_of_code,
        ListNode *symbol_table,
        ListNode **res_words,
        ListNode **res_data_words
) {
    LineOfCode *line_of_code;
    int return_code = 0;
    char error_msg[4096];
    int instruction_counter = INSTRUCTION_COUNTER_IV;
    char extended_error_msg[4096];
    ListNode *binary_data_words = new_list();
    ListNode *binary_words = new_list();
    LIST_ITER_TILL_END(lines_of_code, LineOfCode*, {
        line_of_code = data;
        if (startswith(line_of_code->code, ".string")) {
            convert_string_instruction(line_of_code->code, binary_data_words);
        } else if (startswith(line_of_code->code, ".data")) {
            convert_data_instruction(line_of_code->code, binary_data_words);
        } else if (startswith(line_of_code->code, ".struct")) {
            convert_struct_instruction(line_of_code->code, binary_data_words);
        } else if (!startswith(line_of_code->code, ".")) {
            if (convert_ordinary_instruction(line_of_code, instruction_counter, symbol_table, binary_words,
                                             error_msg) != 0) {
                return_code = 1;
                printf(extended_error_msg, "Line %d: %s\n", line_of_code->idx + 1, error_msg);
            } else {
                instruction_counter += count_instruction_words(line_of_code->code);
            }
        }
    })
    *res_words = binary_words;
    *res_data_words = binary_data_words;
    return return_code;
}
