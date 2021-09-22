#include "binary_encoder.h"

/*** binary encoders ***/

/*  resize the the source to fit needed bit width, then store the content in dest,
    and shodt dest to allow for future encoding     */
void encode(word* dest, word source, int resize, int shift) {
    source <<= resize;
    source >>= resize;
    *dest |= source;
    *dest <<= shift;
}

/*  uses encode to create a word with the R type storing formula    */
void encodeRType(word* dest, Keyword kw, word* paramArr) {   
    int i = 1;

    encode(dest, (word) keyToOp(kw), 0, REG_SIZE_BIT);
    encode(dest, (word) paramArr[0], 0, REG_SIZE_BIT);

    /* to differentiate between 2 param and 3 param R types */
    if(has3Params(kw))
        encode(dest, (word) paramArr[i++], 0, REG_SIZE_BIT);
    else
        encode(dest, (word) 0, 0, REG_SIZE_BIT);        
        
    encode(dest, (word) paramArr[i], 0, REG_SIZE_BIT);
    encode(dest, (word) keyToFunct(kw), 0, R_TYPE_UNUSED_BIT);
}

/*  uses encode to create a word with the I type storing formula    */
void encodeIType(word* dest, Keyword kw, word* paramArr) {    
    encode(dest, (word) keyToOp(kw), 0, REG_SIZE_BIT);
    encode(dest, (word) paramArr[0], 0, REG_SIZE_BIT);

    /* conditional operators have different param input order */
    if(isConditional(kw)) {
        encode(dest, (word) paramArr[1], 0, IMM_SIZE_BIT);
        encode(dest, (word) paramArr[2], IMM_SIZE_BIT, 0);
    }
    else {
        encode(dest, (word) paramArr[2], 0, IMM_SIZE_BIT);
        encode(dest, (word) paramArr[1], IMM_SIZE_BIT, 0);
    }
}

/*  uses encode to create a word with the J type storing formula    */
void encodeJType(word* dest, Keyword kw, word* paramArr) {
    encode(dest, (word) keyToOp(kw), 0, REG_FLAG_SIZE_BIT);

    /* jmp is the only J type that can have a different reg flag */
    if(kw == kw_jmp)
        encode(dest, (word) paramArr[1], 0, ADDRESS_SIZE_BIT);
    else
        encode(dest, (word) 0, 0, ADDRESS_SIZE_BIT);

    encode(dest, (word) paramArr[0], 0, 0);
}

/***    load to mem      ***/

/*  chooses proper encoding structure and stores in given byte array    */
void convertCodeToBinary(Keyword kw, word* params, byte* byteArr) {
    word w = 0;

    if(isRType(kw))
        encodeRType(&w, kw, params);
    else if(isIType(kw))
        encodeIType(&w, kw, params);
    else if(isJType(kw))
        encodeJType(&w, kw, params);
    
    assignToByteArr(w, byteArr, sizeof(word));
}

/*  encodes each param in an array of bytes (each param is 4 bytes max), 
    and loads the array to the data memory  */
void loadDataToMem(word* paramArr, int paramCount, int unitSize, byte* mem, int* adrsP) {
    int i;
    byte barr[4];

    for(i = 0; i < paramCount; i++) {
        assignToByteArr(paramArr[i], barr, unitSize);
        loadBytesToMem(barr, unitSize, mem, adrsP);
        emptyByteArr(barr);
    }
}

/*  moves the leftmost byte to the right end, stores it as a byte, and 
    deletes the byte passed by redefining w through left bit wise shift */
void assignToByteArr(word w, byte* byteArr, int size) {
    int i, shift;

    for(i = size-1; i >= 0; i--) {  
        shift = (BITS_IN_BYTE * size) - BITS_IN_BYTE;
        byteArr[i] = (byte) (w >> shift);
        w <<= BITS_IN_BYTE;
    }
}

void loadBytesToMem(byte *byteArr, int arrSize, byte* mem, int* adrsP) {
    int i;

    for(i = 0; i < arrSize; i++) {
        mem[*adrsP] = byteArr[i];
        (*adrsP)++;
    }
}

void emptyByteArr(byte* byteArr) {
    memset(byteArr, (byte) 0, sizeof(word));
}