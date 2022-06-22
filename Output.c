#include "Output.h"


void writeFileFirstLine(FILE *filePointer, int nCodeSize, int nDataSize)
{
	char strLine[sizeof(int)*BYTE_SIZE*2 + LEN_OF_TAB + END_OF_STRING_SIGN]; /* Length for 2 int values + tab + space*/

	sprintf(strLine, "\t%d %d\n", nCodeSize, nDataSize);

	/* Write line to file */
	fputs(strLine, filePointer);
}


void writeCode(FILE *filePointer,int nStartAddress,codeSegment (*codeSegment),int IC)
{
	int codeCounter;
	char strLine[BASE_4_OFFSET + ADDRESS_DEC + 2*SPACE + END_OF_STRING_SIGN]; /* Length for address and code + 2 spaces */
	char stringOfData[BASE_4_OFFSET + 1];
	int finalAddress;

	for (codeCounter = 0; codeCounter < IC; codeCounter++)
	{
		finalAddress = nStartAddress + codeCounter;

		WriteInBase4(stringOfData, codeSegment[codeCounter].final.code,BASE_4_OFFSET);

		if (finalAddress < THREE_DIGIT_MAX) /*Pad with 0 if needed */
            		sprintf(strLine, "0%d  %s\n", finalAddress, stringOfData);
        	else
            		sprintf(strLine, "%d  %s\n", finalAddress, stringOfData);

		/* Write line to file */
		fputs(strLine, filePointer);
	}
}


void writeData(FILE *filePointer,int nStartAddress,int *dataSegment,int DC)
{

	int dataCounter;
	char strLine[BASE_4_OFFSET + ADDRESS_DEC + 2*SPACE + END_OF_STRING_SIGN]; /* Length for address and code + 2 spaces */
	char stringOfData[BASE_4_OFFSET + END_OF_STRING_SIGN];
	int finalAddress;

	for (dataCounter = 0; dataCounter < DC; dataCounter++)
	{
		finalAddress = nStartAddress + dataCounter;

		WriteInBase4(stringOfData, dataSegment[dataCounter],BASE_4_OFFSET);

		if (finalAddress < THREE_DIGIT_MAX) /*Pad with 0 if needed */
            		sprintf(strLine, "0%d  %s\n", finalAddress, stringOfData);
        	else
            		sprintf(strLine, "%d  %s\n", finalAddress, stringOfData);

        	/* Write line to file */
		fputs(strLine, filePointer);
	}
}


void WriteInBase4(char* stringToWrite, int nData,int nPad)
{
	int CounterOfDight;
	bitField unsignedNumber;
	unsigned int digit;
	int lastCharInString = nPad-1 ;

	/* convert data to unsgined */
	unsignedNumber.value = 0;
	unsignedNumber.value |= nData;

	CounterOfDight = 0;

	/* generate digits in reverse order */
	do {
		/* Extract digit */
		digit = unsignedNumber.value % BASE;
                 switch(digit)
		{
		case 0: stringToWrite[lastCharInString--] ='*'; CounterOfDight++ ; break;
		case 1: stringToWrite[lastCharInString--] ='#'; CounterOfDight++ ; break;
		case 2: stringToWrite[lastCharInString--] ='%'; CounterOfDight++ ; break;
		case 3: stringToWrite[lastCharInString--] ='!'; CounterOfDight++ ; break;

		break;
		}

		}
	/* Proceed to next digit */
	while ((unsignedNumber.value /= BASE) > 0);

	/* Add padding zeros */
	while (CounterOfDight < (nPad))
	{
		stringToWrite[lastCharInString--] = '*';
		CounterOfDight++ ;
	}

	stringToWrite[CounterOfDight] = '\0';

}


void writeEntriesExternals(FILE *filePointer,SymbolTable stSymbolTable)
{
	struct symTabNode *currElement;
    	currElement = stSymbolTable;

    	while (currElement != NULL)
	    {
		writeEntriesExternalsToFile(filePointer,currElement->sName,currElement->nAddress);
		/* Go to the next element */
		currElement = currElement->stNext;
	    }
}

void writeEntriesExternalsToFile(FILE *filePointer, char* strEntryExternLabel, int sAddress)
{
	char strLine[MAX_LEN];


    if (sAddress < THREE_DIGIT_MAX) /*Pad with 0 if needed */
        sprintf(strLine, "%s\t0%d\n", strEntryExternLabel, sAddress);
    else
    {
        sprintf(strLine, "%s\t%d\n", strEntryExternLabel, sAddress);

    }

	/* Write line to file */
	fputs(strLine, filePointer);
}



