#include "commandParser.h"



int checkIfEmptyLine(char *LineString)
{
	int	emptyLine = TRUE;
	int	PointerInLine = 0;

	/* Skip spaces */
	while (isspace(LineString[PointerInLine]))
        PointerInLine++;

	if (LineString[PointerInLine] != SIGN_OF_COMMENT)
	{
		while (LineString[PointerInLine] != '\n'  && emptyLine)
		{
			if ((LineString[PointerInLine] != ' ') && (LineString[PointerInLine] != '\t'))
			{
				/* Line is not empty or comment */
				emptyLine = FALSE;
			}

			PointerInLine++;
		}
	}

	return emptyLine;
}


int	getLabel(char *LineString, char *StringLabel, int *PointerInLine)
{
	int	checkError = FALSE;
	int	PointerOfLabel = 0;
	char 	sHoldLabel[MAX_LEN];

	/* Initialize label */
	sHoldLabel[PointerOfLabel] = '\0';

        /* Skip spaces */
	GoToNextParam (LineString, PointerInLine) ;

	/* Read label until end of line or label-ending sign, as long as all characters are alphanum */
	for (PointerOfLabel = 0; (LineString[*PointerInLine] != '\n') && (LineString[*PointerInLine] != SIGN_OF_END_LABEL);
		PointerOfLabel++ ,(*PointerInLine)++)
	{
		sHoldLabel[PointerOfLabel] = LineString[*PointerInLine];
	}

	if (LineString[*PointerInLine] == SIGN_OF_END_LABEL)
	{
		sHoldLabel[PointerOfLabel] = '\0';
		(*PointerInLine)++;

		/* Check if the label is a legal label */
		if (!checkLabel(sHoldLabel))
		{
			sHoldLabel[0] = '\0';
			checkError = TRUE;
		}
	}
	/* There is no label */
	else
	{
		/* Initialize label */
		sHoldLabel[0] = '\0';

		*PointerInLine = *PointerInLine - PointerOfLabel;
	}

	/* Copy label to return the label to the caller program */
	strcpy(StringLabel,sHoldLabel);

	GoToNextParam(LineString, PointerInLine);

	return checkError;
}


void GoToNextParam(char *LineString, int *PointerInLine)
{
	while ((LineString[*PointerInLine] == ' ') || (LineString[*PointerInLine] == '\t'))
	{
		(*PointerInLine)++;
	}
}


int checkLabel(const char *StringToCheck)
{

	int	nPointerInStr = 0;

	if (StringToCheck == NULL || *StringToCheck == '\0')
	{
		return FALSE;
	}

	/* Check if the first character is not an alpha or if length in too long */
	if (!isalpha(*StringToCheck) || (strlen(StringToCheck) > MAX_LEN -1))
	{
		return FALSE;
	}

	while (StringToCheck[nPointerInStr] != '\0')
	{
		/* Check if the current character is not alphanumeric */
		if (!isalnum(StringToCheck[nPointerInStr]))
		{
			return FALSE;
		}

		nPointerInStr++;
	}

	/* Check if the string is not an register or instruction name */
	return ((!checkRegister(StringToCheck)) && (instructionIndex(StringToCheck) == -1) && !checkDirective(StringToCheck));
}

int	getMacroName(char *LineString, char *StringMacro, int *PointerInLine)
{
	int	checkError = FALSE;
	int	PointerOfMacro = 0;
	char sLongMacro[MAX_LEN];

	/* Init Macro */
	sLongMacro[PointerOfMacro] = '\0';

	/* Read Macro name until end of line or macro-ending sign, as long as all characters are alphanum */
	for (PointerOfMacro = 0 ; !isspace(LineString[*PointerInLine]) && (LineString[*PointerInLine] != SIGN_OF_END_MACRO);
                                                                                        PointerOfMacro++ ,(*PointerInLine)++)
	{
		sLongMacro[PointerOfMacro] = LineString[*PointerInLine];
	}

	if (LineString[*PointerInLine] == SIGN_OF_END_MACRO || isspace(LineString[*PointerInLine]))
	{
		sLongMacro[PointerOfMacro] = '\0';
		(*PointerInLine)++;

		/* Check if the macro is a legal macro */
		if (!checkLabel(sLongMacro))
		{
            /* Init macro name */
			sLongMacro[0] = '\0';
			checkError = TRUE;
		}
	}
	/* There is no macro name */
	else
	{
		/* Init label */
		sLongMacro[0] = '\0';

		*PointerInLine = *PointerInLine - PointerOfMacro;
	}

	/* Copy label to return the label to the caller program */
	strcpy(StringMacro,sLongMacro);

	GoToNextParam(LineString, PointerInLine);

	return checkError;
}


