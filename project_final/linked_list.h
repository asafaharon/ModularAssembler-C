/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <stdbool.h>
#include "macros.h"

#pragma once

/**
 * two-way lineked list.
 */
struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;
} typedef ListNode;

/**
 * Create a new list object on the heap (starting with zero elements).
 */
ListNode *new_list();

/**
 * get last list element.
 */
ListNode *get_last_element(ListNode *node);

/**
 * Get the first list element.
 */
ListNode *get_first_element(ListNode *node);

/**
 * Check whether the list contains zero elements.
 */

bool contains_zero_elements(ListNode *node);

/**
 * Free this list alongside all of its elements.
 */
int free_list(ListNode *node, void free_func(void *));

/**
 * get list length
 */
int list_len(ListNode *node);


/**
 * Add an element to the list
 */
int list_append(ListNode *node, void *data);

/**
 * Check if the list contains the given string
 */
bool string_in_list(ListNode *node, char *string);

/**
 * Concatenate two lists together.
 */
int list_concat(ListNode *node1, ListNode *node2);


/**
 *  A macro that loops on every element of the list and runs the code block with the current element
 */
#define LIST_ITER_TILL_END(node, type, code) \
{                                            \
    ListNode *MACRO_VAR(node) = node;        \
    while (MACRO_VAR(node) != NULL && !contains_zero_elements(node)) \
    {                                        \
        type data = MACRO_VAR(node)->data;   \
        code                                 \
        MACRO_VAR(node) = MACRO_VAR(node)->next; \
    }                                        \
}                                            \

