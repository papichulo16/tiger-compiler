#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "errormsg.h"
#include "temp.h"
#include "tree.h"
#include "assem.h"
#include "codegen.h"

// 0x7fffffffffffffff = 9223372036854775807
// that is 19 digits, 18 for safe measures
#define CONST_SZ_MAX_BYTES 18

extern int WORD_SZ;
static Temp_temp munch_exp(T_exp e);
static void munch_stm(T_stm s);

/* 
 * the book says to have a backtick (`) for temporary registers
 * that chapter 11 will take care of
 * */

AS_instrList g_instr_list = NULL;

static Temp_tempList L(Temp_temp h, Temp_tempList t) { return Temp_TempList(h, t); }
static Temp_labelList LL(Temp_label h, Temp_labelList t) { return Temp_LabelList(h, t); }

char* strcat_malloc(char* dst, char* src) {
  char* t = malloc(strlen(dst) + strlen(src) + 1);

  strcpy(t, dst);
  strcat(t, src);

  return t;
}

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

static struct { char* ir; char* x86; } g_reg_map[] = {
  { "fp", "rbp" },
  { "sp", "rsp" },
  { "r0", "rdi" },
  { "r1", "rsi" },
  { "r2", "rdx" },
  { "r3", "rcx" },
  { "r4", "r8" },
  { "r5", "r9" },
};

