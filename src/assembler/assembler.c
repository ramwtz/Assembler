#include "assembler.h"

/****   Auxiliary local structs     ***/ 
struct MemoryCounters {
    int codeCount;
    int dataCount;
};

struct LocationInFile {
    int currentFileNumber;
    int currentLineNumber;
};

struct ParamStruct {
    int paramCounter;
    int nextFreeLoc;
    word paramArr[MAX_LINE_LENGTH];
};

/***    Main assembly parts    ***/

/*  assembles a given source file into machine code     */
void assembleFile(char *sourceFileName, int sourceFileNumber) {
    boolean continueAssembly = TRUE;
    SymTblP symtbl;
    FILE *srcfp;
    Error err;
    struct MemoryCounters memCount = {0, 0};
    struct LocationInFile locInFile = {0, 0};
    locInFile.currentFileNumber = sourceFileNumber;

    /*  open file to read source code   */
    srcfp = fopen(sourceFileName, "r+");
    if(srcfp == NULL) {
        perror("Unable to read file.\nTerminating.\n");
        exit(EXIT_FAILURE);
    }

    /*  check if file isn't empty, stop assembly if not   */    
    err = checkInputFileContent(srcfp);
    if(checkIfError(err, &locInFile, &continueAssembly))
        return;

    /*  create the symbol table     */
    symtbl = symbolTableCtor();

    /*  call the first pass     */
    firstPass(srcfp, &locInFile, symtbl, &continueAssembly, &memCount);

    /*  check if any error occured at any point, if not then continue to second pass     */
    if(continueAssembly) {
        secondPass(srcfp, sourceFileName, symtbl, &memCount);
    }

    /*  free allocated memory for table & close source file   */
    freeTable(symtbl);
    fclose(srcfp);
}

/*  first pass analyzes the source code for syntax mistakes, fills in and manages the symbol table,
    and handles errors and prints them  */
void firstPass(FILE *srcfp, FileLocP locInFileP, SymTblP symtbl, boolean *continueAssemblyP, MemCountP memCount) {
    int c, nextFreeInTable = 0;
    char buffer[MAX_LINE_LENGTH];
    Error err;
    
    /*  start at the beginning of the file  */
    fseek(srcfp, 0 ,SEEK_SET);

    /*  iterate over all the lines in the source file, loading each line to the buffer   */
    while(fgets(buffer, MAX_LINE_LENGTH, srcfp) != NULL) {
        /*  checks if line buffered is longer then 80 characters by seeing if the new line wasn't buffered */
        if(!strchr(buffer, '\n')) {
            /*  only legal line without new line is the last, and so if the next char is the EOF we've scanned the entire file */
            if(getc(srcfp) == EOF)
                break;
            
            /*  if line in file is longer than 80 characters, we'll loop throughout the line until its end    */
            while((c = getc(srcfp)) != '\n' && c != EOF);

            /*  error handling and and continuing to the next line  */
            printError(er_line_longer_then_80_chars, locInFileP);
            *continueAssemblyP = FALSE;
            locInFileP->currentLineNumber++;
            continue;
        }

        /*  iteratively load each line to the buffer, analyze it, fill in symbols if need be, check for errors,
            and continue to the next line.  */
        analyzeLine(buffer, locInFileP, symtbl, &nextFreeInTable, continueAssemblyP, memCount);
        emptyCharArray(buffer, MAX_LINE_LENGTH);
    }

    /*  if continueAssembly is FALSE, return    */
    if(!continueAssemblyP)
        return;

    /*  after we know how many lines of code will be loaded to memory, we will adjust the temp addresses
        given to the data symbols to their actual values  */
    setAbsoluteAddresses(symtbl, memCount->codeCount + FIRST_ADDRESS);

    /*  after the first pass we check to see if the table generated has no inherent contradictions  */
    err = validateSymbolTable(symtbl);
    locInFileP->currentLineNumber = IN_SYMBOL_TABLE;
    if(err) {
        checkIfError(err, locInFileP, continueAssemblyP);
        return;
    }

    /*  we check to see if after the first pass the total memory is below the size of our RAM,
        and above zero (empty source file)    */
    err = checkMemorySize(memCount->codeCount + memCount->dataCount);
    if(checkIfError(err, locInFileP, continueAssemblyP))
        return;
}

