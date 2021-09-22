#ifndef BINARY_ENCODER_H
#define BINARY_ENCODER_H

/***    include     ***/
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "my_types.h"

/***    macro definitions   ***/

#define REG_SIZE_BIT 5
#define R_TYPE_UNUSED_BIT 6
#define IMM_SIZE_BIT 16
#define REG_FLAG_SIZE_BIT 1
#define ADDRESS_SIZE_BIT 25
#define BITS_IN_BYTE 8

/***    binary encoders     ***/

/*  encodes a the source to the dest, using the resize value for shortening, and the shift value for future encoding*/
void encode(word *dest, word source, int resize, int shift);

/*  encode R type word structure using the keyword and the array of params  */
void encodeRType(word *dest, Keyword kw, word *paramArr);

/*  encode I type word structure using the keyword and the array of params  */
void encodeIType(word *dest, Keyword kw, word *paramArr);

/*  encode J type word structure using the keyword and the array of params  */
void encodeJType(word *dest, Keyword kw, word *paramArr);

/***    load to mem      ***/

/*  calls the proper encoding routine, break the word into bytes and stores in byte array   */
void convertCodeToBinary(Keyword kw, word *paramArr, byte *byteArr);

/*  loads the data params of size unitSize to the data memory at given address*/
void loadDataToMem(word *paramArr, int paramCount, int unitSize, byte *dataMemory, int *addressPointer);

/*  break the word down to bytes and store in the byte array    */
void assignToByteArr(word w, byte *byteArr, int arrSize);

/*  loads a byte array to memory at given address   */
void loadBytesToMem(byte *byteArr, int arrSize, byte *dataMemory, int* addressPointer);

/*  empty byte array for future use     */
void emptyByteArr(byte *byteArr);

#endif