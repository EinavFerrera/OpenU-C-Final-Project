#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structures.h"
#include "secondPass.h"
#include "binaryBuild.h"

void secPass(char *fileName, gNode *hRow, gNode *hSuspectLabel, gNode *hSymbol, gNode *hEntryExtern, int IC, int DC)
{
	/***********************declaration for the proccess************************/
	char filePath[MAXIMUM_FILE_NAME];
	FILE *binaryFile, *entFile, *extFile;
	gNode temp;
	int count = 0;


	/***************************************************************************/

	/***checks if there is undeclared labels but written inside instrucions***/

	cmpListDeleteSameName(hSymbol, hSuspectLabel);

	cmpListCpyDeleteSameName(hEntryExtern, hSuspectLabel, hSymbol);

	if (getName((gNode)hSuspectLabel) != NULL)
	{
		printf("ERROR: Invalid labels was founded - the next labels are not declare properly\n");
		printLabels(*hSuspectLabel);
		return;
	}
	/***************************************************************************/

	updateEntExtInSymbols(hEntryExtern, hSymbol);

	/**************************create an ent file*******************************/
	temp = *hEntryExtern;
	while (temp != NULL)
	{
		if (getType(temp) == ENTRY)
			count++;
		temp = (gNode)getNext(temp);
	}
	if (count > 0)
	{
		count = 0;
		sprintf(filePath, "%s.ent", fileName);
		entFile = fopen(filePath, "w");

		count = createEntFile(*hEntryExtern, *hSymbol, entFile);

		fclose(entFile);
	}
	/***************************************************************************/

	/**************************create an ext file*******************************/

	temp = *hEntryExtern;
	while (temp != NULL)
	{

		if (getType(temp) == EXT)
		{
			if (search(hSymbol, getName(temp)) != NULL)
			{
				count++;
			}
		}
		temp = (gNode)getNext(temp);
	}

	if (count > 0)
	{
		count = 0;
		sprintf(filePath, "%s.ext", fileName);
		extFile = fopen(filePath, "w");
		count = createExtFile(*hEntryExtern, *hSymbol, extFile);
		fclose(extFile);
	}

	/***************************************************************************/

	/**************************create an ob file********************************/
	sprintf(filePath, "%s.ob", fileName);
	binaryFile = fopen(filePath, "w");
	fprintf(binaryFile, "%d\t%d\n", IC, DC);
	binaryCode(binaryFile, *hRow, *hSymbol, IC);
	fclose(binaryFile);

	/***************************************************************************/

	printf("\nThe Second pass phase has been completed successfully for file: %s\n\nAll the process has been finished without errors!!!", filePath);
	printf("\n******************************************************************************************\n");
}

int createEntFile(gNode hEntryExtern, gNode hSymbols, FILE *entFile)
{
	if (hEntryExtern == NULL)
	{
		printf("\tent file was created successfully\n");
		return 0;
	}
	if (getType(hEntryExtern) == ENTRY)
	{
		if (search(&hSymbols, getName(hEntryExtern)) != NULL)
		{
			fprintf(entFile, "%s\t%d\n", getName(hEntryExtern), 100 + getAddress(search(&hSymbols, getName(hEntryExtern))));
		}
	}
	createEntFile((gNode)getNext(hEntryExtern), hSymbols, entFile);
	return 0;
}

int createExtFile(gNode hEntryExtern, gNode hSymbols, FILE *extFile)
{
	gNode tmp;
	if (hSymbols == NULL)
	{
		printf("\text file was created successfully\n");
		return 0;
	}
	tmp = search(&hEntryExtern, getName(hSymbols));
	if (tmp != NULL)
	{
		if (getType(tmp) == EXT)
		{
			fprintf(extFile, "%s\t%d\n", getName(tmp), 100 + getAddress(hSymbols));
		}
	}
	createExtFile(hEntryExtern, (gNode)getNext(hSymbols), extFile);
	return 0;
}

void updateEntExtInSymbols(gNode *hEntExt, gNode *hSymbol)
{
	gNode temp = *hSymbol;
	gNode temp2 = NULL;
	while ((temp) != NULL)
	{
		setARE(temp, 1 << 1);
		temp2 = search(hEntExt, getName(temp));
		if (temp2 != NULL)
		{
			if (getType(temp2) == EXT)
			{
				setARE(temp, 1);
			}
		}
		temp =(gNode) getNext(temp);
	}
}

void printLabels(gNode HEAD)
{
	gNode tmp = HEAD;
	while ((tmp != NULL) && (getName(tmp) != NULL))
	{
		printf("\n");
		printf("%s\t", getName(tmp));
		printf("adress: %d\t", getAddress(tmp));
		printf("\tLine Num: %d\n", getLineNum(tmp));

		tmp = (gNode)getNext(tmp);
	}
	printf("\n");
}
