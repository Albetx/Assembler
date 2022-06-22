#include "instructionCompile.h"

/* instruction data: index 0: num of operands
                     index 1-4: legal addressing type for first operand
                     index 5-8: legal addressing type for second operand */
static int instructionData [NUM_OF_COMMANDS][9] =
                                {{2,1,1,1,1,0,1,1,1},  		/*mov*/
				{2,1,1,1,1,1,1,1,1},		/*cmp*/
				{2,1,1,1,1,0,1,1,1},		/*add*/
				{2,1,1,1,1,0,1,1,1},		/*sub*/
				{1,0,0,0,0,0,1,1,1},		/*not*/
				{1,0,0,0,0,0,1,1,1},		/*clr*/
				{2,0,1,1,0,0,1,1,1},		/*lea*/
				{1,0,0,0,0,0,1,1,1},		/*inc*/
				{1,0,0,0,0,0,1,1,1},		/*dec*/
				{1,0,0,0,0,0,1,0,1},		/*jmp*/
				{1,0,0,0,0,0,1,0,1},		/*bne*/
				{1,0,0,0,0,0,1,1,1},		/*red*/
				{1,0,0,0,0,1,1,1,1},		/*prn*/
				{1,0,0,0,0,0,1,0,1},		/*jsr*/
				{0,0,0,0,0,0,0,0,0},		/*rts*/
                                {0,0,0,0,0,0,0,0,0}};		/*stop*/

static char * nameOfInstruction[NUM_OF_COMMANDS] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

int instructionIndex(const char *StringToCheck)
{
	int InstructionCounter;
	int binCommandFound= FALSE;
	int nCommandNum = -1;

	if ((StringToCheck != NULL) && (strlen(StringToCheck) >= 0))
	{
		for (InstructionCounter=0; InstructionCounter<NUM_OF_COMMANDS && (!binCommandFound); InstructionCounter++)
		{
			if (strcmp(StringToCheck,nameOfInstruction[InstructionCounter]) == 0)
			{
				nCommandNum = InstructionCounter;
				binCommandFound = TRUE;
			}
		}
	}
	/* Return command number or -1 if error occurred */
	return nCommandNum;
}



int AddressNumber(const char *StringToCheck)
{

	char sTempString1[MAX_LEN];
	char sTempString2[MAX_LEN];
	char *sTemp1;
	char *sTemp2;
	int lastChar = 0 ;

	if (StringToCheck == NULL || *StringToCheck == '\0')
        	return ILLEGAL_ADDRESSING;

	/* Get last character index */
	while (*(StringToCheck+1) != '\0') {
            lastChar++ ;
            StringToCheck++ ;
	}

	/* Return to first character */
	StringToCheck -= lastChar ;

	/* Copy original string into to two other work strings */
	strcpy(sTempString1, StringToCheck);
	strcpy(sTempString2, StringToCheck);

	/*extract operands*/
	if(  *(StringToCheck+lastChar)==']' )
	{
		sTemp1=strtok(sTempString1,"[");
		sTemp2=strtok(sTempString2,"[");
		sTemp2=strtok(NULL,"]");
			if((checkLabel(sTemp1)) && ( (checkLabel(sTemp2)) || checkNumber(sTemp2) ))
			{
				return CONSTANT_ADDRESSING;
			}	
	}

	else if (((*StringToCheck) == '#') && ((checkNumber(StringToCheck+1)) || checkLabel(StringToCheck+1)) )
	{
		return IMMEDIATE_ADDRESSING;
	}

	else if (checkLabel(StringToCheck))
	{
		return DIRECT_ADDRESSING;
	}

	else if (checkRegister(StringToCheck))
	{
		return REGISTER_ADDRESSING;
	}

	/* If we got here, no valid ADM found */
	return ILLEGAL_ADDRESSING;
}


