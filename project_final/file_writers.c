/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "file_writers.h"
#include "linked_list.h"
#include "symbols.h"
#include "int_utils.h"
#include "constants.h"
#include "utils.h"


int convert_int_to_symbol(int word, char *res_symbol) {
    /*
     * Use swithc case to map each value to its character in the given base 32.
     */
    switch (word) {
        case 31:
            *res_symbol = 'v';
            break;
        case 30:
            *res_symbol = 'u';
            break;
        case 29:
            *res_symbol = 't';
            break;
        case 28:
            *res_symbol = 's';
            break;
        case 27:
            *res_symbol = 'r';
            break;
        case 26:
            *res_symbol = 'q';
            break;
        case 25:
            *res_symbol = 'p';
            break;
        case 24:
            *res_symbol = 'o';
            break;
        case 23:
            *res_symbol = 'n';
            break;
        case 22:
            *res_symbol = 'm';
            break;
        case 21:
            *res_symbol = 'l';
            break;
        case 20:
            *res_symbol = 'k';
            break;
        case 19:
            *res_symbol = 'j';
            break;
        case 18:
            *res_symbol = 'i';
            break;
        case 17:
            *res_symbol = 'h';
            break;
        case 16:
            *res_symbol = 'g';
            break;
        case 15:
            *res_symbol = 'f';
            break;
        case 14:
            *res_symbol = 'e';
            break;
        case 13:
            *res_symbol = 'd';
            break;
        case 12:
            *res_symbol = 'c';
            break;
        case 11:
            *res_symbol = 'b';
            break;
        case 10:
            *res_symbol = 'a';
            break;
        case 9:
            *res_symbol = '>';
            break;
        case 8:
            *res_symbol = '<';
            break;
        case 7:
            *res_symbol = '*';
            break;
        case 6:
            *res_symbol = '&';
            break;
        case 5:
            *res_symbol = '^';
            break;
        case 4:
            *res_symbol = '%';
            break;
        case 3:
            *res_symbol = '$';
            break;
        case 2:
            *res_symbol = '#';
            break;
        case 1:
            *res_symbol = '@';
            break;
        case 0:
            *res_symbol = '!';
            break;
        default:
            return 1;
    }

    return 0;
}

int convert_number_to_base32(unsigned int number, char *res, bool include_leading_zeroes) {
    char sym;
    unsigned int word;
    int j;

    for (j = 0; j < 2; j++) {
        word = unset_left_bits(number, sizeof(int) * 8 - BITS_IN_BASE32);
        if (convert_int_to_symbol(word, &sym) == 1) {
            return 1;
        }

        res[j] = sym;
        number = number >> BITS_IN_BASE32;

        if (!include_leading_zeroes && number <= 0) {
            j++;
            break;
        }
    }

    res[j] = 0;
    reverse_string(res, res);

    return 0;
}

int dump_external_symbols(Symbol *symbol, FILE *fp) {
    char usage_address[BASE32_CHARACTERS_IN_INT];
    ListNode *usage_addresses = symbol->usage_addresses;

    LIST_ITER_TILL_END(usage_addresses, int*, {
        convert_number_to_base32(*data, usage_address, true);
        fprintf(fp, "%s %s\n", symbol->label, usage_address);
    })

    return 0;
}

int dump_externals_file(ListNode *symbol_table, char *file_path) {
    FILE *descriptor = NULL;
    int idx = 0;

    LIST_ITER_TILL_END(symbol_table, Symbol*, {
        if (is_external(*data)) {
            if (idx == 0) {
                if ((descriptor = fopen(file_path, "w")) == NULL) {
                    return 1;
                }
            }
            dump_external_symbols(data, descriptor);
            idx++;
        }
    })

    if (descriptor != NULL) {
        fflush(descriptor);
        fclose(descriptor);
    }

    return 0;
}

int dump_entries_file(ListNode *symbol_table, char *file_path) {
    FILE *descriptor;
    char base32_address[BASE32_CHARACTERS_IN_INT];
    int idx = 0;

    LIST_ITER_TILL_END(symbol_table, Symbol*, {
        convert_number_to_base32(data->value, base32_address, true);
        if (is_entry(*data)) {

            if (idx == 0) {
                if ((descriptor = fopen(file_path, "w")) == NULL) {
                    return 1;
                }
            }

            fprintf(descriptor, "%s %s\n", data->label, base32_address);
            idx++;
        }
    })

    return 0;
}


int write_assembled_word(FILE *descriptor, int word, int word_idx) {
    char base32_word_idx[BASE32_CHARACTERS_IN_INT];
    char base32_word[BASE32_CHARACTERS_IN_INT];

    convert_number_to_base32(word_idx, base32_word_idx, true);
    convert_number_to_base32(word, base32_word, true);

    fprintf(
            descriptor,
            "%s %s\n",
            base32_word_idx,
            base32_word
    );

    return 0;
}

int dump_object_file(ListNode *assembled_words, ListNode *assembled_data_words, char *file_path) {
    FILE *descriptor;
    int word_idx = INSTRUCTION_COUNTER_IV;
    char base32_assembled_words_length[BASE32_CHARACTERS_IN_INT];
    char base32_assembled_data_length[BASE32_CHARACTERS_IN_INT];
    if ((descriptor = fopen(file_path, "w")) == NULL) {
        return 1;
    }
    convert_number_to_base32(list_len(assembled_words), base32_assembled_words_length, false);
    convert_number_to_base32(list_len(assembled_data_words), base32_assembled_data_length, false);
    fprintf(descriptor, "%s  %s\n", base32_assembled_words_length, base32_assembled_data_length);
    LIST_ITER_TILL_END(assembled_words, int*, {
        write_assembled_word(descriptor, *data, word_idx);
        word_idx++;
    })
    LIST_ITER_TILL_END(assembled_data_words, int*, {
        write_assembled_word(descriptor, *data, word_idx);
        word_idx++;
    })
    fflush(descriptor);
    fclose(descriptor);
    return 0;
}