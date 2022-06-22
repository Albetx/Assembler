#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandParser.h"
#include "assemblerAssist.h"
#include "BinaryStructsAndDefMacros.h"
#include "Output.h"
#include "FileInteraction.h"
#include "MemStructsHandle.h"
#include "instructionCompile.h"


#define	FIRST_PROG	        1
#define	NUM_OF_START_ARGS	1
#define	FIRST_LINE		1
#define	DATA_LENGTH	        500
#define	CODE_LENGTH	        500
#define MACRO_DECLERATION_COM_LEN 7


/*
    First pass on the assembly code, fills the symbol table with labels and macros, counts the code and data segment length.
    fileFullName - name of the current file to compile
    filePointer - pointer to the file
    symTable - symbol table
    IC - code ins count
    DC - data ins count
    dataSegment - array for data segment
    output:
    0 if no errors occurred during the pass, otherwise it returns different number.
*/
int firstPassing(char *fileFullName, FILE *filePointer, SymbolTable *symTable, int *IC, int *DC,int *dataSegment) ;

/*
    Second pass on the assembly code, fills the internal+external symbol tables, makes the binary code for the code and data segments and stores
    it in arrays.
    fileFullName - name of the current file to compile
    filePointer - pointer to the file
    symTable - symbol table
    InternalSymTable - int symbol table
    ExternalSymTable - ext symbol table
    codeSegmentmentLength - length of the CS
    dataSegmentLength - length of DS
    codeSegment - CS array
    dataSegment - array for data segment
    output:
    0 if no errors occurred during the pass, otherwise it returns different number.
*/
int	secondPassing(char *fileFullName, FILE *filePointer, SymbolTable symTable, int codeSegmentmentLength, int dataSegmentLength,
				  int *dataSegment, codeSegment (*codeSegment), SymbolTable *InternalSymTable,SymbolTable *ExternalSymTable) ;