/*  second pass synthesizes the source code into machine binaries, and handling the needed output files, 
    which includes creating, writing, and exporting    */
void secondPass(FILE* srcfp, char* sourceFileName, SymTblP symtbl, MemCountP memCount) {
    FILE *obfp;
    OutFilesP outfsp;
    byte *dataMem;
    char buffer[MAX_LINE_LENGTH];
    int totalCodeSize = memCount->codeCount, totaldataSize = memCount->dataCount;

    /*  create all three possible output files (empty output files will 
        be managed at the end of the second pass)   */
    outfsp = createOutputFiles(sourceFileName);
    obfp = getFilePointer(getObFile(outfsp));

    /*  allocate space for data memory image, so we can print it to file after all code is printed  */
    dataMem = (byte*) calloc(totaldataSize + 1, sizeof(byte));
    if(dataMem == NULL) {
        printf("Unable to allocate virtual memory\n");
        exit(EXIT_FAILURE);
    }

    /*  reset the counters for second pass   */
    memCount->codeCount = 0;
    memCount->dataCount = 0;

    /*  return to beginning of source file  */
    fseek(srcfp, 0, SEEK_SET);

    /*  print first line of object file denoting size of code and data respectively */
    fprintf(obfp, "\t %d %d\n", totalCodeSize, totaldataSize);

    /*  iteratively load each line to the buffer, synthesize it, and continue to the next line.  */  
    while((fgets(buffer, MAX_LINE_LENGTH, srcfp)) != NULL) {
        synthesizeLine(buffer, symtbl, dataMem, memCount, outfsp);
        emptyCharArray(buffer, MAX_LINE_LENGTH);
    }

    /*  append the dataMem to the partially filled object file   */
    printToObFile(obfp, dataMem, totaldataSize, FIRST_ADDRESS + totalCodeSize);

    /*  checks if ext file and ent file were filled, if any is empty it will be deleted     */
    removeExportFileIfEmpty(getExtFile(outfsp));
    removeExportFileIfEmpty(getEntFile(outfsp));

    /*  free data image and all output files    */
    free(dataMem);
    closeOutputFiles(outfsp);
}

/*  checks for syntax errors and fills in the symbol table, in case of erros turns continueAssembly off, 
    cutting of the second pass */
