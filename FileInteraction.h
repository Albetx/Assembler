#ifndef FILEINTERACTION_H
#define FILEINTERACTION_H

#include <stdio.h>
#include <string.h>
#include "BinaryStructsAndDefMacros.h"

#define OBJ_EXTENTION		".ob"
#define ENT_EXTENTION		".ent"
#define EXT_EXTENTION		".ext"
#define FILE_DELETE_ERR		-1
#define READ			"r"
#define WRITE			"w"
#define APPEND			"a"


/* Stores all the files we will use in the program */
typedef struct
{
	FILE *fpCodeFile;		/* *.ob */
	FILE *fpEntryFile;		/* *.ent */
	FILE *fpExternalFile;		/* *.ext */
}outputFiles;


/*	Adds extension to the file name
	sFileFullName 		- full file name, after the extension.
	sFileName 		- original file name.
	sExtention 		- extension to add.
	output :
	A pointer to the new file
*/
char *addFileFormat(char *sFileFullName, char *sFileName, char *sExtention);


/*  	Open's files needed for the output
    	files - pointer to files
	output :
	RC 0 for valid run, a different number if an error occurred
*/
int OpenFiles(char *fileName, outputFiles* files);


/*  	Close files if they were open
    	files - pointer to files
*/
void CloseFiles(outputFiles* files);


/*  	Delete empty files
    	files - pointer to files
	output :
	RC 0 for valid run, a different number if an error occurred
*/
int DeleteEmptyFiles(char* fileName, outputFiles* files);






#endif 