int	instructionSize(char *LineString, int PointerInLine, char *sInstruction)
{
	int	ErrorOn 		= FALSE;
	int	nInsNumber;
	int	numberOfOperands;
	int	opAdressingMethod1;
	int	opAdressingMethod2;
	char 	operand[MAX_LEN];
   	int 	nSize=0;
	/* Increase IC by 1 (instruction minimum length)*/
	nSize++;

	/* Get instruction number */
	nInsNumber = instructionIndex(sInstruction);

	numberOfOperands = instructionData[nInsNumber][0];

	if (numberOfOperands != 0)
	{
		/* Read first operand */
		getNextParam(LineString, &PointerInLine, operand);

		if (numberOfOperands == 1)
		{
			if (strcmp(operand, "") != 0)
			{
				opAdressingMethod1 = AddressNumber(operand);

				if (opAdressingMethod1 != ILLEGAL_ADDRESSING)
				{
					/* Check if addressing method is permitted for operand in current instruction */
					if (instructionData[nInsNumber][5+opAdressingMethod1] == 1)
					{
						/* Check addressing method*/
						if ((opAdressingMethod1 == IMMEDIATE_ADDRESSING) || (opAdressingMethod1 == DIRECT_ADDRESSING)
                       					 || (opAdressingMethod1==REGISTER_ADDRESSING))
						{
							nSize++;
						}
                        			else if ((opAdressingMethod1==CONSTANT_ADDRESSING))
                               				 nSize+=2;
					}
					/* Addressing method is not permitted */
					else
					{
						ErrorOn = TRUE;
					}
				}
				/* Addressing method is not legal */
				else
				{
					ErrorOn = TRUE;
				}
			}
			/* Operand is empty*/
			else
			{
				/* An operand is missing */
				ErrorOn = TRUE;
			}
		}

		else if (numberOfOperands == 2)
		{
			if (strcmp(operand, "") != 0)
			{
				opAdressingMethod1 = AddressNumber(operand);

				if (opAdressingMethod1 != ILLEGAL_ADDRESSING)
				{
					/* Check if addressing method is permitted for operand in current instruction */
					if (instructionData[nInsNumber][opAdressingMethod1] == 1)
					{
						/* Check  addressing method*/
						if ((opAdressingMethod1 == IMMEDIATE_ADDRESSING) || (opAdressingMethod1 == DIRECT_ADDRESSING)
                            				|| (opAdressingMethod1==REGISTER_ADDRESSING))
						{
							nSize++;
						}
                        			else if ((opAdressingMethod1==CONSTANT_ADDRESSING))
                               				 nSize+=2;
					}
					/* Addressing method is not permitted */
					else
					{
						ErrorOn = TRUE;
					}
				}
				/* Addressing method is not legal */
				else
				{
					ErrorOn = TRUE;
				}
			}
			/* Operand is empty */
			else
			{
				/* An operand is missing */
				ErrorOn = TRUE;
			}

			/* Check if there is a comma sign */
			if (LineString[PointerInLine] == ',')
			{
				PointerInLine++; /* skip comma */

				getNextParam(LineString, &PointerInLine, operand);

				if (strcmp(operand, "") != 0)
				{
					opAdressingMethod2 = AddressNumber(operand);

					if (opAdressingMethod2 != ILLEGAL_ADDRESSING)
					{
						/* Check if addressing method is permitted for operand in current instruction */
						if (instructionData[nInsNumber][5+opAdressingMethod2] == 1)
						{
							/* Check addressing method */
							if ((opAdressingMethod2 == IMMEDIATE_ADDRESSING) || (opAdressingMethod2 == DIRECT_ADDRESSING)||
								 (opAdressingMethod2==REGISTER_ADDRESSING && opAdressingMethod1!=REGISTER_ADDRESSING))
								{
									nSize++;
								}

                            				else if ((opAdressingMethod2==CONSTANT_ADDRESSING))
                                				nSize+=2;
						}
						/* Addressing method is not permitted */
						else
						{
							ErrorOn = TRUE;
						}
					}
					/* Addressing method is not legal */
					else
					{
						ErrorOn = TRUE;
					}
				}
				/* Operand is empty */
				else
				{
					/* An operand is missing */
					ErrorOn = TRUE;
				}
			}
			/* If there is no operand seperator */
			else
			{
				/* An operand is missing */
				ErrorOn = TRUE;
			}
		}
	}

        /* Not end of the line */
	if (LineString[PointerInLine] != '\n')
	{
			ErrorOn = TRUE;
	}

	if(ErrorOn==TRUE)
	 	return -1;
	else
		return nSize;
}



