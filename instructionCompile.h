#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "commandParser.h"
#include "BinaryStructsAndDefMacros.h"
#include "SymTable.h"
#include "MemStructsHandle.h"

#define	NUM_OF_COMMANDS		16
#define	ILLEGAL_ADDRESSING     -1
#define	IMMEDIATE_ADDRESSING	0
#define	DIRECT_ADDRESSING	1
#define CONSTANT_ADDRESSING	2
#define	REGISTER_ADDRESSING     3
#define SOURCE                  1
#define DESTENETION             2
#define ABSOLUTE                0
#define RELOCATABLE             2
#define EXTERNAL                1

/*	Gets instruction number for a given instruction.
	Returns the instruction number or -1 if no matching instruction found.
*/
int	instructionIndex(const char *StringToCheck);


/*
	Returns the operand addressing method (CONS/IMM/DIR/REG) or -1 if no addressing method found.
*/
int	AddressNumber(const char *StringToCheck);


/*
	sInstruction 	- instruction to work on.
	Returns  the size of instruction if function ran successfully, and -1 otherwise
*/
int	instructionSize(char *LineString, int PointerInLine, char *sInstruction);


/*	increase IC 	- depends on current command.
	sInstruction 	- instruction to work on.
	IC 		- instruction counter.
	sFileFullName 	- full file name for error printing.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int	Increase_IC(char *LineString, int *PointerInLine, char *sInstruction, int *IC, char *sFileFullName,int numberOfLine);


/*	translates the  instruction and inserts it to the code segment
	LineString 			- line to check.
	sInstruction 			- instruction to work on.
	IC 				- instruction counter.
	codeSegment 			- code segment.
	symTable 			- symbol table to check whether the labels in instruction's operands, if there any, exist.
	stExternalSymbolTable 		- external symbol table, to set external symbol if there is reference to it.
	sFileFullName 			- full file name for error printing.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int	TranslateInstruction(char *LineString, int *PointerInLine, char *sInstruction, int *IC, int codeSegmentLength, int *dataSegment,
        codeSegment (*codeSegment),SymbolTable symTable, SymbolTable *stExternalSymbolTable, char *sFileFullName,int numberOfLine) ;


/*	Translates extra word of the operand and inserts it to the code segment.
	nOpAddMeth 			- address method of operand.
	IC 				- instruction to work on.
	nInstructionIndex 		- instruction index in instruction array to check if addressing method is legal.
	dataSegment 			- data segment
	codeSegment 			- code segment
	symTable 			- symbol table to check whether the labels in instruction's operands, if there any, exist.
	stExternalSymbolTable 	- external symbol table, to set external symbol if there is reference to it.
	sFileFullName 	- full file name for error printing.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int	TranslateExtraWord(char *operand, int nOpAddMeth, int *IC, int codeSegmentLength, int nInstructionIndex, int* dataSegment, codeSegment (*codeSegment),
				  SymbolTable symTable, SymbolTable *stExternalSymbolTable, char *sFileFullName, int numberOfLine,int operandType) ;


/*	Translates extra word of the operand (in case it concludes a label) and inserts it to the code segment.
	operand. 		-operand string.
	nOpAddMeth          	-address method of operand.
	symTable 		- symbol table to check whether the label exists.
	stExternalSymbolTable 	- external symbol table, to set external symbol if there is reference to it.
	dataSegment 		- data segment
	codeSegment 		- code segment
	IC 			- instruction to work on.
	sFileFullName 		- full file name for error printing.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int TranslateLabeledData(char *operand, SymbolTable symTable, SymbolTable *stExternalSymbolTable, codeSegment (*codeSegment),
			 int codeSegmentLength, int *IC, char *sFileFullName, int numberOfLine) ;



#endif