void analyzeLine(char* buffer, FileLocP locInFileP, SymTblP symtbl, int *nextFreeInTableP, boolean *continueAssemblyP, MemCountP memCount) {
    char str[MAX_LINE_LENGTH], label[LONGEST_VAR_NAME];
    int locInTable;
    Keyword kw;
    Error err;
    SymP sym;
    Section sec;

    /*  increment current line to signify our location in the source file for error handling    */
    locInFileP->currentLineNumber++;

    /*  empty buffers to avoid junk    */
    emptyCharArray(str, MAX_LINE_LENGTH); 
    emptyCharArray(label, LONGEST_VAR_NAME);

    /*  move buffer across all initial white space to get beginning of content  */
    skipWhiteSpace(&buffer);
    if(isComment(buffer) || isLineDelimiter(buffer)) 
        return;

    /*  get the first non-blank string terminated by whitespace or comma and store it in str, and
        move the buffer ascross the rest of the whitespace  */
    getNextWord(str, &buffer);
    skipWhiteSpace(&buffer);

    /*  check if string is a known keyword  */
    kw = whichKeyword(str);

    /*  if the string is not a known keyword, check if it's a label definition    */
    if(kw == kw_not_keyword) {
        
        /*  if its an illegal label definition, print error and return, else continue   */
        err = checkLabel(str);
        if(checkIfError(err, locInFileP, continueAssemblyP))
            return;

        /*  we copy the label without the colon*/
        strncpy(label, str, strlen(str) - 1);

        /*  get next white space or comma terminated string in buffer  */
        getNextWord(str, &buffer);
        skipWhiteSpace(&buffer);

        /*  check for illegal comma     */
        err = validateComma(&buffer);
        if(!err) {
            checkIfError(er_illegal_comma, locInFileP, continueAssemblyP);
            return;
        }

        /*  check if string is a known keyword  */
        kw = whichKeyword(str);

        /*  if string is not a known keyword, return with an error, else continue    */
        if(kw == kw_not_keyword) {       
            checkIfError(er_no_such_operation, locInFileP, continueAssemblyP);
            return;
        }

        /*  check if label already in symbol table, and return location if it is    */
        locInTable = inTable(symtbl, label);

        /*  if the command isn't .extern ot .entry (termed "modifier directives" here), check the symbol table, 
            else the label should be ignored   */
        if(!isModifierDir(kw)) {
            /*  if it is not in the symbol table, check if table isn't full, if it is expand, 
            and  either way add the label to the symbol table; else if the symbol is already in the table, 
            check if it is a re-defenition  */    
            if(locInTable == NOT_IN_TABLE) {
                if(isTableFull(symtbl))
                    expandTable(symtbl);
                addSymbolToTable(symtbl, *nextFreeInTableP, label);
                sym = getTableArr(symtbl)[(*nextFreeInTableP)++];
            }
            else {
                sym = getTableArr(symtbl)[locInTable];
                if(getAddress(sym) != 0) {
                    checkIfError(er_redefinition_of_label, locInFileP, continueAssemblyP);
                    return;
                }
                else if(getModifier(sym) == md_extern) {
                    checkIfError(er_extern_var_defined_as_label, locInFileP, continueAssemblyP);
                    return;
                }
            }

            /*  we check which type of keyword it is, and add the temporary address in memory
                to the symbols properties in the table   */
            sec = whichSection(kw);
            if(sec == sc_code)
                setTemporaryAddress(sym, sec, memCount->codeCount + FIRST_ADDRESS);
            else if(sec == sc_data)
                setTemporaryAddress(sym, sec, memCount->dataCount);
        }
    }
    
    /*  check to see which keyword, and select the proper validation method for said keyword.   
        the three validation sections are for directives that have integral parametes of unknown amount,
        asciz which receives a string of unkown length, and the rest of the commands.    */
    if(isIntDir(kw)) 
        err = validateIntDir(str, &buffer, &(memCount->dataCount), kw);
    else if(kw == kw_asciz)
        err = validateAsciz(str, &buffer, &(memCount->dataCount));    
    else if(isCode(kw) || isModifierDir(kw)) {
        /*  stop has no paramaters, check if any extraneous data after command  
            jmp can have either a register or a variable, and so it has a unique validator.
            all other commands in this section have known params of known types, and so are analyzed in
            same validator  */
        if(kw == kw_stop)
            err = checkExtraData(buffer);
        else if(kw == kw_jmp)
            err = validateJmpParam(str, &buffer, symtbl, nextFreeInTableP);
        else 
            err = validateFiniteParams(str, &buffer, kw, symtbl, nextFreeInTableP);

        /*  if isn't modifier directive, it is code, and so we increment the memory by 4 bytes  */
        if(!isModifierDir(kw))
            memCount->codeCount += sizeof(word);
    }

    /*  check if error occured during parameter parsing, and return if so   */
    if(checkIfError(err, locInFileP, continueAssemblyP))
        return;
}

/*  synthesized the line, prints code to object file and location to enntry and extern files, 
    and loades data to data image in dataMem */
