CC=gcc
FLAGS= -Wall -pedantic -g -ansi -m32 -o build/assembler

# In order to compile the assembler, type 'make assembler' in the terminal :)
assembler:
	$(CC) linked_list.c *utils*.c symbol.c symbol_table.c operand.c instruction_info.c instruction.c code_reader.c assembler*.c file_writers.c preprocessor.c main.c $(FLAGS)

clean:
	-rm build/* $(objects)
