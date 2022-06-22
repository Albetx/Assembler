#include "MemStructsHandle.h"


int insertNumbersToDataSegment(char *LineString, int *PointerInLine, int *dataSegment, int *DC,
							  char *FileName, int numberOfLine, SymbolTable symTable)
{
	int	ErrorOn = FALSE;
	int	numberExpected = 1;
	char	sCurrNumber[MAX_LEN];
    	struct	symTabNode *macroSymbol = NULL;

	while (LineString[*PointerInLine] != '\n')
	{
		getNextParam(LineString, PointerInLine, sCurrNumber);

		if (strlen(sCurrNumber) > 0)
		{
			/* Check if another parameter expected */
			if (numberExpected)
			{
			    if (checkNumber(sCurrNumber) || checkLabel(sCurrNumber)) 
				{

				    if (checkNumber(sCurrNumber))
				    {
					/* Insert to data segment and increase DC */
					dataSegment[*DC] = atoi(sCurrNumber);
					(*DC)++;
				    }

				    /* Parameter is a macro, find its actual value and insert it to the data segment then increase DC */
				    else  {
					macroSymbol = getSymbol(symTable,sCurrNumber) ;
					if (macroSymbol != NULL) {
					    dataSegment[*DC] = macroSymbol->nAddress;
					    (*DC)++;
					}
					/* If the parameter is not a legal number */
					else {
					    fprintf(stderr, "%s:%d: ERR: \"%s\" is not a legal number\\macro for .data\n", FileName, numberOfLine, sCurrNumber);
					    ErrorOn = TRUE;
					}
		    	}
			    }
				/* Check if there is a comma sign */
				if (LineString[*PointerInLine] == ',')
				{
					(*PointerInLine)++;
				}
				/* If its the end of parameters list */
				else
				{
					numberExpected = FALSE;
				}
			}
			/* Check if there are more parameters although end of line expected */
			else
			{
				fprintf(stderr, "%s:%d: ERR: End of line expected for .data\n", FileName, numberOfLine);
				ErrorOn = TRUE;

				/* Terminate line parsing */
				LineString[*PointerInLine] = '\n';
			}
		}
		/* If there is no parameter */
		else
		{
			fprintf(stderr, "%s:%d: ERR: Number expected for .data\n",FileName, numberOfLine);
			ErrorOn = TRUE;
			(*PointerInLine)++;
			numberExpected = FALSE;
		}
	}

	/* Check if another parameter expected */
	if (numberExpected)
	{
		fprintf(stderr, "%s:%d: ERR: Number expected for .data\n", FileName, numberOfLine);
		ErrorOn = TRUE;
	}

	return ErrorOn;
}


int insertStringToDataSegment(char *LineString, int *PointerInLine, int *dataSegment, int *DC, char *FileName,int numberOfLine)
{
	int	ErrorOn = FALSE;
	char 	stringToInsert[MAX_LEN];
	int	pointerOfString = 0;

	if (LineString[*PointerInLine] != '\"')
	{
		fprintf(stderr, "%s:%d: ERR: String expected for .string \n", FileName, numberOfLine);
		ErrorOn = TRUE;
	}
	/* If its a start of a string */
	else
	{
		(*PointerInLine)++;

		/* Read string until its end or until end of line */
		for (pointerOfString = 0; (LineString[*PointerInLine] != '\"') &&
					(LineString[*PointerInLine] != '\n'); pointerOfString++, (*PointerInLine)++)
		{
			stringToInsert[pointerOfString] = LineString[*PointerInLine];
		}

		/* Finish string */
		stringToInsert[pointerOfString] = '\0';

		if (LineString[*PointerInLine] != '\"')
		{
			fprintf(stderr, "%s:%d: ERR: String-End expected for .string \n", FileName,numberOfLine);
			ErrorOn = TRUE;
		}
		/* If there is a string end sign */
		else
		{
			if (strlen(stringToInsert) > 0)
			{
				/* Insert string to data segment */
				for (pointerOfString = 0; pointerOfString < strlen(stringToInsert); pointerOfString++)
				{
					dataSegment[*DC] = stringToInsert[pointerOfString];
					(*DC)++;
				}

				/* For '\0' sign at end of string */
				(*DC)++;
			}
			/* If this is an empty string */
			else
				fprintf(stderr, "%s:%d: WARN: An empty string in .string\n", FileName,numberOfLine);

			(*PointerInLine)++;

			GoToNextParam(LineString, PointerInLine);

			/* Check if there is something that is not an end of line sign */
			if (LineString[*PointerInLine] != '\n')
			{
				fprintf(stderr, "%s:%d: ERR: End of line expected for .string \n",FileName, numberOfLine);
				ErrorOn = TRUE;
			}
		}
	}

	return 	ErrorOn;
}


