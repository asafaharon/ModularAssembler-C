/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

ListNode *new_list() {
    ListNode *node = malloc(sizeof(ListNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

ListNode *get_first_element(ListNode *node) {
    ListNode *position = node;
    while (position->prev != NULL) {
        position = position->prev;
    }
    return position;
}

ListNode *get_last_element(ListNode *node) {
    ListNode *position = node;
    while (position->next != NULL) {
        position = position->next;
    }
    return position;
}

int list_len(ListNode *node) {
    int length;
    ListNode *head = get_first_element(node);
    if (head->data == NULL && head->prev == NULL && head->next == NULL) {
        return 0;
    }
    length = 1;
    while (head->next != NULL) {
        length++;
        head = head->next;
    }
    return length;
}

bool contains_zero_elements(ListNode *node) {
    return list_len(node) == 0;
}

int free_single_node(ListNode *node, void item_free_callback(void *)) {
    item_free_callback(node->data);
    free(node);
    return 0;
}

int list_append(ListNode *node, void *data) {
    ListNode *tail;
    ListNode *new_node;
    if (contains_zero_elements(node)) {
        node->data = data;
        return 0;
    }
    tail = get_last_element(node);
    new_node = new_list();
    new_node->data = data;
    tail->next = new_node;
    return 0;
}

int list_concat(ListNode *node1, ListNode *node2) {
    get_last_element(node1)->next = node2;
    return 0;
}

bool string_in_list(ListNode *node, char *string) {
    while (node != NULL && !contains_zero_elements(node)) {
        if (strcmp(node->data, string) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

int free_list(ListNode *node, void free_func(void *)) {
    ListNode *position = get_first_element(node);
    ListNode *next_node;
    while (position->next != NULL) {
        next_node = position->next;
        free_single_node(position, free_func);
        position = next_node;
    }
    return 0;
}