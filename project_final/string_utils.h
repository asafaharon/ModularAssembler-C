/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdbool.h>
#include "linked_list.h"


/**
 * Create a new string instance on the heap.
 */
char *new_string(char *string);


/**
 * slice a string according to the given indicators (similar to string slicing in python)
 */
int slice(char *string, int start, int end, char *res);

/**
 * split a string by a specific separator.
 */
int split(char *string, char *separator, ListNode **res, int max_splits);

int split_whitespaces(char *string, ListNode **res, int max_splits);

/**
 * Split a string by commas, for example: "a,b,c,d" -> ["a", "b", "c", "d"]
 */
int split_commas(char *string, ListNode **res);


bool startswith(char *string, char *substring);

bool compare_strings(char *string1, char *string2);

/**
 * Delete all the whitespaces from the beginning and the end of the string.
 */
int strip(char *string, char *res);

int replace_whitespace_with_space(char *string);

int get_line_from_string(char *string, int index, char *result);

/**
 * Reverse a string
 */
int reverse_string(char *string, char *res);