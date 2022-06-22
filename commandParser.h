#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BinaryStructsAndDefMacros.h"
#include "instructionCompile.h"


#define	SIGN_OF_COMMENT		';'
#define	SIGN_OF_END_LABEL	':'
#define SIGN_OF_END_MACRO   	'='
#define	DATA_LABEL		0
#define	CODE_LABEL		1
#define	EXTERN_LABEL		2
#define ENRTY_LABEL		3
#define MACRO_LABEL     	4
#define PROGRAM_LOAD_ADDRESS	100





/*  Checks if the current line is empty
    output: TRUE if line is empty, FALSE otherwise
*/
int checkIfEmptyLine(char *LineString);

/*	Extracts a label from a line
	output: return 0 for valid run, a different number if an error occurred
*/
int	getLabel(char *LineString, char *StringLabel, int *PointerInLine);

/*	Moves pointer to next non-white space char
*/
void GoToNextParam(char *LineString, int *PointerInLine);

/*	Checks if a string is a valid label
	output: TRUE if string is a valid label, FALSE otherwise
*/
int checkLabel(const char *StringToCheck);

/*	Extracts the macro name from the line
    StringMacro - holds the macro name
	return 0 for valid run, a different number if an error occurred
*/
int	getMacroName(char *LineString, char *StringMacro, int *PointerInLine);

/*	Extracts the macro value from the line
    MacroVal - holds the macro value
	return 0 for valid run, a different number if an error occurred
*/
int	getMacroVal(char *LineString, int *MacroVal, int *PointerInLine);

/*	Extracts an instruction from the line
	stringCommand - holds the command name
*/
void getInstruction(char *LineString, char *stringCommand, int *PointerInLine);

/*	Moves pointer to end of instruction metadata in order to get next parameter
*/
void goOver(char *LineString, int *PointerInLine);

/*	Get next parameter from line
	sParam 	- parameter to return.
	If no parameter found - parameter set to a null string
*/
void getNextParam(char *LineString, int *PointerInLine,char *sParam);

/*	Checks if a string is a valid number
	output: TRUE if string is a numeric, FALSE otherwise
*/
int checkNumber(const char *StringToCheck);

/*	Checks if a string is a valid register
	output: TRUE if string is a register, FALSE otherwise
*/
int checkRegister(const char *StringToCheck);

/*	Checks if a string is a saved word for directive command
	output: TRUE if it is, FALSE otherwise
*/
int checkDirective (const char *StringToCheck);


#endif
