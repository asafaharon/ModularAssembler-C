/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include "int_utils.h"

int *int_init(int num) {
    int *pointer = malloc(sizeof(int));
    *pointer = num;
    return pointer;
}

int unset_left_bits(unsigned int num, unsigned int bits_count) {
    return (num << bits_count) >> bits_count;
}
