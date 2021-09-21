#ifndef MY_TYPES_H
#define MY_TYPES_H
#include <limits.h>

/***  macro definitions   ***/

#define MAX_LINE_LENGTH 81
#define TRUE 1
#define FALSE 0
#define LONGEST_VAR_NAME 32
#define SHORTEST_VAR_NAME 1

/***   type definitions   ***/

/*  choose word definition based on type implementation   */
#define TARGET_MAX 4294967295L

#if USHRT_MAX == TARGET_MAX
  typedef unsigned short word;
#elif ULONG_MAX == TARGET_MAX
  typedef unsigned long word;
#else
  typedef unsigned int word;
#endif

#undef TARGET_MAX

typedef unsigned char byte;
typedef unsigned char boolean;
typedef struct SymbolTable* SymTblP;
typedef struct OutputFiles *OutFilesP;
typedef struct FileStruct *FileStrctP;

/*  enum for error handeling  */
typedef enum {
    er_no_error = 0,
    er_illegal_char_in_label_name,
    er_label_name_cannot_be_Keyword,
    er_label_terminated_incorrectly,
    er_label_name_too_long,
    er_redefinition_of_label,
    er_no_such_operation,
    er_param_not_integral_type,
    er_extraneous_text_after_command,
    er_param_out_of_bounds,
    er_missing_quotation_mark,
    er_invalid_reg,
    er_missing_comma,
    er_illegal_comma,
    er_invalid_imm,
    er_invalid_jmp_param, 
    er_illegal_file_type,
    er_file_is_empty,
    er_line_longer_then_80_chars,
    er_extern_var_defined_as_label,
    er_extern_used_in_conditional,
    er_label_location_unkown,
    er_not_enough_memory
} Error;

/*  enum with all known keyword   */
typedef enum {
    kw_not_keyword,
    kw_add,
    kw_sub,
    kw_and,
    kw_or,
    kw_nor,
    kw_move,
    kw_mvhi,
    kw_mvlo,
    kw_addi,
    kw_subi,
    kw_andi,
    kw_ori,
    kw_nori,
    kw_lb,
    kw_sb,
    kw_lw,
    kw_sw,
    kw_lh,
    kw_sh,
    kw_jmp,
    kw_stop,
    kw_bne,
    kw_beq,
    kw_blt,
    kw_bgt,
    kw_la,
    kw_call,
    kw_extern,
    kw_entry,
    kw_db,
    kw_dh,
    kw_dw,
    kw_asciz
} Keyword;

/*  enum that stores the Opcode's value   */ 
typedef enum {
    op_none = -1,
    op_add = 0,
    op_sub = 0,
    op_and = 0,
    op_or = 0,
    op_nor = 0,
    op_move = 1,
    op_mvhi = 1,
    op_mvlo = 1,
    op_addi = 10,
    op_subi = 11,
    op_andi = 12,
    op_ori = 13,
    op_nori = 14,
    op_bne = 15,
    op_beq = 16,
    op_blt = 17,
    op_bgt = 18,
    op_lb = 19,
    op_sb = 20,
    op_lw = 21,
    op_sw = 22,
    op_lh = 23,
    op_sh = 24,
    op_jmp = 30,
    op_la = 31,
    op_call = 32,
    op_stop = 63
} Opcode;

/*  enum that stores the Funct's value   */ 
typedef enum {
    ft_none = 0,
    ft_add = 1,
    ft_sub = 2,
    ft_and = 3,
    ft_or = 4,
    ft_nor = 5,
    ft_move = 1,
    ft_mvhi = 2,
    ft_mvlo = 3
} Funct;

/*  enum for the two different sections of the memory image   */
typedef enum {
    sc_none = 0,
    sc_code = 1,
    sc_data = 2
} Section;

/*  enum for the two modifiers of labels  */
typedef enum {
    md_none = 0,
    md_entry = 1,
    md_extern = 2
} Modifier;

#endif