int insertExternSymbol(char *LineString, int *PointerInLine, SymbolTable *symTable, char *FileName, int numberOfLine)
{
	int	nRC;
	int	ErrorOn = FALSE;
	char	sExternalSymbol[MAX_LEN];
	struct	symTabNode *stSymbol = NULL;

	getNextParam(LineString, PointerInLine, sExternalSymbol);

	if (strlen(sExternalSymbol) == 0)
	{
		fprintf(stderr, "%s:%d: ERR: Missing label for .extern directive.\n", FileName, numberOfLine);
		ErrorOn = TRUE;
	}
	else if (!checkLabel(sExternalSymbol))
	{
		fprintf(stderr, "%s:%d: ERR: Illegal label name \"%s\".\n", FileName, numberOfLine, sExternalSymbol);
		ErrorOn = TRUE;
	}
	/* Label is legal */
	else
	{
 		stSymbol = getSymbol(*symTable, sExternalSymbol);

		if (stSymbol == NULL)
		{
			nRC = insertSymbol(symTable, sExternalSymbol, 0, EXTERN_LABEL, DEFAULT_SIZE);

			if (nRC == -1)
			{
				fprintf(stderr, "memory alloca failed.\n");
				exit(1);
			}
		}
		/* If a symbol was found */
		else
		{
			/* Check if it is a data symbol or code symbol, then the external is duplicate */
			if (((stSymbol->nType) == DATA_LABEL) || ((stSymbol->nType) == CODE_LABEL))
			{
				fprintf(stderr, "%s:%d: ERR: Duplicate symbol \"%s\".\n", FileName, numberOfLine, sExternalSymbol);
				ErrorOn=TRUE;
			}
		}
	}

	GoToNextParam(LineString, PointerInLine);

	/* Check if there is something that is not an end of line sign */
	if (LineString[*PointerInLine] != '\n')
	{
		fprintf(stderr, "%s:%d: ERR: end of line expected after label.\n", FileName, numberOfLine);
		ErrorOn = TRUE;
	}

	/* Return error indicator */
	return 	ErrorOn;
}


int insertEntrySymbol(char *LineString, int *PointerInLine, SymbolTable symTable,SymbolTable *InternalSymTable,
                       int codeSegmentLength, char *FileName,int numberOfLine)
{
	int	ErrorOn = FALSE;
	int	nRC = 0;
	char	sEnrtySymbol[MAX_LEN];
	struct	symTabNode *stSymbol = NULL;

	/* Read entry symbol */
	getNextParam(LineString, PointerInLine, sEnrtySymbol);

	if (strlen(sEnrtySymbol) == 0)
	{
		fprintf(stderr, "%s:%d: ERR: Missing label for .entry \n",FileName, numberOfLine);
		ErrorOn = TRUE;
	}
	else if (!checkLabel(sEnrtySymbol))
	{
		fprintf(stderr, "%s:%d: ERR: Illegal label name \"%s\".\n", FileName, numberOfLine, sEnrtySymbol);
		ErrorOn = TRUE;
	}
	/* If symbol is a valid label */
	else
	{
		stSymbol = getSymbol(symTable, sEnrtySymbol);

		if (stSymbol != NULL)
		{
			if ((stSymbol->nType) == DATA_LABEL)
			{
				/* Insert entry symbol to internal symbol table */
				nRC = insertSymbol(InternalSymTable, sEnrtySymbol,(stSymbol->nAddress) + codeSegmentLength + PROGRAM_LOAD_ADDRESS, ENRTY_LABEL,DEFAULT_SIZE);

				/* Check if memory allocation failed */
				if (nRC == -1)
				{
					fprintf(stderr, "Memory allocation failed.\n");
					exit(1);
				}
			}
			else if ((stSymbol->nType) == CODE_LABEL)
			{
				/* Insert entry symbol to internal symbol table */
				nRC = insertSymbol(InternalSymTable, sEnrtySymbol,(stSymbol->nAddress) + PROGRAM_LOAD_ADDRESS, ENRTY_LABEL,DEFAULT_SIZE);

				/* Check if memory allocation failed */
				if (nRC == -1)
				{
					fprintf(stderr, "Memory allocation failed.\n");
					exit(1);
				}
			}
			else if ((stSymbol->nType) == EXTERN_LABEL)
			{
				fprintf(stderr, "%s:%d: ERR: illegal label reference \"%s\".\n", FileName,numberOfLine, sEnrtySymbol);
				ErrorOn = TRUE;
			}
		}
		/* If no symbol was found */
		else
		{
			fprintf(stderr, "%s:%d: ERR: Label %s not found.\n", FileName, numberOfLine, sEnrtySymbol);
			ErrorOn = 1;
		}
	}

	GoToNextParam(LineString, PointerInLine);

	if (LineString[*PointerInLine] != '\n')
	{
		fprintf(stderr, "%s:%d: ERR: Line end expected after .enrty label.\n", FileName, numberOfLine);
		ErrorOn = 1;
	}

	return ErrorOn;
}