int	Increase_IC(char *LineString, int *PointerInLine, char *sInstruction, int *IC, char *sFileFullName,int numberOfLine)
{
	int	ErrorOn 	= FALSE;
	int	bIsOpMissing 	= FALSE;
	int	nInsNumber;
	int	numberOfOperands;
	int	opAdressingMethod1;
	int	opAdressingMethod2;
	char 	operand[MAX_LEN];

	/* Increase IC by 1 (instruction minimum length)*/
	(*IC)++;

	/* Get instruction number */
	nInsNumber = instructionIndex(sInstruction);

	numberOfOperands = instructionData[nInsNumber][0];

	if (numberOfOperands != 0)
	{
		getNextParam(LineString, PointerInLine, operand);

		if (numberOfOperands == 1)
		{
			if (strcmp(operand, "") != 0)
			{
				opAdressingMethod1 = AddressNumber(operand);

				if (opAdressingMethod1 != ILLEGAL_ADDRESSING)
				{
					/* Check if addressing method is permitted for operand in current instruction */
					if (instructionData[nInsNumber][5+opAdressingMethod1] == 1)
					{
						/* Check addressing method */
						if ((opAdressingMethod1 == IMMEDIATE_ADDRESSING) || (opAdressingMethod1 == DIRECT_ADDRESSING)||
                             				(opAdressingMethod1==REGISTER_ADDRESSING))
						{
							(*IC)++;
						}
                        			else if ((opAdressingMethod1==CONSTANT_ADDRESSING))
                            				(*IC)+=2;
					}
					/* Addressing method is not permitted */
					else
					{
						fprintf(stderr, "%s:%d: ERR: Addressing method not permitted for dest operand.\n", sFileFullName, numberOfLine);
						ErrorOn = TRUE;
					}
				}
				/* Addressing method is not legal */
				else
				{
					fprintf(stderr,"%s:%d: ERR: Illegal addressing method for dest operand.\n", sFileFullName, numberOfLine);
					ErrorOn = TRUE;
				}
			}
			/* If operand is empty*/
			else
			{
				/* An operand is missing */
				bIsOpMissing = TRUE;
				ErrorOn = TRUE;
			}

		}
		else if (numberOfOperands == 2)
		{
			if (strcmp(operand, "") != 0)
			{
				opAdressingMethod1 = AddressNumber(operand);

				if (opAdressingMethod1 != ILLEGAL_ADDRESSING)
				{
					/* Check if addressing method is permitted for operand in current instruction */
					if (instructionData[nInsNumber][opAdressingMethod1] == 1)
					{
						/* Check addressing method */
						if ((opAdressingMethod1 == IMMEDIATE_ADDRESSING) || (opAdressingMethod1 == DIRECT_ADDRESSING)
                            				|| (opAdressingMethod1==REGISTER_ADDRESSING))
						{
							(*IC)++;
						}
                        			else if ((opAdressingMethod1==CONSTANT_ADDRESSING))
                            				(*IC)+=2;
					}
					/* Addressing method is not permitted */
					else
					{
						fprintf(stderr,"%s:%d: ERR: Addressing method not permitted for source operand.\n", sFileFullName, numberOfLine);
						ErrorOn = TRUE;
					}
				}
				/* Addressing method is not legal */
				else
				{
					fprintf(stderr, "%s:%d: ERR: Illegal addressing method for source operand.\n", sFileFullName, numberOfLine);
					ErrorOn = TRUE;
				}
			}
			/* Operand is empty */
			else
			{
				/* An operand is missing */
				bIsOpMissing = TRUE;
				ErrorOn = TRUE;
			}

			/* Check if there is an operand seperator sign */
			if (LineString[*PointerInLine] == ',')
			{
				(*PointerInLine)++;

				getNextParam(LineString, PointerInLine, operand);

				if (strcmp(operand, "") != 0)
				{
					opAdressingMethod2 = AddressNumber(operand);

					if (opAdressingMethod2 != ILLEGAL_ADDRESSING)
					{
						/* Check if addressing method is permitted for operand in current instruction */
						if (instructionData[nInsNumber][5+opAdressingMethod2] == 1)
						{
							/* Check  addressing method  */
							if ((opAdressingMethod2 == IMMEDIATE_ADDRESSING) || (opAdressingMethod2 == DIRECT_ADDRESSING)||
								 (opAdressingMethod2==REGISTER_ADDRESSING && opAdressingMethod1!=REGISTER_ADDRESSING))
							{
								(*IC)++;
							}
                            				else if ((opAdressingMethod2==CONSTANT_ADDRESSING))
                                				(*IC)+=2;
						}
						/* Addressing method is not permitted */
						else
						{
							fprintf(stderr,"%s:%d: ERR: Addressing method not permitted for dest operand.\n", sFileFullName, numberOfLine);
							ErrorOn = TRUE;
						}
					}
					/* Addressing method is not legal */
					else
					{
						fprintf(stderr,"%s:%d: ERR: Illegal addressing method for dest operand.\n", sFileFullName, numberOfLine);
						ErrorOn = TRUE;
					}
				}
				/* Operand is empty */
				else
				{
					/* An operand is missing */
					bIsOpMissing = TRUE;
					ErrorOn = TRUE;
				}
			}
			/* If there is no comma between operands */
			else
			{
				bIsOpMissing = TRUE;
				ErrorOn = TRUE;
			}
		}
	}

	/* Check if there operand missing */
	if (bIsOpMissing)
	{
		fprintf(stderr, "%s:%d: ERR: Too few operands for instruction.\n", sFileFullName, numberOfLine);
	}

	/* Check if not end of line sign */
	if (LineString[*PointerInLine] != '\n')
	{

        	/* Check if instruction doesn't get operands  */
		if (numberOfOperands != 0)
		{
			fprintf(stderr, "%s:%d: ERR: End of line expected after \"%s\".\n", sFileFullName, numberOfLine, operand);
			ErrorOn = TRUE;
		}
		/* If it does get operands */
		else
		{
			fprintf(stderr, "%s:%d: ERR: End of line expected after \"%s\".\n", sFileFullName, numberOfLine, sInstruction);
			ErrorOn = TRUE;
		}
	}

	/* Return error indication */
	return 	ErrorOn;
}



