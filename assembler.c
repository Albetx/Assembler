/*
Assembler 1.0 - Maman14
Name: Albert Ilyasov
Date: Aug 2019
Description: assembler for assembly language according to the Open University requirements.
Input: file in .as format that include commands and directives in the assambly language.
Output: 3 files, one for the translated code to base 4, one for external lables addresses and one for entry lables addresses.
 
*/


#include "assembler.h"


int main (int argc, char *argv[]) {

	int		IC = 0;
	int		DC = 0;
	int		fileNumber = FIRST_PROG;
	int		ErrorOn	= FALSE;
	char		*fileName;
	char		fileFullName[MAX_LEN];
	FILE		*filePointer;
	SymbolTable	symTable 		= NULL;
	SymbolTable	InternalSymTable 	= NULL;
	SymbolTable	ExternalSymTable 	= NULL;
	int		dataSegment[DATA_LENGTH] = {0};
	codeSegment	codeSegment[CODE_LENGTH] = {{{0}},{{0}}};
	outputFiles 	output;


	if ((argc - NUM_OF_START_ARGS) > 0)
	{

		while (fileNumber < argc)
		{

			fileName = argv[fileNumber];

			/* Adds ".as" at the end of the source file name */
			if (addFileFormat(fileFullName, fileName, EXTENSTION_ASM) != NULL)
			{

				filePointer = fopen(fileFullName, READ);

				if (filePointer != NULL)
				{

					fprintf(stdout, "Compiling file \"%s\"...\n", fileFullName);

					/* Assembler first pass */
					ErrorOn += firstPassing(fileFullName, filePointer, &symTable,&IC, &DC, dataSegment);

					/* Assembler second pass */
                    			ErrorOn += secondPassing(fileFullName, filePointer, symTable,IC, DC, dataSegment,codeSegment,&InternalSymTable, &ExternalSymTable);


					fclose(filePointer);

					if (ErrorOn==FALSE)
					{

						if (OpenFiles(fileName,&output) != FALSE )
						{
							/* Write output files: object, entry, extern */
							writeFileFirstLine(output.fpCodeFile,IC,DC);
							writeCode(output.fpCodeFile,PROGRAM_LOAD_ADDRESS,codeSegment,IC);
							writeData(output.fpCodeFile,PROGRAM_LOAD_ADDRESS + IC,dataSegment,DC);
							writeEntriesExternals(output.fpEntryFile,InternalSymTable);
							writeEntriesExternals(output.fpExternalFile,ExternalSymTable);
							DeleteEmptyFiles(fileName, &output);
							CloseFiles(&output);

							fprintf(stdout, "Compile success for \"%s\"...\n", fileFullName);
						}
					}

					/* there was an error during the pass*/
					else
					{
						fprintf(stderr, "Compile failed for \"%s\".\n", fileFullName);
					}

					clearSymbolTable(&symTable);
					clearSymbolTable(&InternalSymTable);
					clearSymbolTable(&ExternalSymTable);
				}
				/* cant open the file */
				else
				{
					fprintf(stderr, "Error can not open file \"%s\".\n", fileFullName);
					exit(1);
				}
			}
			else
			{
				fprintf(stderr, "File name error occurred.\n");
				exit(1);
            		}

	    		/* Preparation before reading the next file  */
			fileNumber++;
			IC = 0;
			DC = 0;
			ErrorOn = FALSE;
			fprintf(stderr, "_____________________________\n");
		} /* End of files loop */
	}
	/* No file entered */
	else
	{
		fprintf(stderr, "No files to compile, stopped.\n");
	}

	return FALSE;

}




