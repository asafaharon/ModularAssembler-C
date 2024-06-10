/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#pragma once


/*
 * A trick to use a variable inside a macro.
 */
# define MACRO_VAR(name) __FILE__##__LINE__##name