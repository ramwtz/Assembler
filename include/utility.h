#ifndef UTILITY_H
#define UTILITY_H
#include "my_types.h"

/***    Boolean Queries     ***/

/*  return true if kw has a variable as the third parameter, false othwewise   */
boolean hasThirdVar(Keyword kw);

/*  return true if keyword has a register as the second parameter, false othwewise   */
boolean hasSecondReg(Keyword kw);

/*  return true if keyword receives three parameters, false othwewise   */
boolean has3Params(Keyword kw);

/*  return true if keyword receives one parameter, false othwewise   */
boolean just1Param(Keyword kw);

/*  return true if keyword is one of the data storing directives, false othwewise   */
boolean isDataDir(Keyword kw);

/*  return true if keyword is one of the integral type data storing directives, false othwewise   */
boolean isIntDir(Keyword kw);

/*  return true if keyword is one of the code operations, false othwewise   */
boolean isCode(Keyword kw);

/*  return true if keyword is considered an R type operation, false othwewise   */
boolean isRType(Keyword kw);

/*  return true if keyword is considered an I type operation, false othwewise   */
boolean isIType(Keyword kw);

/*  return true if keyword is considered a J type operation, false othwewise   */
boolean isJType(Keyword kw);

/*  return true if keyword is one of the conditional I type operations, false othwewise   */
boolean isConditional(Keyword kw);

/*  return true if keyword is .extern or .entry, false othwewise   */
boolean isModifierDir(Keyword kw);

/*  return true if keyword is code operation that receives variable as parameter, false othwewise   */
boolean isCodeWithVars(Keyword kw);

/***    myType queries     ***/

/*  returns size of directive passed in bytes, defaults to 0*/
int whichSize(Keyword dir);

/*  returns which section of the memory image kw encodes to  */
Section whichSection(Keyword kw);

/***    Converters      ***/

/*  returns Opcode enum equivalent of the kw  */
Opcode keyToOp(Keyword kw);

/*  returns Funct enum equivalent of the kw  */
Funct keyToFunct(Keyword kw);

#endif