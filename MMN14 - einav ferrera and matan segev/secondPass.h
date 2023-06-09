#ifndef secondPass
#define secondPass
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXIMUM_FILE_NAME 1000 /* max length of file name  */

/** secPass
 * the main function of second pass
 * @param fileName - the name of file as written in terminal
 * @param hRow - list of rows
 * @param hSuspectLabel - list of suspect labels - used in instructions
 * @param hSymbol - list of all delared labels
 * @param hEntryExtern - list of entry and extern labels
 * @param IC - instruction counter
 * @param DC - data counter
 * */
void secPass(char *fileName, gNode *hRow, gNode *hSuspectLabel, gNode *hSymbol, gNode *hEntryExtern, int IC, int DC);

/** createEntFile
 * creating .ent extenssion file for all the entry labels and was declared
 * @param hSymbol - list of all delared labels
 * @param hEntryExtern - list of entry and extern labels
 * @param entFile - pointer to opened .ent file
 * */
int createEntFile(gNode hEntryExtern, gNode hSymbols, FILE *entFile);

/** createExtFile
 * creating .ext extenssion file for all the extern labels and was declared
 * @param hSymbol - list of all delared labels
 * @param hEntryExtern - list of entry and extern labels
 * @param entFile - pointer to opened .ent file
 * */
int createExtFile(gNode hEntryExtern, gNode hSymbols, FILE *extFile);

/** updateEntExtInSymbols
 * setting the A/R/E value inside each label in the list
 * @param hEntryExtern - list of entry and extern labels
 * @param hSymbol - list of all delared labels
 * @param entFile - pointer to opened .ent file
 * */
void updateEntExtInSymbols(gNode *hEntExt, gNode *hSymbol);

/** printLabels
 * printting the labels that used and didnt declare
 * @param HEAD - head of the certain list (suspect label list)
 * */
void printLabels(gNode HEAD);

/** cmpListCpyDeleteSameName
 * run on 2 lists and cmp if have the same label. if does, copy to the third list, and delete form the second
 * @param src - first to compare
 * @param willBeDeleted - second to compare, will be delete from if found
 * @param willBeExtended - will be extend if found
 * */
void cmpListCpyDeleteSameName(gNode *src, gNode *willBeDeleted, gNode *willBeExtended);
#endif
