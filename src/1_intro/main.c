#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "slp.h"
#include "prog1.h"
#include "main.h"

intvar_node_t* g_intvar_stack_h = NULL;

void intvar_cleanup() {

  intvar_node_t* cur = g_intvar_stack_h;
  intvar_node_t* tmp;

  while (cur) {

    tmp = cur;
    cur = (intvar_node_t *) cur->next;

    free(tmp->id);
    free(tmp);
  }

  g_intvar_stack_h = NULL;
}

intvar_node_t* intvar_get(string id) {

  intvar_node_t* cur = g_intvar_stack_h;
  int l = strlen(id);

  while (cur) {

    if (strlen(cur->id) != l)
      goto next;

    if (!strcmp(cur->id, id))
      return cur;

  next:
    cur = (intvar_node_t *) cur->next;
  }

  return NULL;
}

intvar_node_t* intvar_push(string id) {

  intvar_node_t* p = checked_malloc(sizeof(*p));

  p->id = String(id);
  p->val = 0;
  p->next = g_intvar_stack_h;

  g_intvar_stack_h = p;
}

intvar_node_t* intvar(string id) {

  if (intvar_node_t* p = intvar_get(id), p)
    return p;

  return intvar_push(id);
}

int binop(A_binop oper, A_exp left, A_exp right) {

  int l = eval_exp(left);
  int r = eval_exp(right);

  switch (oper) {

    case A_plus:
      return l + r;
    case A_minus:
      return l - r;
    case A_times:
      return l * r;
    case A_div:
      return (int) (l / r);
  }

  printf("Ayo?? What operation is that??\n");
  exit(-1);
}

explist_t* eval_explist(A_expList expl) {

  explist_t* l;

  if (!expl)
    return NULL;

  l = checked_malloc(sizeof(*l));
  l->next = NULL;

  switch(expl->kind) {

    case A_pairExpList: 
      l->val = eval_exp(expl->u.pair.head);
      l->next = eval_explist(expl->u.pair.tail);

      break;

    case A_lastExpList: 
      l->val = eval_exp(expl->u.last);
      break;
  }

  free(expl);
  return l;
}

int eval_exp(A_exp exp) {

  intvar_node_t* v;
  int r;

  if (!exp)
    return 0;

  switch(exp->kind) {

    case A_idExp:
      v = intvar_get(exp->u.id);

      if (!v) {

        printf("Error: var \"%s\" does not exist in expression.\n", exp->u.id);
        exit(-1);
      }

      r = v->val;

      break;

    case A_numExp:
      r = exp->u.num;
      break;

    case A_opExp:
      r = binop(exp->u.op.oper, exp->u.op.left, exp->u.op.right);
      break;

    case A_eseqExp:
      parse_stm(exp->u.eseq.stm);
      r = eval_exp(exp->u.eseq.exp);

      break;
  }

  free(exp);
  return r;
}

int assign(string id, A_exp exp) {

  intvar_node_t* v; 
  int e = eval_exp(exp);

  v = intvar(id);
  v->val = e;

  return e;
}

void handle_print(A_expList expl) {

  explist_t* l = eval_explist(expl);
  explist_t* tmp;

  while (l) {

    printf("%d ", l->val);

    tmp = l;
    l = l->next;

    free(tmp);
  }

  putchar('\n');
}

void parse_stm(A_stm stm) {

  if (!stm)
    return;

  switch (stm->kind) {

    case A_compoundStm:
      parse_stm(stm->u.compound.stm1);
      parse_stm(stm->u.compound.stm2);

      break;

    case A_assignStm:
      assign(stm->u.assign.id, stm->u.assign.exp);

      break;

    case A_printStm:
      handle_print(stm->u.print.exps);
      break;
  }

  free(stm);
}

void interp(A_stm stm) {

  printf("Interpreting program...\n");
  parse_stm(stm);
  intvar_cleanup();
}

int main() {

  interp(prog());

  return 0;
}

