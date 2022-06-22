#include "SymTable.h"



int insertSymbol(SymbolTable *symTable, char *symbolNameSt, int symbolAddressN, int symbolTypeN,int commandSizeN)
{
	struct symTabNode *newNode;
	struct symTabNode *currNode;
   	int	outputCode = PASS;
	int	nodeFound = FALSE;


	if (*symTable == NULL)
	{
		newNode = (SymbolTable)malloc(sizeof(struct symTabNode));

		if (newNode != NULL)
		{
			/* Set the new Node data with the name, address and type */
			strcpy(newNode->sName, symbolNameSt);
			newNode->nAddress = symbolAddressN;
			newNode->nType = symbolTypeN;
			newNode->nCommandSize = commandSizeN;
			newNode->stNext = NULL;

			*symTable = newNode;
		}
		/* If memory allocation failed*/
		else
			outputCode = NO_MEMORY_ALLOCATION;
	}
	/* If there more nodes in the symbol table */
	else
	{
		/* Set off the flag that indicates whether the symbol was found in symbol table or not */
		nodeFound = FALSE;

		currNode = *symTable;

		while ((currNode->stNext != NULL) && !nodeFound)
		{
			if (strcmp(currNode->sName, symbolNameSt) == 0)
			{
                	/* The symbol was found */
                	nodeFound = TRUE;
			}
			currNode = currNode->stNext;
		}

		/* Check if the last Node name is not the given name and that the given symbol wasn't already found */
		if ((symbolTypeN == ALLOW_DUPLICATE_LABELS) || ((strcmp(currNode->sName, symbolNameSt) != 0) && (!nodeFound)))
		{
			newNode = (SymbolTable)malloc(sizeof(struct symTabNode));

			if (newNode != NULL)
			{
				/* Set the new Node data with the name, address and type */
				strcpy(newNode->sName, symbolNameSt);
				newNode->nAddress = symbolAddressN;
				newNode->nType = symbolTypeN;
				newNode->nCommandSize = commandSizeN;
				newNode->stNext = NULL;

				currNode->stNext = newNode;
			}
			/* if memory allocation failed*/
			else
			{
				/* Insert failed */
				outputCode = NO_MEMORY_ALLOCATION;
			}
		}

		/* Last symbol in the table has the same name */
		else
            		nodeFound = TRUE;
	}

	/* If given symbol was already in the symbol table */
	if (nodeFound)
	{
		/* Symbol was found */
		outputCode = SYMBOL_FOUND;
	}
	/* Return indication for a successful insert or fail */
	return outputCode;
}


SymbolTable getSymbol(SymbolTable symTable, char *symbolNameSt)
{
	struct symTabNode	*currNode;

	if (symTable != NULL)
	{
		currNode = symTable;
		while ((currNode->stNext != NULL))
		{
			if (strcmp(currNode->sName, symbolNameSt) == 0)
				return currNode;

			currNode = currNode->stNext;
		}

		/* Check if the last Node data is not equal and that the given symbol wasn't already found */
		if ((strcmp(currNode->sName, symbolNameSt) == 0))
		{
			return currNode;
		}
	}
	/* Symbol wasn't found */
	return NULL;
}

void clearSymbolTable(SymbolTable *symTable)
{
	struct symTabNode *currNode;
	struct symTabNode *nextNode;

	if (*symTable != NULL)
	{
		currNode = *symTable;

		while (currNode != NULL)
		{
			nextNode = currNode->stNext;
			free(currNode);
			currNode = nextNode;
		}
	}
	*symTable = NULL;

}
