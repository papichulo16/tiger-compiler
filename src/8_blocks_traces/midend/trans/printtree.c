/*
 * printtree.c - functions to print out intermediate representation (IR) trees.
 *
 * SEQ chains are flattened so each statement gets its own line, labels sit
 * at column 0 like assembly, and each statement's expression tree is
 * printed inline on that line.
 */
#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "printtree.h"

static void pr_exp(FILE *out, T_exp exp);
static void pr_stm_inline(FILE *out, T_stm stm);

static const char *safe_str(const char *s) {
  return s ? s : "NULL";
}

static const char *safe_sym(S_symbol sym) {
  return sym ? safe_str(S_name(sym)) : "NULL";
}

static const char *safe_temp(Temp_temp t) {
  return t ? safe_str(Temp_look(Temp_name(), t)) : "NULL";
}

static char oper[][12] = {
  "PLUS", "MINUS", "TIMES", "DIVIDE",
  "AND", "OR", "LSHIFT", "RSHIFT", "ARSHIFT", "XOR",
  "EQ", "NE", "LT", "GT", "LE", "GE", "ULT", "ULE", "UGT", "UGE"};

static void pr_exp(FILE *out, T_exp exp)
{
  if (!exp) {
    fprintf(out, "NULL");
  } else switch (exp->kind) {
  case T_OP:
    fprintf(out, "OP(%s, ", oper[exp->u.OP.op]);
    pr_exp(out, exp->u.OP.left);
    fprintf(out, ", ");
    pr_exp(out, exp->u.OP.right);
    fprintf(out, ")");
    break;
  case T_MEM:
    fprintf(out, "MEM(");
    pr_exp(out, exp->u.MEM);
    fprintf(out, ")");
    break;
  case T_TEMP:
    fprintf(out, "TEMP t%s", safe_temp(exp->u.TEMP));
    break;
  case T_ESEQ:
    fprintf(out, "ESEQ({ ");
    pr_stm_inline(out, exp->u.ESEQ.stm);
    fprintf(out, " }, ");
    pr_exp(out, exp->u.ESEQ.exp);
    fprintf(out, ")");
    break;
  case T_NAME:
    fprintf(out, "NAME %s", safe_sym(exp->u.NAME));
    break;
  case T_CONST:
    fprintf(out, "CONST %d", exp->u.CONST);
    break;
  case T_CALL:
    fprintf(out, "CALL(");
    pr_exp(out, exp->u.CALL.fun);
    for (T_stmList args = exp->u.CALL.args; args; args = args->tail) {
      fprintf(out, ", ");
      pr_stm_inline(out, args->head);
    }
    fprintf(out, ")");
    break;
  }
}

/* prints a single non-SEQ statement with no indent or newline */
static void pr_stm_body(FILE *out, T_stm stm)
{
  switch (stm->kind) {
  case T_SEQ:
    pr_stm_inline(out, stm);
    break;
  case T_LABEL:
    fprintf(out, "%s:", safe_sym(stm->u.LABEL));
    break;
  case T_JUMP:
    fprintf(out, "JUMP(");
    pr_exp(out, stm->u.JUMP.exp);
    fprintf(out, ")");
    break;
  case T_CJUMP:
    fprintf(out, "CJUMP(");
    pr_exp(out, stm->u.CJUMP.cond);
    fprintf(out, ", %s, %s)", safe_sym(stm->u.CJUMP.tr), safe_sym(stm->u.CJUMP.fa));
    break;
  case T_MOVE:
    fprintf(out, "MOVE(");
    pr_exp(out, stm->u.MOVE.dst);
    fprintf(out, ", ");
    pr_exp(out, stm->u.MOVE.src);
    fprintf(out, ")");
    break;
  case T_EXP:
    fprintf(out, "EXP(");
    pr_exp(out, stm->u.EXP);
    fprintf(out, ")");
    break;
  }
}

/* NULL statements are treated as no-ops inside SEQs so they don't clutter the output */
static int is_noop(T_stm stm)
{
  return stm == NULL;
}

/* used inside ESEQ, where statements must stay on the expression's line */
static void pr_stm_inline(FILE *out, T_stm stm)
{
  if (is_noop(stm)) {
    fprintf(out, "NULL");
  } else if (stm->kind != T_SEQ) {
    pr_stm_body(out, stm);
  } else if (is_noop(stm->u.SEQ.left)) {
    pr_stm_inline(out, stm->u.SEQ.right);
  } else if (is_noop(stm->u.SEQ.right)) {
    pr_stm_inline(out, stm->u.SEQ.left);
  } else {
    pr_stm_inline(out, stm->u.SEQ.left);
    fprintf(out, "; ");
    pr_stm_inline(out, stm->u.SEQ.right);
  }
}

static void pr_stm_lines(FILE *out, T_stm stm)
{
  if (is_noop(stm)) {
    /* nothing to print */
  } else if (stm->kind == T_SEQ) {
    pr_stm_lines(out, stm->u.SEQ.left);
    pr_stm_lines(out, stm->u.SEQ.right);
  } else {
    if (stm->kind != T_LABEL) fprintf(out, "    ");
    pr_stm_body(out, stm);
    fprintf(out, "\n");
  }
}

void printStmList (FILE *out, T_stmList stmList)
{
  for (; stmList; stmList = stmList->tail) {
    pr_stm_lines(out, stmList->head);
  }
}

