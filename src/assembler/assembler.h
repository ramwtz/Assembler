#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/***    includes     ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
#include "my_types.h"
#include "parser.h"
#include "symbol.h"
#include "binary_encoder.h"
#include "file_manager.h"

/***    macro definitions   ***/

#define SIZE_OF_MEM 33554432
#define FIRST_ADDRESS 100
#define IN_SYMBOL_TABLE -1

/***    type definitions    ***/

typedef struct MemoryCounters *MemCountP;
typedef struct LocationInFile *FileLocP;
typedef struct ParamStruct *ParamStrP;

/***    main parts    ***/

/*  assembles a given file*/
void assembleFile(char* sourceFileName, int sourceFileNumber);

/*  the first pass over the source code, analysis and symbol table construction     */
void firstPass(FILE *srcfp, FileLocP locInFile, SymTblP symbolTable, boolean *continueAssembly, MemCountP memoryCounter);

/*  analyzes a single line in the first pass  */
void analyzeLine(char *buffer, FileLocP locInFile, SymTblP symbolTable, int *nextFreeInTableP,  boolean *continueAssembly, MemCountP memoryCounter);

/* the second pass over the source code, synthesis and file management  */
void secondPass(FILE* srcfp, char* sourceFileName, SymTblP symbolTable , MemCountP memoryCounter);

/*  synthesizes a single line in the second pass    */
void synthesizeLine(char* buffer, SymTblP symbolTable, byte *dataMemory, MemCountP memoryCounter, OutFilesP ofp);

/*  auxiliary routine to load a param to the param array for future encoding    */
void loadParamToArray(char *str, SymTblP symbolTable, Keyword kw, ParamStrP paramstr, MemCountP memoryCounter, OutFilesP ofp);

/***    Error Handling      ***/

/*  checks if memory size not exceding max ram size     */
Error checkMemorySize(int currentMemSize);

/*  checks if error, if true prints error to console and changes continue assembly flag to false, finally returning true;
    else returns false   */
boolean checkIfError(Error err, FileLocP locInFile, boolean *continueAssembly);

/*  print appropriate text to console for given error   */
void printError(Error err, FileLocP locInFile);

#endif