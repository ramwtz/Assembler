#include <stdio.h>
#include "utility.h"

/***    Boolean Queries     ***/

/*  checks if kw is equal to any keyword enum thats has 3 params     */ 
boolean has3Params(Keyword kw) {
    return kw == kw_add || kw == kw_sub || kw == kw_and
        || kw == kw_or || kw == kw_nor || kw ==kw_addi
        || kw == kw_subi || kw == kw_andi || kw == kw_ori
        || kw == kw_nori || kw == kw_lb || kw == kw_sb
        || kw == kw_lw || kw == kw_sw || kw == kw_lh
        || kw == kw_sh || kw == kw_beq || kw == kw_bne
        || kw == kw_blt || kw == kw_bgt;
}

/*  checks if kw is equal to any keyword enum thats has a register as a second param  */ 
boolean hasSecondReg(Keyword kw) {
    return kw == kw_add || kw == kw_sub || kw == kw_and
        || kw == kw_or || kw == kw_nor || kw ==kw_beq 
        || kw == kw_bne || kw == kw_blt || kw == kw_bgt;
}

/*  checks if kw is equal to any keyword enum thats has a variable as a third param  */ 
boolean hasThirdVar(Keyword kw) {
    return kw == kw_beq || kw == kw_bne || kw == kw_blt 
    || kw == kw_bgt || just1Param(kw);
}

/*  checks if kw is equal to any keyword enum thats has only one param  */ 
boolean just1Param(Keyword kw) {
    return kw == kw_la || kw == kw_call || isModifierDir(kw);
}

/*  checks if kw is equal to any keyword enum thats is a code operation  */ 
boolean isCode(Keyword kw) {
    return isRType(kw) || isIType(kw) || isJType(kw); 
}

/*  checks if kw is equal to any keyword enum that is encoded in the structure of R types   */
boolean isRType(Keyword kw) {
    return kw == kw_add || kw == kw_sub || kw == kw_and
        || kw == kw_or || kw == kw_nor || kw == kw_move
        || kw == kw_mvhi || kw == kw_mvlo;
}

/*  checks if kw is equal to any keyword enum that is encoded in the structure of I types   */
boolean isIType(Keyword kw) {
    return kw == kw_addi || kw == kw_subi || kw == kw_andi 
        || kw == kw_ori || kw == kw_nori || kw == kw_lb 
        || kw == kw_sb || kw == kw_lw || kw == kw_sw 
        || kw == kw_lh || kw == kw_sh || kw == kw_beq 
        || kw == kw_bne || kw == kw_blt || kw == kw_bgt;
}

/*  checks if kw is equal to any keyword enum that is encoded in the structure of J types   */
boolean isJType(Keyword kw) {
    return kw == kw_jmp || kw == kw_stop || kw == kw_la
        || kw == kw_call;
}

/*  checks if kw is equal to any keyword enum that is one of the conditional I type operations  */
boolean isConditional(Keyword kw) {
    return kw == kw_bgt || kw == kw_beq || kw == kw_blt || kw == kw_bne;
}

/*  checks if kw is equal to extern or entry directives   */
boolean isModifierDir(Keyword kw) {
    return kw == kw_entry || kw == kw_extern;
}

/*  checks if kw is equal to any keyword enum that is one of the data storing directives  */
boolean isDataDir(Keyword kw) {
    return isIntDir(kw) || kw == kw_asciz;
}

/*  checks if kw is equal to any keyword enum that is one of the integral data type storing directives  */
boolean isIntDir(Keyword kw) {
    return kw == kw_db || kw == kw_dh || kw == kw_dw;
}

/*  checks if kw is equal to any keyword enum that has a variable as any param  */
boolean isCodeWithVars(Keyword kw) {
    return isConditional(kw) || kw == kw_la || kw == kw_call
        || kw == kw_jmp;
}

/***    myType queries     ***/

/*  checks which size of data dir is supposed to receive    */
int whichSize(Keyword dir) {
    switch(dir) {
        case kw_asciz:
        case kw_db:
            return sizeof(byte);
        case kw_dh:
            return (sizeof(word) / 2);
        case kw_dw:
        case kw_not_keyword:
            return sizeof(word);
        default:
            printf("No such size in whichSize\n");
            return 0;
    }
}

/*  find which section of memory image the kw encodes to    */ 
Section whichSection(Keyword kw) {
    if(isCode(kw))
        return sc_code;
    else if(isDataDir(kw))
        return sc_data;
    else
        return sc_none;
}

/***    Converters      ***/

/*  converts kw to equivalent opcode enum   */
Opcode keyToOp(Keyword kw) {
    switch (kw) {
        case kw_add:
            return op_add;
        case kw_sub:
            return op_sub;
        case kw_and:
            return op_and;
        case kw_or:
            return op_or;
        case kw_nor:
            return op_nor;
        case kw_move:
            return op_move;
        case kw_mvhi:
            return op_mvhi;
        case kw_mvlo:
            return op_mvlo;
        case kw_addi:
            return op_addi;
        case kw_subi:
            return op_subi;
        case kw_andi:
            return op_andi;
        case kw_ori:
            return op_ori;
        case kw_nori:
            return op_nori;
        case kw_bne:
            return op_bne;
        case kw_beq:
            return op_beq;
        case kw_blt:
            return op_blt;
        case kw_bgt:
            return op_bgt;
        case kw_lb:
            return op_lb;
        case kw_sb:
            return op_sb;
        case kw_lw:
            return op_lw;
        case kw_sw:
            return op_sw;
        case kw_lh:
            return op_lh;
        case kw_sh:
            return op_sh;  
        case kw_jmp:
            return op_jmp;
        case kw_la:
            return op_la;
        case kw_call:
            return op_call;
        case kw_stop:
            return op_stop;
        default:
            printf("conversion failed in KeyToOp\n");
            return op_none;
    }
}

/*  converts kw to equivalent funct enum   */
Funct keyToFunct(Keyword kw) {
    switch (kw) {
        case kw_add:
            return ft_add;
        case kw_sub:
            return ft_sub;
        case kw_and:
            return ft_and;
        case kw_or:
            return ft_or;
        case kw_nor:
            return ft_nor;
        case kw_move:
            return ft_move;
        case kw_mvhi:
            return ft_mvhi;
        case kw_mvlo:
            return ft_mvlo;
        default:
            printf("conversion failed in FunctToOp\n");
            return ft_none;
    }
}