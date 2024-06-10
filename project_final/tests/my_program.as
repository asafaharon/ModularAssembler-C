.entry LOOP
    .entry LENGTH
    .extern L3
    .extern W

macro m1
    inc r2
    mov #10, r2
endmacro

MAIN:   mov S1.1, W
add r2,STR
LOOP:   jmp W
prn #-5
sub r1, r4
inc K
m1
mov S1.2, r3
bne L3
END:    hlt
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
K:      .data 22
S1:     .struct 8,"ab"
