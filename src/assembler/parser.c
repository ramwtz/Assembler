#include "parser.h"

/***    validators    ***/

/*  gets next word in buffer, and checks if its a register  */
Error validateRegParam(char* str, char** bufferP) {
    getNextWord(str, bufferP);
    skipWhiteSpace(bufferP);
    
    if(!isRegister(str))
        return er_invalid_reg;
    return er_no_error;
}

/*  gets next word in buffer, and checks if its a immediate parameter  */
Error validateImmParam(char* str, char** bufferP) {
    getNextWord(str, bufferP);
    skipWhiteSpace(bufferP);

    if(!isImmediate(str))
        return er_invalid_imm;
    return er_no_error;
}

/*  gets next word in buffer, and checks if its a legal variable  */
Error validateVarParam(char* str, char** bufferP, Keyword kw, SymTblP symtbl, int* nextFreeTable) {
    SymP sym;
    Error err;
    int loc;
    
    getNextWord(str, bufferP);
    skipWhiteSpace(bufferP);

    if((err = checkVar(str)))
        return err;

    loc = inTable(symtbl, str);
    if(loc == NOT_IN_TABLE) {
        if(isTableFull(symtbl))
            expandTable(symtbl);
        addSymbolToTable(symtbl, *nextFreeTable, str);
        loc = (*nextFreeTable)++;
    }

    sym = getTableArr(symtbl)[loc];
    if(kw == kw_extern) {
        if(getModifier(sym) == md_entry || getAddress(sym) != 0)
            return er_redefinition_of_label;
        setModifier(sym, md_extern);
    }
    else if(kw == kw_entry) {
        if(getModifier(sym) == md_extern)
            return er_redefinition_of_label;
        setModifier(sym, md_entry);
    }
    
    setUsedInConditional(sym, isConditional(kw));
        
    return er_no_error;
}

/*  validates the params based on the expected structure of the input for a given command  */
Error validateFiniteParams(char* str, char** bufferP, Keyword kw, SymTblP symtbl, int* nextFreeTable) {
    Error err;

    if(!just1Param(kw)) {
        if((err = validateRegParam(str, bufferP)))
            return err;

        if((err = validateComma(bufferP)))
            return err;

        if(has3Params(kw)) {
            if(hasSecondReg(kw)) { 
                if((err = validateRegParam(str, bufferP)))
                    return err;
            }
            else {
                if((err = validateImmParam(str, bufferP)))
                    return err;
            }

            if((err = validateComma(bufferP)))
                return err;
        }
    }

    if(hasThirdVar(kw)) {
        if((err = validateVarParam(str, bufferP, kw, symtbl, nextFreeTable)))
            return err;
    }
    else {
        if((err = validateRegParam(str, bufferP)))
            return err;
    }

    return checkExtraData(*bufferP);
}

/*  validates the jmp operation parameter, wether variable or register  */
Error validateJmpParam(char* str, char** bufferP, SymTblP symtbl, int* nextFreeTable) {
    int loc;
    
    getNextWord(str, bufferP);
    skipWhiteSpace(bufferP);

    if(!isRegister(str)) {
        if(checkVar(str))
            return er_invalid_jmp_param;
        
        if((loc = inTable(symtbl, str)) == NOT_IN_TABLE) {
            if(isTableFull(symtbl))
                expandTable(symtbl);
            addSymbolToTable(symtbl, (*nextFreeTable)++, str);
        }
    }
    return checkExtraData(*bufferP);
}

/*  validates the params of any integral data directive iteratively until buffer ends   */
Error validateIntDir(char* str, char** bufferP, int* DC, Keyword kw) {
    int param;
    Error err;
    
    while(TRUE) {
        getNextWord(str, bufferP);
        skipWhiteSpace(bufferP);

        if(!isInteger(str))
            return er_param_not_integral_type;
        param = atoi(str);
        if(!paramInBounds(param, kw))
            return er_param_out_of_bounds;

        *DC += whichSize(kw);
        
        if((err = validateComma(bufferP))) {
            if(!checkExtraData(*bufferP))
                break;
            else
                return err;
        }
    }

    return er_no_error;
}

/*  validates the string param of the asciz directive iteratively until quotation mark termination   */
Error validateAsciz(char* str, char** bufferP, int* DC) {
    if(**bufferP != '\"')
        return er_missing_quotation_mark;
    advancePointer(bufferP);

    while(**bufferP != '\"') {
        if(**bufferP == '\0')
            return er_missing_quotation_mark;
        (*DC)++;
        (*bufferP)++;
    }
    (*DC)++;
    advancePointer(bufferP);

    return checkExtraData(*bufferP);
}

/* check if comma and advanced the buffer if so     */
Error validateComma(char** bufferP) {
    if(!isComma(*bufferP)) 
        return er_missing_comma;
    
    advancePointer(bufferP);
    skipWhiteSpace(bufferP);
    
    return er_no_error; 
}

/***    error checkers    ***/

Error checkExtraData(char* str) {
    if(!isLineDelimiter(str))
        return er_extraneous_text_after_command;
    return er_no_error;
}

Error checkLabel(char* str) {
    char temp[MAX_LINE_LENGTH], *loc;
    long int dis, len;

    emptyCharArray(temp, MAX_LINE_LENGTH);

    len = strlen(str);

    loc = strchr(str, ':');
    if(loc == NULL)
        return er_label_terminated_incorrectly;
    dis = loc - str;
    if(len - dis > 1)
        return er_label_terminated_incorrectly;
    strncpy(temp, str, dis);
    return checkVar(temp);
}