int	getMacroVal(char *LineString, int *MacroVal, int *PointerInLine)
{

    int	checkError = FALSE;
    int	PointerOfMacro;
    char sMacroVal[MAX_LEN];

	if (LineString[*PointerInLine] == SIGN_OF_END_MACRO) /* Skip macro equality sign */
		(*PointerInLine)++;

    GoToNextParam(LineString, PointerInLine);

    for (PointerOfMacro = 0 ; LineString[*PointerInLine] != '\n'; PointerOfMacro++, (*PointerInLine)++) {
        sMacroVal [PointerOfMacro] = LineString[*PointerInLine] ; /* Extract macro value */
    }

    sMacroVal [PointerOfMacro] = '\0' ;
    *MacroVal = atoi (sMacroVal) ;

    /* If no value entered */
    if (strcmp(sMacroVal , "\0") == 0)
        checkError = TRUE ;

    /* If "atoi" returned 0 while the value was'nt 0 - macro not valid */
    if ((*MacroVal == 0) && strcmp (sMacroVal, "0") != 0 )
        checkError = TRUE ;

    return checkError ;

}

void getInstruction(char *LineString, char *stringCommand, int *PointerInLine)
{
	int nCommandPointer = 0;

	/* Init command */
	stringCommand[nCommandPointer] = '\0';

	/* Read command  */
	for (nCommandPointer = 0; !isspace(LineString[*PointerInLine]) ; nCommandPointer++, (*PointerInLine)++)
	{
		stringCommand[nCommandPointer] = LineString[*PointerInLine];
	}

	/* Finish command */
	stringCommand[nCommandPointer] = '\0';

	GoToNextParam(LineString, PointerInLine);
}


void goOver(char *LineString, int *PointerInLine)
{
	while ((LineString[*PointerInLine] != ',') && (LineString[*PointerInLine] != '\0') && (LineString[*PointerInLine] != '\n'))
	{
		(*PointerInLine)++;
	}
	while ((LineString[*PointerInLine] != ' ') && (LineString[*PointerInLine] != '\t') && (LineString[*PointerInLine] != '\n'))
	{
		(*PointerInLine)++;
	}
}

void getNextParam(char *LineString, int *PointerInLine,char *sParam)
{
	int nParamPointer = 0;

	GoToNextParam(LineString, PointerInLine);

	/* Set param to empty param */
	sParam[nParamPointer] = '\0';

	/* Read param  */
    for (nParamPointer = 0;	(LineString[*PointerInLine] != ',') &&
                !(isspace(LineString[*PointerInLine])); nParamPointer++, (*PointerInLine)++)
    {
        sParam[nParamPointer] = LineString[*PointerInLine];
    }

	/* Finish param */
	sParam[nParamPointer] = '\0';
	GoToNextParam(LineString, PointerInLine);
}


int checkNumber(const char *StringToCheck)
{
	char	*sPointerToRest;
	int	nConvNum;

	if ((StringToCheck == NULL) ||
		(!isdigit(*StringToCheck) && !(*StringToCheck == '-') && !(*StringToCheck == '+')))
	{
		return FALSE;
	}

	nConvNum = strtol(StringToCheck, &sPointerToRest, 10);

	/* Check if converted number is not in range */
	if ((nConvNum < -32678) || (nConvNum > 32767))
	{
		return FALSE;
	}

	/* Return false is there is any part of the string that was not converted */
	return *sPointerToRest == '\0';
}


int checkRegister(const char *StringToCheck)
{
	/* Check if the string is null or its length is zero */
	if (StringToCheck == NULL || *StringToCheck == '\0')
	{
		return FALSE;
	}

	/* Check if the string is one of the following: r0-7 */
	return ((*StringToCheck == 'r') && isdigit(*(StringToCheck+1)) && (*(StringToCheck+1) != '9') && 
		(*(StringToCheck+1) != '8') && (*(StringToCheck+2) == '\0'));
}

int checkDirective (const char *StringToCheck)
{
    /* Check if the string is null or its length is zero */
	if (StringToCheck == NULL || *StringToCheck == '\0')
	{
		return FALSE;
	}
	else if (strcmp(StringToCheck, "data")==0 || strcmp(StringToCheck, "entry")==0 ||strcmp(StringToCheck, "extern")==0
          ||strcmp(StringToCheck, "define")==0 || strcmp(StringToCheck, "string")==0 )
        return TRUE ;
    /* Not directive command */
    return FALSE ;
}






