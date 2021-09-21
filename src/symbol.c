#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"

/***    struct definition   ***/

typedef struct Symbol {
    char label[LONGEST_VAR_NAME];
    int address;
    Section sec;
    Modifier mod;
    boolean usedInConditional;
} Symbol;

typedef struct SymbolTable {
    Symbol **tableArr;
    int size;
} SymbolTable;

/***    constructors    ***/

/*  allocate memory for table, allocate an array of pointers to symbols of size INIT_TABLE_SIZE
    and define that to be the size of the table, exit if unsuccessful     */
SymbolTable *symbolTableCtor() {
    SymbolTable * symtbl = (SymbolTable *) malloc(sizeof(SymbolTable));
    if(symtbl == NULL) {
        printf("%s", "Unable to allocate");
        exit(EXIT_FAILURE);
    }

    symtbl->tableArr = (Symbol **) calloc(INIT_TABLE_SIZE, sizeof(Symbol*));
    if(symtbl == NULL) {
        printf("%s", "Unable to allocate");
        exit(EXIT_FAILURE);
    }

    symtbl->size = INIT_TABLE_SIZE;
    return symtbl;
}

/*  allocate memory for new symbol, exit if unsuccessful    */
Symbol *symbolCtor() {
    Symbol *symP = (Symbol *) malloc(sizeof(Symbol)); 
    if(symP == NULL) {
        printf("%s", "Unable to allocate");
        exit(EXIT_FAILURE);
    }
    return symP;
}

/***    getters     ***/

char *getLabel(Symbol *symP) {
    return symP->label;
}

int getAddress(Symbol *symP) {
    return symP->address;
}

Section getSection(Symbol *symP) {
    return symP->sec;
}

Modifier getModifier(Symbol *symP) {
    return symP->mod;
}

boolean getUsedInConditional(Symbol *symP) {
    return symP->usedInConditional;
}

Symbol **getTableArr(SymbolTable *symtbl) {
    return symtbl->tableArr;
}

int getTableSize(SymbolTable *symtbl) {
    return symtbl->size;
}

/***    setters     */

void setModifier(Symbol *symP, Modifier mod) {
    symP->mod = mod;
}

void setUsedInConditional(Symbol *symP, boolean bool) {
    symP->usedInConditional = bool;
}

void setTemporaryAddress(Symbol *symP, Section sec, int address) {
    symP->sec = sec;
    symP->address = address;
}

/*  set the data addresses relative to the final code address's value   */ 
void setAbsoluteAddresses(SymbolTable *symtbl, int MC) {
    int i = 0, adrs;

    while(symtbl->tableArr[i] != NULL) {
        if(getSection(symtbl->tableArr[i]) == sc_data) {
            adrs = getAddress(symtbl->tableArr[i]);
            symtbl->tableArr[i]->address = adrs + MC;
        }
        i++;
    }
}

/***    manipulator     ***/

void addSymbolToTable(SymbolTable *symtbl, int freeLoc, char* label) {
    symtbl->tableArr[freeLoc] = symbolCtor();
    strcpy(symtbl->tableArr[freeLoc]->label, label);
    symtbl->tableArr[freeLoc]->address = 0;
    symtbl->tableArr[freeLoc]->sec = sc_none;
    symtbl->tableArr[freeLoc]->mod = md_none;
}

/*  reallocate the table arr with twice the size of the previous array  */
void expandTable(SymbolTable *symtbl) {
    symtbl->tableArr = (Symbol **) realloc (symtbl->tableArr, 2 * symtbl->size);
    if(symtbl->tableArr == NULL) {
        printf("%s", "Unable to allocate");
        exit(EXIT_FAILURE);
    }
    symtbl->size *= 2;   
}

/***    query    ***/

/*  check if one or less spaces are free in the table    */
boolean isTableFull(SymbolTable *symtbl) {
    int usedSize = getUsedTableSize(symtbl);
    
    return((symtbl->size - usedSize) <= 1);
}

/*  iterate over the table untill we reach the null pointer     */
int getUsedTableSize(SymbolTable *symtbl) {
    int i = 0;
    while(symtbl->tableArr[i] != NULL) {
        i++;
    }
    return i;
}

/*  check if label is in table by iteration through the table   */ 
int inTable(SymbolTable *symtbl, char *label) {
    int i = 0;

    while(symtbl->tableArr[i] != NULL) {
        if(!strcmp(getLabel(symtbl->tableArr[i]), label))
            return i;
        i++;
    }
    return NOT_IN_TABLE;
}

/*  checks if no impossible definitions have happed in the table    */
Error validateSymbolTable(SymbolTable *symtbl) {
    int i = 0;
    Error err = er_no_error;
    
    while(symtbl->tableArr[i] != NULL) {
        if(getModifier(symtbl->tableArr[i]) == md_extern) {
            if(getAddress(symtbl->tableArr[i]) != 0)
                err = er_redefinition_of_label;
            if(getUsedInConditional(symtbl->tableArr[i]) == TRUE)
                err = er_extern_used_in_conditional;
        }
        else {
            if(getAddress(symtbl->tableArr[i]) == 0) {
               err = er_label_location_unkown;
            }
        }
        i++;
    }
    return err;
}

/***    free    ***/

void freeTable(SymbolTable *symtbl) {
    int i = 0;

    while(symtbl->tableArr[i] != NULL) {
        free(symtbl->tableArr[i++]);
    }

    free(symtbl->tableArr);
    free(symtbl);
}