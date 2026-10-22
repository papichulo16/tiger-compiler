#pragma once

#include "temp.h"

/*
 * tree.h - Definitions for intermediate representation (IR) trees.
 *
 */
typedef struct T_stm_ *T_stm;
typedef struct T_exp_ *T_exp;
typedef struct T_expList_ *T_expList;
struct T_expList_ {T_exp head; T_expList tail;};
typedef struct T_stmList_ *T_stmList;
struct T_stmList_ {T_stm head; T_stmList tail;};

typedef enum {T_plus, T_minus, T_mul, T_div,
	      T_and, T_or, T_lshift, T_rshift, T_arshift, T_xor,
	      T_eq, T_ne, T_lt, T_gt, T_le, T_ge,
	      T_ult, T_ule, T_ugt, T_uge} T_op;

struct T_stm_ {enum {T_SEQ, T_LABEL, T_JUMP, T_CJUMP, T_MOVE,
		       T_EXP} kind;
	       union {struct {T_stm left, right;} SEQ;
		      Temp_label LABEL;
		      struct {T_exp exp; Temp_labelList jumps;} JUMP;
		      struct {T_exp cond; Temp_label tr, fa;} CJUMP;
		      struct {T_exp dst, src;} MOVE;
		      T_exp EXP;
		    } u;
	     };

struct T_exp_ {enum {T_OP, T_MEM, T_TEMP, T_ESEQ, T_NAME,
		      T_CONST, T_CALL} kind;
	      union {struct {T_op op; T_exp left, right;} OP;
		     T_exp MEM;
		     Temp_temp TEMP;
		     struct {T_stm stm; T_exp exp;} ESEQ;
		     Temp_label NAME;
		     int CONST;
		     struct {T_exp fun; T_stmList args;} CALL;
		   } u;
	    };

T_expList T_ExpList (T_exp head, T_expList tail);
T_stmList T_StmList (T_stm head, T_stmList tail);

T_stm T_Seq(T_stm left, T_stm right);
T_stm T_Label(Temp_label);
T_stm T_Jump(T_exp exp, Temp_labelList labels);
T_stm T_Cjump(T_exp cond, Temp_label tr, Temp_label fa);
T_stm T_Move(T_exp, T_exp);
T_stm T_Exp(T_exp);

T_exp T_Op(T_op, T_exp, T_exp);
T_exp T_Mem(T_exp);
T_exp T_Temp(Temp_temp);
T_exp T_Eseq(T_stm, T_exp);
T_exp T_Name(Temp_label);
T_exp T_Const(int);
T_exp T_Call(T_exp fun, T_stmList args);

T_op T_notRel(T_op);  /* a op b    ==     not(a notRel(op) b)  */
T_op T_commute(T_op); /* a op b    ==    b commute(op) a       */
