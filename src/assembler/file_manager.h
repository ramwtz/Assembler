#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

/***    include     ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_types.h"

/***    macro definitions   ***/

#define LENGTH_OF_SOURCE_EXTENSION 2
#define TOTAL_BYTES_IN_ROW 4
#define FIRST_BYTE_IN_ROW 0
#define LAST_BYTE_IN_ROW 3

/***    creators    ***/

/*  creates an instance of a fileStruct, receives the source file name 
    and extension of the file should have */
FileStrctP createFileStruct(char *sourceFileName, const char *extension);

/*  creates an instance of an outputFiles struct, holds all three output files  */
OutFilesP createOutputFiles(char *inputFileName);

/*  creates a file named bu given file name  */ 
FILE* createFile(char *fileName);

/*  puts a new file name in dest, built from source and extension   */
void createFileName(char *dest, char *source, const char *extension);

/***    getters     ***/

/*  returns pointer to FILE for a fileStruct object     */
FILE *getFilePointer(FileStrctP fsp);

/*  returns the name of the file in a fileStruct object     */
char *getFileName(FileStrctP fsp);

/*  returns pointer to object fileStruct for the outputFiles struct     */
FileStrctP getObFile(OutFilesP ofp);

/*  returns pointer to extern fileStruct for the outputFiles struct     */
FileStrctP getExtFile(OutFilesP ofp);

/*  returns pointer to entry fileStruct for the outputFiles struct     */
FileStrctP getEntFile(OutFilesP ofp);

/***    print      ***/

/* prints contents of byteArr to the object file with the current address in the memory image   */
void printToObFile(FILE* obfp, byte* byteArr, int byteArrSize, int crntAdress);

/*  add a reference to an external to the ext file  */
void addReferenceToExt(FILE* extfp, char* str, int address);

/*  add an entry label to the ent file  */
void addLabelToEnt(FILE *entfp, char* label, int address);

/***    queries     ***/

/*  return true if file is empty, false otherwise   */
boolean isFileEmpty(FILE* fp);

/*  returns the length of the current row   */
int rowLength(FILE *fp);

/***    error checkers      ***/

/*  check if input file name is of legal type    */

/*  check if input file isn't empty    */
Error checkInputFileContent(FILE *fp);

/***    free, close, & delete    ***/

/*  check if export is empty, if so delete it */
void removeExportFileIfEmpty(FileStrctP fsp);

/*  frees a given fileStruct     */
void freeFileStrct(FileStrctP fsp);

/*  closes all output files and frees relevant structs  */
void closeOutputFiles(OutFilesP ofp);

#endif