void synthesizeLine(char* buffer, SymTblP symtbl, byte *dataMem, MemCountP memCount, OutFilesP ofp) {
    int locInTable, len;
    struct ParamStruct paramstr = {0, 0};
    byte byteArr[4];
    char str[MAX_LINE_LENGTH];
    Keyword kw;
    FILE* obfp, *entfp;
    SymP sym;
    
    /*  get pointers to pbject and entry files    */
    obfp = getFilePointer(getObFile(ofp));
    entfp = getFilePointer(getEntFile(ofp));

    /*  empty one-word-buffer to avoid junk    */
    emptyCharArray(str, MAX_LINE_LENGTH);
    
    /*  move buffer across all initial white space to get beginning of content  
        if is a comment line, or empty line, return   */
    skipWhiteSpace(&buffer);
    if(isComment(buffer) || isLineDelimiter(buffer))
        return;

    /*  get the first non-blank string terminated by whitespace or comma and store it in str, and
        move the buffer ascross the rest of the whitespace  */
    getNextWord(str, &buffer);
    skipWhiteSpace(&buffer);
    kw = whichKeyword(str);

    /*  check if known keyword, if not must be label, so check if label is entry,
        if so write to ent file, then parse the next word, which must be the command  */
    if(kw == kw_not_keyword) {
        len = strlen(str);
        str[len - 1] = 0;
        locInTable = inTable(symtbl, str);
        sym = getTableArr(symtbl)[locInTable];
        if(getModifier(sym) == md_entry)
        addLabelToEnt(entfp, str, getAddress(sym));
        
        getNextWord(str, &buffer);
        skipWhiteSpace(&buffer);
        kw = whichKeyword(str);
    }
    
    /*  modifier directives aren't synthesized     */
    if(isModifierDir(kw))
        return;
    
    /*  load all params to the param array, if asciz then we load every char to the array,
        else if all other commands (except .entry & .extern) we load the params' integral value to 
        the param array  */
    if(kw == kw_asciz) {   
        /*  skip over first quotation mark,
            iteratively load each char to the param array until hit by quotation mark   
            add null terminator at end      */
        advancePointer(&buffer);
        while(*buffer != '\"') {
            paramstr.paramArr[paramstr.nextFreeLoc++] = (word) *buffer++;
            paramstr.paramCounter++;
        }
        paramstr.paramArr[paramstr.nextFreeLoc++] = 0;
        paramstr.paramCounter++;
    }    
    else {
        /*  iterative get next word in buffer, load it to mem, traverse the comma if any,
            and reapeat untill all params in array  */
        while(TRUE) {
            if(isLineDelimiter(buffer))
                break;
            getNextWord(str, &buffer);
            skipWhiteSpace(&buffer);

            loadParamToArray(str, symtbl, kw, &paramstr, memCount, ofp);
            
            if(isLineDelimiter(buffer))
                break;
            advancePointer(&buffer);
            skipWhiteSpace(&buffer);
        }
    }

    /*  if keyword is data directive, we load the params to data image memory,
        else if is code, we print directly to the object file   */
    if(isDataDir(kw)){
        loadDataToMem(paramstr.paramArr, paramstr.paramCounter, whichSize(kw), dataMem, &(memCount->dataCount));
    }
    else if(isCode(kw)){
        emptyByteArr(byteArr);
        convertCodeToBinary(kw, paramstr.paramArr, byteArr);
        printToObFile(obfp, byteArr, sizeof(word), memCount->codeCount + FIRST_ADDRESS);
        memCount->codeCount += sizeof(word);
    }
}

