#ifndef SYMBOL_H
#define SYMBOL_H

/***    includes     ***/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "my_types.h"

/***  macro definitions   ***/

#define INIT_TABLE_SIZE 100
#define NOT_IN_TABLE -1

/***    typedef section     ***/
typedef struct Symbol *SymP;

/***    constructors    ***/

/*  creates a pointer to an empty symbol   */
SymP symbolCtor(void);
/*  creates a pointer to a symbol table, including an array of null symbol pointers  */
SymTblP symbolTableCtor(void);

/***    getters     ***/

/*  return symbol's label   */
char* getLabel(SymP symbolPointer);

/*  return symbol's address     */
int getAddress(SymP symbolPointer);

/*  return memory image section where label is located    */
Section getSection(SymP symbolPointer);

/*  return symbol's modifier    */ 
Modifier getModifier(SymP symbolPointer);

/*  return the symbol array in the symbol table     */
SymP* getTableArr(SymTblP symbolTable);

/*  return total size of table (including null pointers)    */
int getTableSize(SymTblP symbolTable);

/***    setters     ***/

/*  set modifier for symbol   */
void setModifier(SymP symbolPointer, Modifier mod);

/*  set the used_in_conditionl boolean flag for symbol   */
void setUsedInConditional(SymP symbolPointer, boolean bool);

/*  set the temporary addresses in the geiven section during the first pass */
void setTemporaryAddress(SymP symbolTable, Section sec, int tempAddress);

/*  set the absolute address after the first pass using the last address of code found  */
void setAbsoluteAddresses(SymTblP symbolTable, int lastCodeAddress);

/***    manipulators    ***/

/*  add a new symbol to the next free location, with the given label    */
void addSymbolToTable(SymTblP symbolTable, int nextFreeLocation, char *label);

/*  dynamically expand the symbol table if more space needed   */
void expandTable(SymTblP symbolTable);

/***    queries     ***/

/*  return TRUE if table is full (except final slot in arr which is a terminator)   */
boolean isTableFull(SymTblP symtbl);

/*  return the size of table actually used  */
int getUsedTableSize(SymTblP symtbl);

/*  returns location of label in table using its name, return -1 if not in table    */
int inTable(SymTblP symtbl, char *label);

/*  validate symbol table integrity, returns error enum if found, no error if none  */
Error validateSymbolTable(SymTblP symtbl);

/***    free    ***/

/*  frees all pointers in the array, then the array itself and finally the table    */
void freeTable(SymTblP symtbl);

#endif