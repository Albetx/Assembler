#include "FileInteraction.h"



char *addFileFormat(char *sFileFullName, char *sFileName, char *sExtention)
{
	char* sFilePointer = NULL;

	sFilePointer = strcpy(sFileFullName, sFileName);

	if (sFilePointer != NULL)
	{
		/* Get a pointer to the new string */
		sFilePointer = strcat(sFileFullName, sExtention);
	}

	return sFilePointer;
}



int OpenFiles(char *fileName, outputFiles* files)
{
	int  checkError = 0;
	char strNewName[MAX_LEN];

	addFileFormat(strNewName, fileName, OBJ_EXTENTION);
	if ((files->fpCodeFile = fopen(strNewName, WRITE)) == NULL)
	{
		checkError++;
	}


	addFileFormat(strNewName, fileName, ENT_EXTENTION);
	if ((files->fpEntryFile = fopen(strNewName, WRITE)) == NULL)
	{
		checkError++;
	}

	addFileFormat(strNewName, fileName, EXT_EXTENTION);
	if ((files->fpExternalFile = fopen(strNewName, WRITE)) == NULL)
	{
		checkError++;
	}


	if (checkError == 0)
	{
		return TRUE;
	}
	else
	{
		CloseFiles(files);
		return FALSE;
	}
}

/*
 Closes all files, in case they are open.
*/
void CloseFiles(outputFiles* files)
{
	if (files->fpCodeFile != NULL)
	{
		fclose(files->fpCodeFile);
		files->fpCodeFile = NULL;
	}
	if (files->fpEntryFile != NULL)
	{
		fclose(files->fpEntryFile);
		files->fpEntryFile = NULL;
	}
	if (files->fpExternalFile != NULL)
	{
		fclose(files->fpExternalFile);
		files->fpExternalFile = NULL;
	}
}

/*
 This function deletes empty entry and external files.
*/
int DeleteEmptyFiles(char* fileName, outputFiles* files)
{

	int nError = 0;
	char strNewName[MAX_LEN];

	if (ftell(files->fpEntryFile) == 0)
	{
		fclose(files->fpEntryFile);
		files->fpEntryFile = NULL;

		/* Create the string for the entities file name (*.ent) */
		addFileFormat(strNewName, fileName, ENT_EXTENTION);

		if (remove(strNewName) == FILE_DELETE_ERR)
		{
			fprintf(stderr,	"Failed in deleting the file: \"%s\".", strNewName);
			nError++;
		}
	}

	if (ftell(files->fpExternalFile) == 0)
	{

		fclose(files->fpExternalFile);
		files->fpExternalFile = NULL;

		/* Create the string for the externals file name (*.ext) */
		addFileFormat(strNewName, fileName, EXT_EXTENTION);

		if (remove(strNewName) == FILE_DELETE_ERR)
		{
			fprintf(stderr,	"Failed in deleting the file: \"%s\".", strNewName);
			nError++;
		}
	}

	return nError;
}