Error checkVar(char* str) {
    int len = strlen(str); 
    
    if(len > LONGEST_VAR_NAME || len < SHORTEST_VAR_NAME)
        return er_label_name_too_long;
    if(whichKeyword(str))
        return er_label_name_cannot_be_Keyword; 
    if(!isalpha(*str))
        return er_illegal_char_in_label_name;
    
    while(*str != '\0') {
        if(!isalnum(*str++))
            return er_illegal_char_in_label_name;
    }
    return er_no_error;
}

/***     buffer manipulation    ***/

void emptyCharArray(char* str, int size) {
    memset(str, 0, size);
}

void skipWhiteSpace(char** strP) {
    while(isWhiteSpace(*strP) && **strP != '\0')
        (*strP)++;
}

/*  fills the dest array until terminated by comma, white space or line delimiter   */
void getNextWord(char* dest, char** sourceP) {
    int i = 0;

    emptyCharArray(dest, MAX_LINE_LENGTH);
    while(!isComma(*sourceP) && !isWhiteSpace(*sourceP) && !isLineDelimiter(*sourceP)) {
        dest[i++] = *(*sourceP)++;
    }
}

void advancePointer(char** sourceP) {
    (*sourceP)++;
}

/***    queries    ***/

boolean isImmediate(char* str) {
    unsigned int i;

    if(isInteger(str)) {
        i = atoi(str);
        if(i <= UNSIGNED_WORD_MAX)
            return TRUE;    
    }
    return FALSE;
}

boolean isRegister(char* str) {
    int i;

    if(*str == '$') {
        if(isInteger(str+1)) {
            i = atoi(str+1);
            if(i >= FIRST_REG && i <= LAST_REG)
                return TRUE;
        }
    }
    return FALSE;
}

boolean isComment(char* str) {
    return (*str == ';');
}

boolean isComma(char* str) {
    return (*str == ',');
}

boolean isWhiteSpace(char* str) {
    return (*str == ' ' || *str == '\t');
}

boolean isLineDelimiter(char* str) {
    return (*str == '\0' || *str == '\n');
}


boolean isInteger(char* str) {
    if(*str == '-' || *str == '+')
        str++;
    
    do {
        if(!isdigit(*str++))
            return FALSE;
    } while(*str != '\0');

    return TRUE;
}

/* returns true if param is in bounds of data directive, false otherwise    */
boolean paramInBounds(unsigned int param,  Keyword dir) {
    switch (dir) {
        case kw_db:
            return (byte) param <= UNSIGNED_BYTE_MAX;
        case kw_dh:
            return param <= UNSIGNED_HALFWORD_MAX;
        case kw_dw:
            return param <= UNSIGNED_WORD_MAX;
        default:
            return FALSE;
    }
}

/*  compares str to all known keyword and returns the appropriate keyword enum  */
Keyword whichKeyword(char* str) {
    if(!strcmp(str, "add"))
        return kw_add;
    else if(!strcmp(str, "sub"))
        return kw_sub;
    else if(!strcmp(str, "and"))
        return kw_and;
    else if(!strcmp(str, "or"))
        return kw_or;
    else if(!strcmp(str, "nor"))
        return kw_nor;
    else if(!strcmp(str, "move"))
        return kw_move;
    else if(!strcmp(str, "mvhi"))
        return kw_mvhi;
    else if(!strcmp(str, "mvlo"))
        return kw_mvlo;
    else if(!strcmp(str, "addi"))
        return kw_addi;
    else if(!strcmp(str, "subi"))
        return kw_subi;
    else if(!strcmp(str, "andi"))
        return kw_andi;
    else if(!strcmp(str, "ori"))
        return kw_ori;
    else if(!strcmp(str, "nori"))
        return kw_nori;
    else if(!strcmp(str, "bne"))
        return kw_bne;
    else if(!strcmp(str, "beq"))
        return kw_beq;
    else if(!strcmp(str, "blt"))
        return kw_blt;
    else if(!strcmp(str, "bgt"))
        return kw_bgt;
    else if(!strcmp(str, "lb"))
        return kw_lb;
    else if(!strcmp(str, "sb"))
        return kw_sb;
    else if(!strcmp(str, "lw"))
        return kw_lw;
    else if(!strcmp(str, "sw"))
        return kw_sw;
    else if(!strcmp(str, "lh"))
        return kw_lh;
    else if(!strcmp(str, "sh"))
        return kw_sh;
    else if(!strcmp(str, "jmp"))
        return kw_jmp;
    else if(!strcmp(str, "la"))
        return kw_la;
    else if(!strcmp(str, "call"))
        return kw_call;
    else if(!strcmp(str, "stop"))
        return kw_stop;
    else if(!strcmp(str, ".db"))
        return kw_db;
    else if(!strcmp(str, ".dh"))
        return kw_dh;
    else if(!strcmp(str, ".dw"))
        return kw_dw;
    else if(!strcmp(str, ".asciz"))
        return kw_asciz;
    else if(!strcmp(str, ".extern"))
        return kw_extern;
    else if(!strcmp(str, ".entry"))
        return kw_entry;
    return kw_not_keyword;
}