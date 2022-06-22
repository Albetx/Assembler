#ifndef ASSEMBLERASSIST_H
#define ASSEMBLERASSIST_H

#include "SymTable.h"
#include "assembler.h"



/*	Compiles macro declaration commands, checks for code correctness and stores the macro in the symbol table.
	StringLabel 	- macro name.
	fileFullName    - full file name for error printing.
	ErrorOn         - check for error in the code.
	symTable        - symbol table to store the macro in.
*/
void performMacroDecleration (char *StringLabel, char *fileFullName, int numberOfLine, char *stringCurrentLine, int *PointerInLineCpy,
                            int *ErrorOn, SymbolTable *symTable );

/*	Compiles data and string directives, checks for code correctness and stores the values in the data segment.
	StringLabel 	- macro name.
	fileFullName    - full file name for error printing.
	ErrorOn         - check for error in the code.
	symTable        - symbol table used in case the value is a macro.
	dataSegment     - data segment to store the data values in.
*/
void performDataOrStringDir (char *StringLabel, char *stringCurrentLine, int *PointerInLine, char *stringCommand,
                              SymbolTable *symTable, int *DC, char *fileFullName, int numberOfLine, int *dataSegment, int *ErrorOn);

/*	Compiles instruction commands, checks for code correctness, stores the label in the symbol table with the size that the command will need
    in the object file.
	StringLabel 	- macro name.
	fileFullName    - full file name for error printing.
	ErrorOn         - check for error in the code.
	symTable        - symbol table to hold the label.
*/
void performInsCommand (char *StringLabel, char *stringCurrentLine, int *PointerInLine, char *stringCommand,
                              SymbolTable *symTable, char *fileFullName, int numberOfLine, int *ErrorOn, int *IC);

#endif 
