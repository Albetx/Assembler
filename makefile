CC = gcc 
CFLAGS = -Wall -ansi -std=c90
MFLAG = -lm -pedantic

default: main

main: assembler.o assemblerAssist.o instructionCompile.o commandParser.o FileInteraction.o MemStructsHandle.o Output.o SymTable.o
	$(CC) $(CFLAGS) -o assembler assembler.o assemblerAssist.o instructionCompile.o commandParser.o FileInteraction.o MemStructsHandle.o Output.o SymTable.o $(MFLAG)
assembler.o: assembler.c assembler.h commandParser.h assemblerAssist.h BinaryStructsAndDefMacros.h Output.h FileInteraction.h MemStructsHandle.h instructionCompile.h
	$(CC) $(CFLAGS) -c assembler.c $(MFLAG)
assemblerAssist.o: assemblerAssist.c assemblerAssist.h
	$(CC) $(CFLAGS) -c assemblerAssist.c $(MFLAG)
BasicFunctions.o: instructionCompile.c instructionCompile.h BinaryStructsAndDefMacros.h SymTable.h MemStructsHandle.h commandParser.h
	$(CC) $(CFLAGS) -c instructionCompile.c $(MFLAG)
commandParser.o: commandParser.c commandParser.h BinaryStructsAndDefMacros.h instructionCompile.h
	$(CC) $(CFLAGS) -c commandParser.c $(MFLAG)
FileInteraction.o: FileInteraction.c FileInteraction.h BinaryStructsAndDefMacros.h
	$(CC) $(CFLAGS) -c FileInteraction.c $(MFLAG)
MemStructsHandle.o: MemStructsHandle.c MemStructsHandle.h commandParser.h BinaryStructsAndDefMacros.h SymTable.h
	$(CC) $(CFLAGS) -c MemStructsHandle.c $(MFLAG)
Output.o: Output.c Output.h BinaryStructsAndDefMacros.h SymTable.h
	$(CC) $(CFLAGS) -c Output.c $(MFLAG)
SymTable.o: SymTable.c SymTable.h BinaryStructsAndDefMacros.h
	$(CC) $(CFLAGS) -c SymTable.c $(MFLAG)

