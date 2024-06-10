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
 * General constant values
 */
#define SRC_REG_OP_IDX 6
#define DEST_REG_OP_IDX 2
#define ADDRESS_TYPE_BITS_COUNT 2
#define REGISTERS_COUNT 8

#define INSTRUCTION_COUNTER_IV 100
#define WORDS_COUNT_FOR_INSTRUCTION 1

/*
 * Consts used for instruction encode process
 */
#define OPCODE_INDEX 6
#define SOURCE_OPERAND_ADDRESSING_MODE_INDEX 4
#define DEST_OPERAND_ADDRESSING_MODE_INDEX 2
#define ADDRESS_TYPE_INDEX 0


/*
 * Consts related to base32
 */
#define BITS_IN_BASE32 5
#define BASE32_CHARACTERS_IN_INT 10