int firstPassing(char *fileFullName, FILE *filePointer, SymbolTable *symTable, int *IC, int *DC,int *dataSegment)
{

	int	ErrorOn 		= FALSE;
	int     bLastErrorOccured 	= FALSE;
	int	nNumOfEmptyLines 	= 0;
	int	nRC 			= 0;
	int	numberOfLine 		= FIRST_LINE;
	int	PointerInLine;
	int	PointerInLineCpy;
	char    sMacroDecleration   [MACRO_DECLERATION_COM_LEN + END_OF_STRING_SIGN] ;
	char	stringCurrentLine   [MAX_LEN];
	char	stringCommand       [MAX_LEN];
	char	StringLabel         [LABEL_MAX_LEN];
	char    *stringCurrentLinePtr ;

	clearSymbolTable(symTable);

    while (fgets(stringCurrentLine, MAX_LEN, filePointer) != NULL) {

        PointerInLine = 0;

        if (!checkIfEmptyLine(stringCurrentLine)) {

            GoToNextParam (stringCurrentLine, &PointerInLine) ; 

            /* Get command label and command itself */
            nRC = getLabel(stringCurrentLine, StringLabel, &PointerInLine);

            stringCurrentLinePtr = stringCurrentLine+PointerInLine ; /* Pointer from the start of the expression */
            strncpy (sMacroDecleration, stringCurrentLinePtr, MACRO_DECLERATION_COM_LEN) ;
            PointerInLineCpy = PointerInLine + MACRO_DECLERATION_COM_LEN ;

            /* In case this is a macro command */
            if (strcmp(sMacroDecleration , MACRO_DIRECTIVE) == 0 )
                performMacroDecleration(StringLabel,fileFullName,numberOfLine,stringCurrentLine,&PointerInLineCpy, &ErrorOn, symTable) ;

            /* Not a macro command */
            else {

                /* Check if the label is legal */
                if (nRC != 0) {
                    fprintf(stderr, "%s:%d: ERR: Illegal label name.\n", fileFullName, numberOfLine);
                    ErrorOn++;
                }

                getInstruction(stringCurrentLine, stringCommand, &PointerInLine);

                /* Check if there is a command in the current line */
                if (strlen(stringCommand) > 0)
                {
                    /* .data or .string directive */
                    if ((strcmp(DATA_DIRECTIVE, stringCommand) == 0) || (strcmp(STRING_DIRECTIVE, stringCommand) == 0)){
                        performDataOrStringDir (StringLabel, stringCurrentLine, &PointerInLine, stringCommand, symTable,
                                                 DC, fileFullName, numberOfLine, dataSegment, &ErrorOn);
                    }

                    /* .extern or .entry directive */
                    else if ((strcmp(EXTERN_DIRECTIVE, stringCommand) == 0) || (strcmp(ENTRY_DIRECTIVE, stringCommand) == 0))
                    {
                        if (strlen(StringLabel) > 0)
                            fprintf(stderr, "%s:%d: WARN: Label ignored.\n", fileFullName, numberOfLine);


                        if (strcmp(EXTERN_DIRECTIVE, stringCommand) == 0) {
                            if(insertExternSymbol(stringCurrentLine, &PointerInLine, symTable, fileFullName,numberOfLine) == TRUE) {
                                fprintf(stderr, "ERR3, line : %d", numberOfLine);
                                ErrorOn++;
                            }
                        }
                    }

                    /* The command is instruction */
                    else {
                        performInsCommand (StringLabel, stringCurrentLine, &PointerInLine, stringCommand, symTable,
                                           fileFullName, numberOfLine, &ErrorOn, IC) ;
                    }

                }
                /* If there is no command in current line */
                else
                    fprintf(stderr, "%s:%d: warning: a line without a command, line ignored.\n", fileFullName, numberOfLine);

            } /* End of no macro command */
        } /* End of not empty or command line */

        /* Line is empty or comment */
        else
        {
            /* Increase empty and comment lines counter */
            nNumOfEmptyLines++;
        }

        /* Move on to next line */
        numberOfLine++;
        bLastErrorOccured+=ErrorOn;
        ErrorOn=FALSE;

    } /* End of while loop */


    /* If all the lines are empty or comment lines or there no line to read */
    if ((numberOfLine == nNumOfEmptyLines) || (numberOfLine == FIRST_LINE))
    {
       fprintf(stderr, "file : %s is empty...\n", fileFullName);
    }

    if(bLastErrorOccured>0)
        return TRUE;

    return FALSE;


}




int	secondPassing(char *fileFullName, FILE *filePointer, SymbolTable symTable, int codeSegmentmentLength, int dataSegmentLength,
				  int *dataSegment, codeSegment (*codeSegment), SymbolTable *InternalSymTable,SymbolTable *ExternalSymTable)
{

	/* Data Definition */
	int	ErrorOn 			= FALSE;
	int	IC 				= 0;
	int	numberOfLine 	= FIRST_LINE;
	int	PointerInLine;
	char	stringCurrentLine[MAX_LEN];
	char	StringLabel[LABEL_MAX_LEN];
	char	stringCommand[MAX_LEN];

	/* Go back to beginning of the file*/
	rewind(filePointer);

	while (fgets(stringCurrentLine, MAX_LEN, filePointer) != NULL)
	{
		PointerInLine = 0;

		if (!checkIfEmptyLine(stringCurrentLine))
		{

			getLabel(stringCurrentLine, StringLabel, &PointerInLine);
			getInstruction(stringCurrentLine, stringCommand, &PointerInLine);

			/* Check if there is a command in the current line */
			if (strlen(stringCommand) > 0)
			{
				if ((strcmp(DATA_DIRECTIVE, stringCommand) != 0) && (strcmp(STRING_DIRECTIVE, stringCommand) != 0))
				{
					if (strcmp(ENTRY_DIRECTIVE, stringCommand) == 0)
					{
						ErrorOn+= insertEntrySymbol(stringCurrentLine,&PointerInLine,symTable,InternalSymTable,codeSegmentmentLength,
                                  		fileFullName,numberOfLine);
					}
					/* Check if command is instruction */
					else if (instructionIndex(stringCommand) != -1)
					{
						/* Append code segment with command and operands */
						ErrorOn+= TranslateInstruction(stringCurrentLine, &PointerInLine, stringCommand, &IC, codeSegmentmentLength,
                                    		dataSegment ,codeSegment, symTable,ExternalSymTable, fileFullName, numberOfLine);

					}
				}
			}
		}

		/* Move on to next line */
		numberOfLine++;
	}

	/* Return error indication */
	return ErrorOn;
}