int	TranslateInstruction(char *LineString, int *PointerInLine, char *sInstruction, int *IC, int codeSegmentLength, int *dataSegment,
        codeSegment (*codeSegment),SymbolTable symTable, SymbolTable *stExternalSymbolTable, char *sFileFullName,int numberOfLine)
{

	int ErrorOn = FALSE;
	Instruction	bnInstruction = {0};
	int	nInstructionIndex;
	int	op1TypeAddress;
	int	op2TypeAddress;
	int	numberOfOperands;
	int operandType= DESTENETION;
	char operand1[MAX_LEN] = "";
	char operand2[MAX_LEN] = "";
	char operand1Cpy[MAX_LEN] = "";
	char operand2Cpy[MAX_LEN] = "";


	numberOfOperands = instructionData[instructionIndex(sInstruction)][0];
	bnInstruction.OpCode = instructionIndex(sInstruction);

	/* Read first operand */
	getNextParam(LineString, PointerInLine, operand1);

	if (LineString[*PointerInLine] == ',')
	{
		(*PointerInLine)++;
		getNextParam(LineString, PointerInLine, operand2);
	}

	/* Check if there is only one operand */
	if ((*operand2 == '\0') && (*operand1 != '\0'))
	{
		/* Its the dest operand */
		strcpy(operand2, operand1);
		*operand1 = '\0';
	}

	op1TypeAddress = AddressNumber(operand1);

	if (op1TypeAddress != ILLEGAL_ADDRESSING)
	{
		bnInstruction.SOURSE=op1TypeAddress;
	}

	op2TypeAddress = AddressNumber(operand2);

	if (op2TypeAddress != ILLEGAL_ADDRESSING)
	{
		/* Set instruction dest addressing method */
		bnInstruction.DES=op2TypeAddress;
	}

	/* Append code segment with instruction, save instruction index, and increase IC */
	codeSegment[*IC].csInst 	= bnInstruction;
	codeSegment[*IC].csInst.ARE 	= ABSOLUTE;
	nInstructionIndex 		= *IC;
	(*IC)++;

	if (numberOfOperands == 1)
	{
		strcpy(operand2Cpy,operand2);
		/* Add operand extra words to code segment */
		ErrorOn+=TranslateExtraWord(operand2Cpy, op2TypeAddress, IC, codeSegmentLength, nInstructionIndex, dataSegment, codeSegment,
					symTable, stExternalSymbolTable, sFileFullName, numberOfLine,operandType);
	}
	/* Check if instruction gets two operands */
	else if (numberOfOperands == 2)
	{	/* if both operands are registers*/
    		if( op2TypeAddress==REGISTER_ADDRESSING && op1TypeAddress==REGISTER_ADDRESSING )
		{
			codeSegment[*IC].rgCode.leftCode = atoi(operand1+1);
			codeSegment[*IC].rgCode.rightCode = atoi(operand2+1);
			codeSegment[*IC].rgCode.ARE = ABSOLUTE;
			(*IC)++;

		}
		/* Any other case with 2 operands */
		else
		{
			strcpy(operand1Cpy,operand1);
			strcpy(operand2Cpy,operand2);
			operandType=SOURCE;
			ErrorOn += TranslateExtraWord(operand1Cpy, op1TypeAddress, IC, codeSegmentLength, nInstructionIndex, dataSegment, codeSegment,
						symTable, stExternalSymbolTable, sFileFullName, numberOfLine,operandType);

			/* Add operand extra words to code segment */
			operandType=DESTENETION;
			ErrorOn += (TranslateExtraWord(operand2Cpy, op2TypeAddress, IC, codeSegmentLength, nInstructionIndex, dataSegment,
	                codeSegment,symTable, stExternalSymbolTable, sFileFullName, numberOfLine,operandType));
		}
	}


	if(ErrorOn>0)
		return TRUE;
	else return FALSE;

}