static char* reg_lookup(T_exp e) {

  char* reg = NULL;
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

bool g_name_exp = false;

static char* munch_operand(T_exp e, char* placeholder, Temp_tempList *list) {

  char* reg = reg_lookup(e);

  if (!reg && e->kind == T_CONST) {

    reg = malloc(CONST_SZ_MAX_BYTES);
    snprintf(reg, CONST_SZ_MAX_BYTES, "%d", e->u.CONST);
  }

  // need to be careful if it calls already from the exp name
  // the munch_exp(e) will cause infinite recursion
  if (!reg && !g_name_exp) {

    reg = placeholder;
    *list = L(munch_exp(e), NULL);
  }

  return reg;
}

static Temp_temp munch_exp_name(T_exp e) {


  Temp_temp r = Temp_newtemp();
  Temp_tempList l = NULL;
  char* reg;
  char* buf = malloc(CONST_SZ_MAX_BYTES*2);

  g_name_exp = true;
  reg = munch_operand(e, "`s0", &l);
  g_name_exp = false;

  if (reg)
    snprintf(buf, CONST_SZ_MAX_BYTES*2, "mov `d0, %s", reg);
  else
    snprintf(buf, CONST_SZ_MAX_BYTES*2, "mov `d0, %s", Temp_labelstring(e->u.NAME));

  emit(AS_Move(buf, L(r, NULL), l));

  return r;
}

Temp_label g_true_block = NULL;

// idiv only divides rdx:rax
static void munch_exp_div(Temp_temp r, Temp_temp divisor) {

  emit(AS_Oper("push rdx", NULL, NULL, NULL));
  emit(AS_Move("mov rax, `s0", NULL, L(r, NULL)));
  emit(AS_Oper("cqo", NULL, NULL, NULL));
  emit(AS_Oper("idiv `s0", NULL, L(divisor, NULL), NULL));
  emit(AS_Oper("pop rdx", NULL, NULL, NULL));
  emit(AS_Move("mov `d0, rax", L(r, NULL), NULL));
}

static Temp_temp munch_exp_op(T_exp e) {

  Temp_temp r;
  T_exp left = e->u.OP.left;
  T_exp right = e->u.OP.right;
  T_op op = e->u.OP.op;

  //emit(AS_Move("mov `d0, `s0", L(r, NULL), L(munch_exp(left), NULL)));
  r = munch_exp(left);

  switch (op) {

    case T_plus:
      emit(AS_Move("add `d0, `s0", L(r, NULL), L(munch_exp(right), NULL)));
      break;

    case T_minus:
      emit(AS_Move("sub `d0, `s0", L(r, NULL), L(munch_exp(right), NULL)));
      break;

    case T_mul:
      emit(AS_Move("imul `d0, `s0", L(r, NULL), L(munch_exp(right), NULL)));
      break;

    case T_div:
      munch_exp_div(r, munch_exp(right));
      break;

    case T_and:
    case T_or:
      printf("and or or not implemented\n");
      break;

    case T_lshift:
    case T_rshift:
    case T_xor:
      printf("bit math not implemented\n");
      break;

    case T_eq:

      emit(AS_Oper("cmp `d0, `s0", L(r, NULL), L(munch_exp(right), NULL), NULL));
      emit(AS_Oper(strcat_malloc("je ", Temp_labelstring(g_true_block)), NULL, NULL, AS_Targets(LL(g_true_block, NULL))));
      break;

    case T_ne:

      emit(AS_Oper("cmp `d0, `s0", L(r, NULL), L(munch_exp(right), NULL), NULL));
      emit(AS_Oper(strcat_malloc("jne ", Temp_labelstring(g_true_block)), NULL, NULL, AS_Targets(LL(g_true_block, NULL))));
      break;

    case T_lt:

      emit(AS_Oper("cmp `d0, `s0", L(r, NULL), L(munch_exp(right), NULL), NULL));
      emit(AS_Oper(strcat_malloc("jl ", Temp_labelstring(g_true_block)), NULL, NULL, AS_Targets(LL(g_true_block, NULL))));
      break;

    case T_gt:

      emit(AS_Oper("cmp `d0, `s0", L(r, NULL), L(munch_exp(right), NULL), NULL));
      emit(AS_Oper(strcat_malloc("jg ", Temp_labelstring(g_true_block)), NULL, NULL, AS_Targets(LL(g_true_block, NULL))));
      break;

    case T_le:

      emit(AS_Oper("cmp `d0, `s0", L(r, NULL), L(munch_exp(right), NULL), NULL));
      emit(AS_Oper(strcat_malloc("jle ", Temp_labelstring(g_true_block)), NULL, NULL, AS_Targets(LL(g_true_block, NULL))));
      break;

    case T_ge:

      emit(AS_Oper("cmp `d0, `s0", L(r, NULL), L(munch_exp(right), NULL), NULL));
      emit(AS_Oper(strcat_malloc("jge ", Temp_labelstring(g_true_block)), NULL, NULL, AS_Targets(LL(g_true_block, NULL))));
      break;
  }

  return r;
}

static void munch_call_params(T_stmList args) {

  if (!args)
    return;

  munch_call_params(args->tail);
  munch_stm(args->head);
}

static Temp_temp munch_exp_call(T_exp e) {

  T_exp fun = e->u.CALL.fun;
  T_stmList args = e->u.CALL.args;
  Temp_temp r = Temp_newtemp(); 
  char* buf;
  size_t buf_sz;

  // fun should never not be a name label hopefully
  if (fun->kind != T_NAME) {

    fprintf(stderr, "fun not a name??? why??\n");
    exit(-1);
  }

  buf_sz = strlen(Temp_labelstring(fun->u.NAME)) + 8;
  buf = malloc(buf_sz);
  snprintf(buf, buf_sz, "call %s", Temp_labelstring(fun->u.NAME));

  munch_call_params(args);
  emit(AS_Oper(buf, NULL, NULL, NULL));
  emit(AS_Move("mov `d0, rax", L(r, NULL), NULL));

  return r;
}

static Temp_temp munch_exp(T_exp e) {

  Temp_temp r = Temp_newtemp(); 
  char* tmp;

  if (!e)
    goto out;

  switch (e->kind) {

    case T_OP:
      r = munch_exp_op(e);
      break;

    case T_MEM:

      emit(AS_Move("mov `d0, qword [`s0]", L(r, NULL), L(munch_exp(e->u.MEM), NULL)));
      break;

    case T_TEMP:
      return e->u.TEMP;

    case T_ESEQ:
      printf("does this also exist?\n");
      break;

    case T_NAME:
      r = munch_exp_name(e);
      break;

    case T_CONST:

      tmp = malloc(CONST_SZ_MAX_BYTES*2);
      snprintf(tmp, CONST_SZ_MAX_BYTES*2, "mov `d0, %d", e->u.CONST);
      emit(AS_Move(tmp, L(r, NULL), NULL));
      break;

    case T_CALL:
      r = munch_exp_call(e);
      break;
  }

out:
  return r;
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

  // math with a dst same as oper
  // MOVE(NAME X, OP(OP, NAME X, exp))
  else if (*src && *dst && (*dst)->kind == T_NAME && (*src)->kind == T_OP &&
      (*src)->u.OP.left->kind == T_NAME && (*src)->u.OP.left->u.NAME == (*dst)->u.NAME) {

    switch ((*src)->u.OP.op) {

      case T_plus:

        *fstr = "add %s, %s";
        *src = (*src)->u.OP.right;
        break;

      case T_minus:

        *fstr = "sub %s, %s";
        *src = (*src)->u.OP.right;
        break;

      case T_mul:

        *fstr = "imul %s, %s";
        *src = (*src)->u.OP.right;
        break;

      default:
        break;
    }
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

static void munch_stm_cjump(T_stm s) {

  T_exp cond = s->u.CJUMP.cond;
  Temp_label tr = s->u.CJUMP.tr;
  Temp_label fa = s->u.CJUMP.fa;

  g_true_block = tr;
  munch_exp(cond);
}

static void munch_stm(T_stm s) {

  char* tmp;
  size_t tmp_sz;

  if (!s)
    return;

  switch (s->kind) {

    case T_SEQ:
      printf("does this ever hit?\n");
      break;

    case T_LABEL:

      tmp_sz = strlen(Temp_labelstring(s->u.LABEL)) + 2;
      tmp = malloc(tmp_sz);

      snprintf(tmp, tmp_sz, "%s:", Temp_labelstring(s->u.LABEL));
      emit(AS_Label(tmp, s->u.LABEL));
      break;

    case T_JUMP:

      tmp_sz = strlen(Temp_labelstring(s->u.JUMP.exp->u.NAME)) + 8;
      tmp = malloc(tmp_sz);

      snprintf(tmp, tmp_sz, "jmp %s", Temp_labelstring(s->u.JUMP.exp->u.NAME));
      emit(AS_Oper(tmp, NULL, NULL, AS_Targets(s->u.JUMP.jumps)));

      break;

    case T_CJUMP:
      munch_stm_cjump(s);
      break;

    case T_MOVE:
      munch_stm_move(s);
      break;

    case T_EXP:

      // assuming and exp stm is a retval
      emit(AS_Move("mov rax, `s0", NULL, L(munch_exp(s->u.EXP), NULL)));
      break;
  }
}

static bool is_sp(T_exp e) {

  return e && e->kind == T_NAME && e->u.NAME == Temp_namedlabel("sp"); }

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
  char* reg;
  char* instr;
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

