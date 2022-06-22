#include "assemblerAssist.h"


void performMacroDecleration (char *StringLabel, char *fileFullName, int numberOfLine, char *stringCurrentLine, int *PointerInLineCpy,int *ErrorOn, SymbolTable *symTable )
{
    	char    sMacroName [LABEL_MAX_LEN] ;
	int     macroVal ;
	int     InsertMacro = TRUE ;
	int     nRC = 0 ; /* ERR num */

    /* Check if the label is legal */
    if (strcmp(StringLabel,"\0") != 0)
    {
        fprintf(stderr, "%s:%d: ERR: Illegal label before macro command.\n", fileFullName, numberOfLine);
        (*ErrorOn)++;
    }

    /* Skip spaces */
    while (isspace(stringCurrentLine[*PointerInLineCpy]))
        (*PointerInLineCpy)++;

    nRC = getMacroName (stringCurrentLine, sMacroName, PointerInLineCpy) ;

    if (nRC) {
        fprintf (stderr, "%s:%d: ERR: Macro name is not valid.\n", fileFullName,numberOfLine) ;
        (*ErrorOn)++;
        InsertMacro = FALSE ;
    }

    nRC = getMacroVal (stringCurrentLine, &macroVal, PointerInLineCpy) ;

    if (nRC) {
        fprintf (stderr, "%s:%d: ERR:Macro value is not valid.\n", fileFullName,numberOfLine) ;
        (*ErrorOn)++;
        InsertMacro = FALSE ;
    }

    if (InsertMacro) {

        nRC = insertSymbol (symTable, sMacroName, macroVal, MACRO_LABEL,DEFAULT_SIZE);

        if (nRC == NO_MEMORY_ALLOCATION)
            {
                fprintf(stderr, "memory allocation failed.\n");
                exit(1);
            }
            /* Check if macro already declared  */
        else if (nRC == SYMBOL_FOUND)
            {
                fprintf(stderr, "%s:%d: ERR: Duplicate macro found : \"%s\".\n",fileFullName,numberOfLine, sMacroName);
                (*ErrorOn)++;
            }
    }

}



void performDataOrStringDir (char *StringLabel, char *stringCurrentLine, int *PointerInLine, char *stringCommand,
                              SymbolTable *symTable, int *DC, char *fileFullName, int numberOfLine, int *dataSegment, int *ErrorOn)
{

    int     nCommandSize ;
    int     nRC = 0 ; /* ERR num */

    if (strlen(StringLabel) > 0)
    {
        /* insert label  */
        nCommandSize=instructionSize(stringCurrentLine,*PointerInLine,stringCommand);
        nRC = insertSymbol(symTable, StringLabel, *DC, DATA_LABEL,nCommandSize);


        if (nRC == NO_MEMORY_ALLOCATION)
        {
            fprintf(stderr, "memory allocation failed.\n");
            exit(1);
        }
        /* Check if duplicate symbol */
        else if (nRC == SYMBOL_FOUND)
        {
            fprintf(stderr, "%s:%d: ERR: Duplicate symbol found : \"%s\".\n", fileFullName, numberOfLine, StringLabel);
            (*ErrorOn)++;
        }
    }


    if (strcmp(DATA_DIRECTIVE, stringCommand) == 0)
    {
        if((insertNumbersToDataSegment(stringCurrentLine, PointerInLine, dataSegment, DC,fileFullName, numberOfLine, *symTable))!= 0)
        {
            (*ErrorOn)++;
            fprintf(stderr, "ERR1, line:%d\n", numberOfLine);
        }
    }

    /* The command is .string directive */
    else
    {
        if ((insertStringToDataSegment(stringCurrentLine, PointerInLine,dataSegment, DC,fileFullName, numberOfLine))!= 0)
        {
            (*ErrorOn)++;
            fprintf(stderr, "ERR2, line:%d\n", numberOfLine);
        }
    }

}



void performInsCommand (char *StringLabel, char *stringCurrentLine, int *PointerInLine, char *stringCommand,
                              SymbolTable *symTable, char *fileFullName, int numberOfLine, int *ErrorOn, int *IC)
{

    int PointerInLineCpy ;
    int nRC ; /* ERR num */
    int nCommandSize ;


    /* Check if there is a label */
    if (strlen(StringLabel) > 0)
    {

        nCommandSize = instructionSize(stringCurrentLine,*PointerInLine,stringCommand);
        nRC = insertSymbol(symTable, StringLabel, *IC, CODE_LABEL,nCommandSize);
        if (nRC == NO_MEMORY_ALLOCATION)
        {
            fprintf(stderr, "memory allocation failed.\n");
            exit(1);
        }
        /* Check if duplicate symbol */
        else if (nRC == SYMBOL_FOUND)
        {
            fprintf(stderr, "%s:%d: ERR: Duplicate symbol found : \"%s\".\n", fileFullName, numberOfLine, StringLabel);
            (*ErrorOn)++;
        }
    }

    /* Check if command is a legal instruction */
    if (instructionIndex(stringCommand)!= -1)
    {
            PointerInLineCpy = *PointerInLine;
            /* Increase IC by command length */
            if(Increase_IC(stringCurrentLine, &PointerInLineCpy, stringCommand, IC, fileFullName, numberOfLine)==TRUE)
            {
                fprintf(stderr, "ERR4, line : %d", numberOfLine);
                (*ErrorOn)++;
            }

    }
    /* The command is illegal*/
    else
    {
        fprintf(stderr, "%s:%d: ERR: unknown command \"%s\".\n", fileFullName, numberOfLine, stringCommand);
        (*ErrorOn)++;
    }

}