int	TranslateExtraWord(char *operand, int nOpAddMeth, int *IC, int codeSegmentLength, int nInstructionIndex, int* dataSegment, codeSegment (*codeSegment),
				  SymbolTable symTable, SymbolTable *stExternalSymbolTable, char *sFileFullName, int numberOfLine,int operandType)
{
	int	ErrorOn = FALSE;
	int 	arrIndex;   /*Index number in the array, can be numeric or macro*/
	char 	*temp1 = NULL;
	char 	*temp2 = NULL;
	int 	lastChar = 0 ; /* Holds the last index of the operand string */
	char 	sTempString1[MAX_LEN];
	char	sTempString2[MAX_LEN];

	struct symTabNode *currNode = NULL;

	if (nOpAddMeth == IMMEDIATE_ADDRESSING)
	{
	    if (checkNumber (operand+1)) {
		/* Append code segment with immediate value */
		codeSegment[*IC].csAddress.nCodeAdd = atoi(operand+1);
	    }

	    /* Operand is a macro value, get its value and append code segment */
	    else  {
		    currNode=NULL;
		    currNode = getSymbol(symTable,operand+1);
		    if (currNode!=NULL)
		        codeSegment[*IC].csAddress.nCodeAdd = currNode->nAddress;

		    else {
			    fprintf(stderr, "%s:%d: ERR: Cannot resolve CONSTANT_ADDRESSING - bad macro name.\n", sFileFullName, numberOfLine);
			    ErrorOn=TRUE;
		    }
	    }
		codeSegment[*IC].csAddress.ARE = ABSOLUTE;
        	/* Increase IC */
		(*IC)++;
	}
	else if (nOpAddMeth == DIRECT_ADDRESSING)
	{
			/* Append code segment with label and save external symbol reference if needed */
			ErrorOn += TranslateLabeledData(operand, symTable, stExternalSymbolTable, codeSegment, codeSegmentLength, IC,sFileFullName, numberOfLine);
	}

	else if(nOpAddMeth==REGISTER_ADDRESSING)
	{
		if(operandType==DESTENETION)
		{
			codeSegment[*IC].rgCode.rightCode=atoi(operand+1);
			codeSegment[*IC].rgCode.ARE=ABSOLUTE;
			(*IC)++;
		}
		else if(operandType==SOURCE)
		{
			codeSegment[*IC].rgCode.leftCode=atoi(operand+1);
			codeSegment[*IC].rgCode.ARE=ABSOLUTE;
			(*IC)++;
		}
	}

	else if(nOpAddMeth == CONSTANT_ADDRESSING)
	{
		/* Get last character index */
		while (*(operand+1) != '\0') {
		        lastChar++ ;
		        operand++ ;
        }

        /* Return to first character */
        operand -= lastChar ;

        /* Copy original string into to two other work strings */
        strcpy(sTempString1, operand);
        strcpy(sTempString2, operand);

        /*extract operands*/
        temp1=strtok(sTempString1,"[");
        temp2=strtok(sTempString2,"[");
        temp2=strtok(NULL,"]");

        /* Check inside braces operand */
        currNode=NULL;
        currNode = getSymbol(symTable,temp2);

        /* If the value is a macro that already been declared*/
        if (currNode != NULL)
        {
            /* Check if the symbol is of a type - macro */
            if ((currNode->nType) == MACRO_LABEL)
                arrIndex = currNode->nAddress;
            else {
	        fprintf(stderr, "%s:%d: ERR: Cannot resolve CONSTANT_ADDRESSING, \"%s\" undefined.\n.\n", sFileFullName, numberOfLine, temp2);
	        ErrorOn=TRUE;
            }
        }

        /* If the value is numeric */
        else if (checkNumber(temp2))
            arrIndex = atoi(temp2) ;

        /* Bad value */
        else {
            fprintf(stderr, "%s:%d: ERR: Cannot resolve CONSTANT_ADDRESSING, \"%s\" undefined.\n", sFileFullName, numberOfLine, temp2);
            ErrorOn=TRUE;
        }

        /* Append code segment with label and save external symbol reference if needed */
        ErrorOn += TranslateLabeledData(temp1, symTable, stExternalSymbolTable, codeSegment, codeSegmentLength, IC, sFileFullName, numberOfLine);
        /* insert the result to code segment*/
        codeSegment[*IC].csAddress.nCodeAdd = arrIndex;
        codeSegment[*IC].csAddress.ARE = ABSOLUTE;
        (*IC)++;
        }

	if(ErrorOn>0)
		return TRUE;
	else
		return FALSE;
}

