; file ps.as
.extern L3
.extern W

    .entry LENGTH
    
MAIN:   mov @r3 ,LENGTH
LOOP: 	        jmp      L1
  
;this is comment
    prn -5


bne W
sub @r1, @r4
bne L3
L1: inc K
.entry LOOP
jmp LOOP
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
