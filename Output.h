#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "BinaryStructsAndDefMacros.h"
#include "SymTable.h"


#define THREE_DIGIT_MAX     	999
#define LEN_OF_TAB		1
#define BASE_4_OFFSET		7
#define FILE_DELETE_ERR	   	-1
#define BYTE_SIZE           	8
#define ADDRESS_DEC         	4
#define SPACE               	1
#define END_OF_STRING_SIGN  	1
#define BASE                	4


/* Bit filed to make the base convert simple */
typedef struct
{
	unsigned int value:14;
}bitField;



/*  Writes the first line of the .obj file
    The line will be in the following format:	CodeSize DataSize
*/
void writeFileFirstLine(FILE *filePointer, int nCodeSize, int nDataSize);

/*  Writes Code segment to the file in the following format: Address(base 10)	code(in base 4)
*/
void writeCode(FILE *filePointer,int nStartAddress,codeSegment (*codeSegment),int IC);

/*  Writes Data segment to the file in the following format: Address(base 10)	data(in base 4)
*/
void writeData(FILE *filePointer,int nStartAddress,int *dataSegment,int DC);

/*  Converts a number to a base 4 in string type
	stringToWrite - string for base 4 value
	nData - number to convert
	nPad - offset for the wanted base
*/
void WriteInBase4(char* stringToWrite, int nData,int nPad);

/*  Goes over symbol table and writes external end entry lines to the .obj file
*/
void writeEntriesExternals(FILE *filePointer,SymbolTable stSymbolTable);

/*  Writes the entry or extern line to the file in the following format: Label(string)	Address(base 10).
*/
void writeEntriesExternalsToFile(FILE *filePointer, char* strEntryExternLabel, int sAddress);

#endif
