/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#pragma once


/**
* Create an int instance on the heap (by using malloc).
 */
int *int_init(int num);

/**
 * Unset the left given bits
 */
int unset_left_bits(unsigned int num, unsigned int bits_count);