/*  finds what type of integral param was given, and then loads it to param array  */
void loadParamToArray(char *str, SymTblP symtbl, Keyword kw, ParamStrP paramstr, MemCountP memCount, OutFilesP ofp) {
    SymP *tblArr;
    int val, loc, regFlag = 0;
    Modifier mod;
    FILE *extfp;

    /*  get pointers to entry and extern files    */
    extfp = getFilePointer(getExtFile(ofp));

    /*  find which type is param, either an immediate, a register or a known label,
        then assigns its value to val  */
    if(isInteger(str)) {
        val = atoi(str);
    }
    else if(isRegister(str)) {
        val = atoi(str+1);
        if(kw == kw_jmp)
            regFlag = 1;
    }
    else {
        /*  get all the relevant identifiers of the var from the symbol table   */ 
        tblArr = getTableArr(symtbl);
        loc = inTable(symtbl, str);
        val = getAddress(tblArr[loc]);
        mod = getModifier(tblArr[loc]);

        if(isConditional(kw))
            val -= (memCount->codeCount + FIRST_ADDRESS);
        else if(mod == md_extern && isCodeWithVars(kw))
            addReferenceToExt(extfp, str, memCount->codeCount + FIRST_ADDRESS);
    }

    /*  add val to param array, and increment number of params   */
    paramstr->paramArr[paramstr->nextFreeLoc++] = (word) val;
    if(kw == kw_jmp)
        paramstr->paramArr[paramstr->nextFreeLoc] = (word) regFlag;
    
    paramstr->paramCounter++;
}

/***    errors    ***/

/*  check if size of memory after first pass is under the total RAM size  */
Error checkMemorySize(int totalMemSize) {
    if(totalMemSize >= (SIZE_OF_MEM - FIRST_ADDRESS)) 
        return er_not_enough_memory;
    return er_no_error;
}

boolean checkIfError(Error err, FileLocP locInFile, boolean* continueAssembly) {
    if(err) {
        printError(err, locInFile);
        *continueAssembly = FALSE;
        return TRUE;
    }
    return FALSE;
}

void printError(Error err, FileLocP locInFile) {
    printf("Error in file no. %d", locInFile->currentFileNumber);
    
    if(locInFile->currentLineNumber == IN_SYMBOL_TABLE) {
        printf(", in symbol table: ");
    }
    else {
        switch(err) {
            case er_illegal_file_type:
                printf(": file has unknown extension\n");
                return;
            case er_file_is_empty:
                printf(": file is empty\n");
                return;
            case er_not_enough_memory:
                printf(": Available memory is insufficient\n");
                return;
            default:
                printf(", on Line %d: ", locInFile->currentLineNumber);
                break;
        }
    }
    switch(err) {
        case er_illegal_char_in_label_name:
            printf("Illegal char in label name\n");
            break;
        case er_label_name_cannot_be_Keyword:
            printf("A keyword cannot be used as a label name\n");
            break;
        case er_label_name_too_long:
            printf("Label name must not exceed 31 characters\n");
            break;
        case er_label_terminated_incorrectly:
            printf("Label definition must be followed directly by colon and then white space\n"); 
            break;
        case er_redefinition_of_label:
            printf("A label with this name is already defined\n");
            break;
        case er_no_such_operation:
            printf("Invalid command name\n");
            break;
        case er_param_not_integral_type:
            printf("Param is not integral type\n");
            break;
        case er_extraneous_text_after_command:
            printf("Extraneous text after command\n");
            break;
        case er_param_out_of_bounds:
            printf("Integral directive param outside bounds\n");
            break;
        case er_missing_quotation_mark:
            printf("Missing Quotation mark in string\n");
            break;
        case er_invalid_reg:
            printf("Invalid or missing register param\n");
            break;
        case er_missing_comma:
            printf("Missing comma\n");
            break;
        case er_illegal_comma:
            printf("Illegal comma\n");
            break;
        case er_invalid_imm:
            printf("Invalid or missing Immediate param\n");
            break;
        case er_invalid_jmp_param:
            printf("Invalid or missing param for jmp operation\n");
            break;
        case er_illegal_file_type:
            printf("File type unknown to Assembler\n");
            break;
        case er_label_location_unkown:
            printf("A non external variable in memory has no address\n");
            break;
        case er_line_longer_then_80_chars:
            printf("Line longer then max line length (80 characters)\n");
            break;
        case er_extern_var_defined_as_label:
            printf("A variable defined as external can't be used for defining a label\n");
            break;
        case er_extern_used_in_conditional:
            printf("external variables can't be used with conditional operations\n");
            break;  
        default:
            break;
    }           
}