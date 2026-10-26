#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "errormsg.h"
#include "temp.h"
#include "tree.h"
#include "assem.h"
#include "codegen.h"

extern int WORD_SZ;

/* 
 * the book says to have a backtick (`) for temporary registers
 * that chapter 11 will take care of
 * */

AS_instrList g_instr_list = NULL;

static Temp_tempList L(Temp_temp h, Temp_tempList t) { return Temp_TempList(h, t); }

static void emit(AS_instr instr) {

  g_instr_list = AS_InstrList(instr, g_instr_list);
}

static AS_instrList reverse_instr_list(AS_instrList asl, AS_instrList new) {

  AS_instrList reversed;

  if (!asl)
    return new;

  reversed = reverse_instr_list(asl->tail, AS_InstrList(asl->head, new));
  free(asl);

  return reversed;
}

static Temp_temp munch_exp_name(T_exp e) {}

static Temp_temp munch_exp(T_exp e) {

  Temp_temp r = Temp_newtemp(); 

  if (!e)
    goto out;

  switch (e->kind) {

    case T_OP:
      break;

    case T_MEM:
      break;

    case T_TEMP:
      return e->u.TEMP;

    case T_ESEQ:
      break;

    case T_NAME:
      munch_exp_name(e);
      break;

    case T_CONST:
      break;

    case T_CALL:
      break;
  }

out:
  return r;
}

static struct { string ir; string x86; } g_reg_map[] = {
  { "fp", "rbp" },
  { "sp", "rsp" },
  { "r0", "rdi" },
  { "r1", "rsi" },
  { "r2", "rdx" },
  { "r3", "rcx" },
  { "r4", "r8" },
  { "r5", "r9" },
};

static string reg_lookup(T_exp e) {

  string reg = NULL;
  int i;

  if (!e || e->kind != T_NAME)
    goto out;

  for (i = 0; i < sizeof(g_reg_map) / sizeof(g_reg_map[0]); i++) {

    if (e->u.NAME == Temp_namedlabel(g_reg_map[i].ir)) {

      reg = g_reg_map[i].x86;
      break;
    }
  }

out:
  return reg;
}

static string munch_operand(T_exp e, string placeholder, Temp_tempList *list) {

  string reg = reg_lookup(e);

  if (!reg) {

    reg = placeholder;
    *list = L(munch_exp(e), NULL);
  }

  return reg;
}

// improve later to have address offsetting in derefs
static void munch_mov_oper_types(T_exp* dst, T_exp* src, char** fstr) {

  if (*dst && (*dst)->kind == T_MEM) {

    *fstr = "mov qword [%s], %s";
    *dst = (*dst)->u.MEM;
  }
  else if (*src && (*src)->kind == T_MEM) {

    *fstr = "mov %s, qword [%s]";
    *src = (*src)->u.MEM;
  }
}

static void munch_stm_move(T_stm s) {

  T_exp dst = s->u.MOVE.dst;
  T_exp src = s->u.MOVE.src;
  char* fstr = "mov %s, %s";
  char* dst_reg;
  char* src_reg;
  char* instr;
  Temp_tempList dst_list = NULL;
  Temp_tempList src_list = NULL;

  munch_mov_oper_types(&dst, &src, &fstr);

  dst_reg = munch_operand(dst, "`d0", &dst_list);
  src_reg = munch_operand(src, "`s0", &src_list);

  instr = malloc(strlen(fstr) + strlen(dst_reg) + strlen(src_reg) + 1);
  sprintf(instr, fstr, dst_reg, src_reg);

  emit(AS_Move(instr, dst_list, src_list));
}

static bool is_sp(T_exp e) {

  return e && e->kind == T_NAME && e->u.NAME == Temp_namedlabel("sp");
}

// OP(op, NAME sp, CONST WORD_SZ)
static bool is_sp_offset(T_exp e, T_op op) {

  return e && e->kind == T_OP && e->u.OP.op == op && is_sp(e->u.OP.left)
    && e->u.OP.right && e->u.OP.right->kind == T_CONST && e->u.OP.right->u.CONST == WORD_SZ;
}

static bool munch_push_pop(T_stmList stml) {

  T_stm head;
  T_stm tail;
  T_exp head_src;
  T_exp head_dst;
  T_exp tail_src;
  T_exp tail_dst;
  string reg;
  string instr;
  Temp_tempList list = NULL;

  if (!stml->tail || !stml->tail->head)
    return false;

  head = stml->head;
  tail = stml->tail->head;

  if (head->kind != T_MOVE || tail->kind != T_MOVE)
    return false;

  head_src = head->u.MOVE.src;
  head_dst = head->u.MOVE.dst;
  tail_src = tail->u.MOVE.src;
  tail_dst = tail->u.MOVE.dst;

  if (!head_src || !head_dst || !tail_src || !tail_dst)
    return false;

  // push 
  // MOVE(NAME sp, OP(MINUS, NAME sp, CONST 8))
  // MOVE(MEM(NAME sp), exp)
  if (is_sp(head_dst) && is_sp_offset(head_src, T_minus)
      && tail_dst->kind == T_MEM && is_sp(tail_dst->u.MEM)) {

    reg = munch_operand(tail_src, "`s0", &list);
    instr = malloc(strlen("push ") + strlen(reg) + 1);
    sprintf(instr, "push %s", reg);

    emit(AS_Oper(instr, NULL, list, NULL));
    return true;
  }

  // pop
  // MOVE(exp, MEM(NAME sp))
  // MOVE(NAME sp, OP(PLUS, NAME sp, CONST 8))
  if (head_src->kind == T_MEM && is_sp(head_src->u.MEM)
      && is_sp(tail_dst) && is_sp_offset(tail_src, T_plus)) {

    reg = munch_operand(head_dst, "`d0", &list);
    instr = malloc(strlen("pop ") + strlen(reg) + 1);
    sprintf(instr, "pop %s", reg);

    emit(AS_Oper(instr, list, NULL, NULL));
    return true;
  }
  
  return false;
}

static void munch_stm(T_stm s) {

  if (!s)
    return;

  switch (s->kind) {

    case T_SEQ:
      break;

    case T_LABEL:
      break;

    case T_JUMP:
      break;

    case T_CJUMP:
      break;

    case T_MOVE:
      munch_stm_move(s);
      break;

    case T_EXP:
      break;
  }
}

AS_instrList codegen(T_stmList stml) {

  AS_instrList tmp;

  g_instr_list = NULL;

  for (;stml; stml = stml->tail) {

    if (!stml->head)
      continue;

    if (munch_push_pop(stml))
      stml = stml->tail; // to skip two instructions
    else
      munch_stm(stml->head);
  }

  emit(AS_Oper("ret", NULL, NULL, NULL));

  g_instr_list = reverse_instr_list(g_instr_list, NULL);

  tmp = g_instr_list;
  g_instr_list = NULL;

  return tmp;
}

