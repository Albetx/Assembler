#ifndef MEMSTRUCTSHANDLE_H
#define MEMSTRUCTSHANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandParser.h"
#include "BinaryStructsAndDefMacros.h"
#include "SymTable.h"

#define DEFAULT_SIZE 0 /* default command size when not relevant */



/*	Saves numeric operands in the data segment
	LineString  	- line to check.
	dataSegment 	- data segment .
	DC 		- data counter.
	FileName 	- full file name for error printing.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int insertNumbersToDataSegment(char *LineString, int *PointerInLine, int *dataSegment, int *DC,
							  char *FileName, int numberOfLine, SymbolTable symTable) ;


/*	Saves .string operands in the data segment
	LineString  	- line to check.
	dataSegment 	- data segment .
	DC 		- data counter.
	FileName 	- full file name for error printing.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int	insertStringToDataSegment(char *LineString, int *PointerInLine, int *dataSegment, int *DC, char *FileName,int numberOfLine);


/*	Saves .extern symbol in symbol table
	LineString 	- line to check.
	symTable 	- symbol table .
	FileName 	- full file name for error printing.
	numberOfLine 	- line number in file to work on.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/


int	insertExternSymbol(char *LineString, int *PointerInLine, SymbolTable *symTable, char *FileName, int numberOfLine);

/*	Saves .entry symbol in symbol table
	LineString 		- line to check.
	symTable 		- symbol table .
	InternalSymTable 	- internal symbol table.
	FileName 		- full file name for error printing.
	numberOfLine 		- line number in file to work on.
	output :
	Returns 0 if function ran successfully, and a different number otherwise
*/
int	insertEntrySymbol(char *LineString, int *PointerInLine, SymbolTable symTable,SymbolTable *InternalSymTable,
                       int codeSegmentLength, char *FileName,int numberOfLine);

#endif
