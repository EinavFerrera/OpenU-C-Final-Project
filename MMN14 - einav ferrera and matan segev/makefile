.DEFAULT_GOAL := all

assembler: main.o preAssembler.o structures.o firstPass.o registerTable.o binaryBuild.o secPass.o
	gcc *.c -Wall -ansi -pedantic -o assembler -lm
main.o: main.c preAssembler.h
	gcc main.c -Wall -ansi -pedantic -c
preAssembler.o: preAssembler.c preAssembler.h firstPass.h errors.h structures.h types.h
	gcc preAssembler.c -Wall -ansi -pedantic -c
structures.o: structures.c structures.h types.h
	gcc structures.c -Wall -ansi -pedantic -c
firstPass.o: firstPass.c firstPass.h structures.h registerTable.h secondPass.h types.h
	gcc firstPass.c -Wall -ansi -pedantic -c
secPass.o: secondPass.c secondPass.h structures.h binaryBuild.h
	gcc secondPass.c -Wall -ansi -pedantic -c
registerTable.o: registerTable.c registerTable.h
	gcc registerTable.c -Wall -ansi -pedantic -c
binaryBuild.o: binaryBuild.c binaryBuild.h structures.h firstPass.h
	gcc binaryBuild.c -Wall -ansi -pedantic -c


clean:
	rm -f *.o

.PHONY: all
all: assembler clean
