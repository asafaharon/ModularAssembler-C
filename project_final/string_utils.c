/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string_utils.h"


char *new_string(char *string) {
    int string_length = strlen(string);
    char *res = malloc(string_length + 1);
    return strcpy(res, string);
}

int slice(char *string, int start, int end, char *res) {
    int string_length = strlen(string);
    if (end == -1) {
        end = string_length;
    }
    if (string_length != 0 && (start < 0 || start >= string_length || end < start || end > string_length + 1)) {
        return 1;
    }
    strncpy(res, string + start, end - start);
    res[end - start] = 0;
    return 0;
}


char *get_first_non_separator_character(char *string, char *separator) {
    int separator_length = strlen(separator);
    while (string == strstr(string, separator)) {
        string += separator_length;
    }
    return string;
}

char *get_last_separator(char *string, char *separator) {
    int separator_length = strlen(separator);
    char *position = string + strlen(string) - 1;
    while (strstr(position - separator_length, separator) == position && position - separator_length >= position) {
        position -= separator_length;
    }
    return position + 1;
}

int split(char *string, char *separator, ListNode **res, int max_splits) {
    char split_item[4096];
    int splits_count = 0;
    char *position;
    ListNode *split_items = new_list();
    do {
        splits_count++;
        string = get_first_non_separator_character(string, separator);
        position = strstr(string, separator);
        if (position == NULL) {
            strcpy(split_item, string);
        } else {
            slice(string, 0, position - string, split_item);
        }
        list_append(split_items, new_string(split_item));
        string = position;
    } while ((max_splits == -1 || splits_count < max_splits) && position != NULL && strlen(string) > 0);
    if (splits_count == max_splits && string != NULL) {
        string = get_first_non_separator_character(string, separator);
        position = get_last_separator(string, separator);
        if (string != position) {
            slice(string, 0, position - string, split_item);
            list_append(split_items, new_string(split_item));
        }
    }
    *res = split_items;
    return 0;
}

int split_whitespaces(char *string, ListNode **res, int max_splits) {
    ListNode *split_res;
    int return_code;
    replace_whitespace_with_space(string);
    return_code = split(string, " ", &split_res, max_splits);
    *res = split_res;
    return return_code;
}

int split_commas(char *string, ListNode **res) {
    ListNode *split_res;
    ListNode *stripped_res = new_list();
    split(string, ",", &split_res, -1);
    LIST_ITER_TILL_END(split_res, char*, {
        strip(data, data);
        list_append(stripped_res, new_string(data));
    })
    *res = stripped_res;
    free_list(split_res, free);
    return 0;
}

bool startswith(char *string, char *substring) {
    return strstr(string, substring) == string;
}

bool compare_strings(char *string1, char *string2) {
    return strcmp(string1, string2) == 0;
}

int strip(char *string, char *res) {
    int start_idx;
    int end_idx;
    for (start_idx = 0; isspace(string[start_idx]); start_idx++) {}
    for (end_idx = strlen(string) - 1; isspace(string[end_idx]); end_idx--) {}
    return slice(string, start_idx, end_idx + 1, res);
}

int replace_whitespace_with_space(char *string) {
    int string_length = strlen(string);
    int i;
    for (i = 0; i < string_length; i++) {
        if (isspace(string[i])) {
            string[i] = ' ';
        }
    }
    return 0;
}


int reverse_string(char *string, char *res) {
    int i;
    int string_length = strlen(string);
    char *temp_string = malloc(string_length + 1);
    for (i = 0; i < strlen(string); i++) {
        temp_string[i] = string[string_length - i - 1];
    }
    temp_string[string_length] = 0;
    strcpy(res, temp_string);
    free(temp_string);
    return 0;
}

int get_line_from_string(char *string, int index, char *result) {
    char *original_string = string;
    unsigned int string_length = strlen(string);
    char *next_line = strstr(string, "\n");
    bool is_last_line;
    int i;
    for (i = 0; i < index && next_line != NULL; i++) {
        string = next_line + 1;
        next_line = strstr(string, "\n");
    }
    if (i != index) {
        return 1;
    }
    is_last_line = next_line == NULL && (string - original_string) <= string_length;
    if (is_last_line) {
        strcpy(result, string);
    } else {
        slice(string, 0, next_line - string, result);
    }
    return 0;
}