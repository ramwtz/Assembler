#ifndef PARSE_H
#define PARSE_H
#include "my_types.h"

/***    macro definitions    ***/

#define FIRST_REG 0
#define LAST_REG 32
#define SIGNED_BYTE_MAX 127
#define SIGNED_BYTE_MIN -128
#define SIGNED_HALFWORD_MAX 32767
#define SIGNED_HALFWORD_MIN -32768
#define SIGNED_WORD_MAX 2147483647
#define SIGNED_WORD_MIN -2147483648
#define UNSIGNED_WORD_MAX 4294967295UL
#define	UNSIGNED_HALFWORD_MAX 65535
#define UNSIGNED_BYTE_MAX 255

/***    validators    ***/

/*  validate a register-type param   */
Error validateRegParam(char *param, char **bufferPointer);

/*  validate an immediate-type param   */
Error validateImmParam(char *param, char **bufferPointer);

/*  validate a variable-type param, also add to table if needed   */
Error validateVarParam(char *param, char **bufferPointer, Keyword kw, SymTblP symtbl, int *pointerToNextFreeInTable);

/*  validate finite param input     */
Error validateFiniteParams(char *params, char **bufferPointer, Keyword kw, SymTblP symtbl, int *pointerToNextFreeInTable);

/*  validate the parameter of the jmp operation     */
Error validateJmpParam(char *param, char **bufferPointer, SymTblP symtbl, int *pointerToNextFreeInTable);

/*  validate the parameters of integral type data directives    */
Error validateIntDir(char *params, char **bufferPointer, int *DataCount, Keyword kw);

/*  validate the string input of the asciz directive     */
Error validateAsciz(char *stringInput, char **bufferPointer, int*);

/* validate if next char is comma   */
Error validateComma(char **bufferPointer);

/***    error checkers    ***/

/*  check if any extraneous content after finishing parsing, return appropriate error enum     */
Error checkExtraData(char *buffer);

/*  checks if legal label, returns appropriate error enum    */
Error checkLabel(char *label);

/*  check if legal variable, returns appropriate error enum     */
Error checkVar(char *var);

/***     buffer manipulation    ***/

/*  set all chars to null char in char array of given size    */
void emptyCharArray(char *arr, int size);

/*  set all chars to null char in char array of given size    */
void skipWhiteSpace(char **bufferPointer);

/*  put the next white-char or comma terminated string in str   */
void getNextWord(char *str ,char **bufferPointer);

/*  move the buffer to point to next char   */
void advancePointer(char **bufferPointer);

/***    queries    ***/

/*  return true if str is a comma, false otherwise   */
boolean isComma(char *str);

/*  return true if str is a register, false otherwise   */
boolean isRegister(char *str);

/*  return true if str is an immediate param, false otherwise   */
boolean isImmediate(char *str);

/*  return true if buffer is a comment line, false otherwise   */
boolean isComment(char *buffer);

/*  return true if str is a line delimiter, false otherwise   */
boolean isLineDelimiter(char *str);

/*  return true if str is a white space, false otherwise   */
boolean isWhiteSpace(char *str);

/*  return true if str is an integer, false otherwise   */
boolean isInteger(char *str);

/*  return true if param is in , false otherwise   */
boolean paramInBounds(unsigned int param, Keyword dir);

/*  return Keyword enum equivalent of passed str    */
Keyword whichKeyword(char *keywordStr);

#endif