/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "code_reader.h"
#include "string_utils.h"
#include "re_utils.h"
#include "linked_list.h"

#define PART_SIZE 4096

LineOfCode *line_of_code_init(char *label, char *code, int idx) {
    int res_size = sizeof(LineOfCode);
    LineOfCode *res = malloc(res_size);
    if (label == NULL) {
        res->label = NULL;
    } else {
        res->label = new_string(label);
    }
    res->code = new_string(code);
    res->idx = idx;
    return res;
}

void line_of_code_free(LineOfCode *line_of_code) {
    if (line_of_code->label != NULL) {
        free(line_of_code->label);
    }
    free(line_of_code->code);
    free(line_of_code);
}


int line_of_code_to_string(LineOfCode *line, char *dest) {
    if (line->label != NULL) {
        sprintf(dest, "%s: %s", line->label, line->code);
    } else {
        strcpy(dest, line->code);
    }
    return 0;
}


int read_file_to_end(FILE *fd, char **input_res, int *total_characters) {
    char currentchar;
    int totalchars = 0;
    int totalparts = 1;
    char *input = malloc(PART_SIZE);

    while ((currentchar = fgetc(fd)) != EOF) {
        totalchars++;
        input[totalchars - 1] = currentchar;
        if (totalchars % PART_SIZE <= 0) {
            totalparts++;
            input = realloc(input, totalparts * totalparts);
            if (input == NULL) {
                return 1;
            }
        }
    }
    input[totalchars] = 0;
    *total_characters = totalchars;
    *input_res = input;
    return 0;
}

bool line_contains_label(char *line) {
    return re_match("^[a-zA-Z0-9]*:.*", line);
}

int read_line_with_label(char *line, int idx, ListNode *lines) {
    ListNode *splitline_res;
    LineOfCode *line_of_code;

    split(line, ":", &splitline_res, 1);
    if (list_len(splitline_res) < 2) {
        printf("Line %d: Line is labeled but contains no content\n", idx + 1);
        return 1;
    }

    strip(splitline_res->next->data, splitline_res->next->data);
    line_of_code = line_of_code_init(splitline_res->data, splitline_res->next->data, idx);
    list_append(lines, line_of_code);

    free_list(splitline_res, free);
    return 0;
}

int _read_not_labeled_line(char *line, int idx, ListNode *lines) {
    LineOfCode *line_of_code = line_of_code_init(NULL, line, idx);
    list_append(lines, line_of_code);
    return 0;
}

int read_lines_of_code(char *file_path, ListNode **res) {
    int totalchars;
    char *content;
    char line_string[1000];
    FILE *descriptor;
    int idx = 0;

    ListNode *lines = new_list();

    if ((descriptor = fopen(file_path, "r")) == NULL) {
        return 1;
    }

    read_file_to_end(descriptor, &content, &totalchars);

    for (; get_line_from_string(content, idx, line_string) == 0; idx++) {
        strip(line_string, line_string);
        if (strlen(line_string) == 0 || startswith(line_string, ";")) {
            continue;
        }

        if (line_contains_label(line_string)) {
            read_line_with_label(line_string, idx, lines);
        } else {
            _read_not_labeled_line(line_string, idx, lines);
        }
    }

    *res = lines;
    return 0;
}