int TranslateLabeledData(char *operand, SymbolTable symTable, SymbolTable *stExternalSymbolTable, codeSegment (*codeSegment),
			 int codeSegmentLength, int *IC, char *sFileFullName, int numberOfLine)
{
	int	ErrorOn = 0;
	struct	symTabNode *stnCurrSymbol = NULL;

	stnCurrSymbol = getSymbol(symTable, operand);

	if (stnCurrSymbol != NULL)
	{
		if ((stnCurrSymbol->nType) == DATA_LABEL)
		{
			/* Update code segment with symbol address */
			codeSegment[*IC].csAddress.nCodeAdd = (stnCurrSymbol->nAddress) + codeSegmentLength + PROGRAM_LOAD_ADDRESS;
			codeSegment[*IC].csAddress.ARE = RELOCATABLE;
		}
		else if ((stnCurrSymbol->nType) == CODE_LABEL)
		{
			/* Update code segment with symbol address */
			codeSegment[*IC].csAddress.nCodeAdd = (stnCurrSymbol->nAddress) + PROGRAM_LOAD_ADDRESS;
			codeSegment[*IC].csAddress.ARE = RELOCATABLE;
		}
		/* If the symbol is of a type - extern */
		else
		{
			/* Insert symbol to external symbol table and append code segment with zeros */
			insertSymbol(stExternalSymbolTable, operand, *IC + PROGRAM_LOAD_ADDRESS, ALLOW_DUPLICATE_LABELS, DEFAULT_SIZE);
			codeSegment[*IC].csAddress.nCodeAdd = 0;
			codeSegment[*IC].csAddress.ARE = EXTERNAL;
		}
		/* Increase IC */
		(*IC)++;
	}
	/* If a symbol wasn't found */
	else
	{
		fprintf(stderr, "%s:%d: ERR: Label \"%s\" not found.\n", sFileFullName, numberOfLine, operand);
		ErrorOn = TRUE;
	}

	/* Return error indication */
	return ErrorOn;

}



