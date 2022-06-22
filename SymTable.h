#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinaryStructsAndDefMacros.h"


#define  PASS                    0
#define	NO_MEMORY_ALLOCATION	-1
#define	SYMBOL_FOUND		-2
#define ALLOW_DUPLICATE_LABELS  -3

/* linked list to store labels and macros */
struct symTabNode
{
	char sName[LABEL_MAX_LEN];
	int	nAddress;
	int	nType;
	int nCommandSize;

	struct	symTabNode *stNext;
};

typedef struct symTabNode *SymbolTable;


/*	Insert symbol\macro to the symbol table.
	symTable 	- symbol table.
	symbolNameSt 	- the name of the symbol to insert.
	symbolAddressN 	- the address of the symbol to insert
	symbolTypeN 	- the type of the symbol to insert
	commandSizeN    - if its a command label, holds number of lines to store in the .ob file
	output :
	Returns PASS if function ran successfully, -1 for mam allocation prob and -2 for a symbol that already found in the table
*/
int insertSymbol(SymbolTable *symTable, char *symbolNameSt, int symbolAddressN, int symbolTypeN,int commandSizeN);

/*	Gets a symbol from the symbol table
	symTable 	- symbol table.
	symbolNameSt 	- the symbol name to search for.
	output :
	Returns a symbol table node if symbol found and NULL otherwise
*/
SymbolTable getSymbol(SymbolTable symTable, char *symbolNameSt);

/*	Frees memory allocation and sets the head of the symbol table to NULL.
	symTable 	- the symbol table to clear.
*/
void clearSymbolTable(SymbolTable *symTable);


#endif
