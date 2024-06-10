/*
 * Maman 14
 * 13/8/2022
 * Author 1: Dima Subotin
 * ID: 311626519
 * Author 2: Asaf Aharon
 * ID: 206552044
 */

#include <stdlib.h>
#include <regex.h>
#include "re_utils.h"


bool re_match(char *pattern, char *text) {
    regex_t regex;
    int match_res;
    regcomp(&regex, pattern, REG_EXTENDED);
    match_res = regexec(&regex, text, 0, NULL, 0);
    return match_res != REG_NOMATCH;
}