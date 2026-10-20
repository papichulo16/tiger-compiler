#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"
#include "types.h"
#include "sem.h"
#include "sem_sym.h"
#include "sem_exp.h"
#include "sem_dec.h"
#include "trans.h"

void sem_trans_fundecl(trans_t* t, A_fundecList fl);
void sem_trans_nametyl(trans_t* t, A_nametyList nl);
void sem_trans_nametyl_chk(A_nametyList nl);
void sem_trans_fieldl(A_fieldList fl);

void sem_trans_field(A_field f);
void sem_trans_fundec(trans_t* t, A_fundec f);
void sem_trans_namety(A_namety n);
void sem_trans_dec(trans_t* t, A_dec d);

// fixing some shitty parser decisions
void sem_dec_merge(A_decList dl) {

  A_decList nx = dl->tail;
  A_nametyList nl;
  A_fundecList fl;

  if (!nx || !nx->head || nx->head->kind != dl->head->kind)
    return;

  switch (dl->head->kind) {

    case A_typeDec:

      for (nl = dl->head->u.type; nl->tail; nl = nl->tail);
      nl->tail = nx->head->u.type;
      break;

    case A_functionDec:

      for (fl = dl->head->u.function; fl->tail; fl = fl->tail);
      fl->tail = nx->head->u.function;
      break;

    case A_varDec:
      return;
  }

  dl->tail = nx->tail;

  free(nx->head);
  free(nx);

  sem_dec_merge(dl);
}

void sem_trans_decl(trans_t* t, A_decList dl) {

  if (!dl)
    return;

  if (dl->head) {

    sem_dec_merge(dl);
    sem_trans_dec(t, dl->head);
  }

  sem_trans_decl(t, dl->tail);

  free(dl);
}

void sem_trans_fieldl(A_fieldList fl) {

  if (!fl)
    return;

  if (fl->head)
    sem_trans_field(fl->head);

  sem_trans_fieldl(fl->tail);

  free(fl);
}

bool fundec_dup(A_fundecList all, A_fundecList cur) {

  for (; all != cur; all = all->tail)
    if (all->head->name == cur->head->name)
      return true;

  return false;
}

void sem_trans_fundecl_head(A_fundecList all, A_fundecList cur) {

  A_fundec f;

  if (!cur)
    return;

  f = cur->head;

  if (fundec_dup(all, cur))
    EM_semantic_error(f->pos, "function '%s' is already defined", S_name(f->name));
  else
    sem_sym_fun_add(f->pos, g_symtab, f->name, f->result, f->params);

  sem_trans_fundecl_head(all, cur->tail);
}

void sem_trans_fundecl(trans_t* t, A_fundecList fl) {

  if (!fl)
    return;

  sem_trans_fundecl_head(fl, fl);

  if (fl->head) 
    sem_trans_fundec(t, fl->head);

  sem_trans_fundecl(t, fl->tail);

  free(fl);
}

bool namety_dup(A_nametyList all, A_nametyList cur) {

  for (; all != cur; all = all->tail)
    if (all->head->name == cur->head->name)
      return true;

  return false;
}

void sem_trans_nametyl_dec(A_nametyList all, A_nametyList cur) {

  A_namety n;

  if (!cur)
    return;

  n = cur->head;

  if (namety_dup(all, cur))
    EM_semantic_error(n->ty->pos, "type '%s' is already defined", S_name(n->name));
  else
    sem_sym_ty_dec(g_symtab, n->name);

  sem_trans_nametyl_dec(all, cur->tail);
}

void sem_trans_nametyl_def(trans_t* t, A_nametyList all, A_nametyList cur) {

  if (!cur)
    return;

  if (!namety_dup(all, cur))
    sem_sym_ty_def(t, g_symtab, cur->head->name, cur->head->ty);

  sem_trans_nametyl_def(t, all, cur->tail);
}

void sem_trans_nametyl(trans_t* t, A_nametyList nl) {

  if (!nl)
    return;

  sem_trans_nametyl_dec(nl, nl);
  sem_trans_nametyl_def(t, nl, nl);
  sem_trans_nametyl_chk(nl);
}

void sem_trans_nametyl_chk(A_nametyList nl) {

  if (!nl)
    return;

  if (nl->head) 
    sem_trans_namety(nl->head);

  sem_trans_nametyl_chk(nl->tail);

  free(nl);
}

void sem_trans_field(A_field f) {

  if (!f)
    return;


}

void sem_trans_fundec(trans_t* t, A_fundec f) {

  Ty_ty res;
  trans_t* tmp;

  if (!f)
    return;

  if (trans_is_stdlib_fun(f->name))
    EM_semantic_error(f->pos, "cant use a stdlib fun name");

  res = sem_sym_type_get(g_symtab, f->name);
  tmp = sem_sym_fun_scope_begin(t, g_symtab, f->params);

  if (!sem_sym_ty_eq(res, sem_trans_exp(tmp, f->body)))
    EM_semantic_error(f->pos, "function body type does not match its return type");

  trans_prologue(tmp, true);
  trans_epilogue(tmp, true);
  trans_add_fun(t, tmp, f->name);

  S_endScope(g_symtab);
  trans_free(&tmp);
  free(f);
}

void sem_trans_namety(A_namety n) {

  if (!n)
    return;

  sem_sym_ty_cycle_chk(n->ty->pos, g_symtab, n->name);

  free(n);
}

void sem_trans_vardec(trans_t* tr, A_dec d) {

  Ty_ty ty = NULL;
  Ty_ty init;
  T_exp src;
  T_exp dst;

  init = sem_trans_exp(tr, d->u.var.init);

  if (!d->u.var.typ) {

    ty = init;

    if (sem_sym_ty_actual(init) == Ty_Nil())
      EM_semantic_error(d->pos, "nil init needs a record type");
  }

  if (sem_sym_inuse(g_symtab, d->u.var.var))
    EM_semantic_error(d->pos, "variable '%s' is already defined", S_name(d->u.var.var));

  if (d->u.var.typ) {

    ty = sem_sym_ty_get(d->pos, g_symtab, d->u.var.typ);

    if (!sem_sym_ty_eq(ty, init))
      EM_semantic_error(d->pos, "bad exp type and var type");
  }

  src = trans_pop_exp(tr);
  sem_sym_var_add(d->pos, tr, g_symtab, d->u.var.var, ty, false);

  trans_var_access(tr, d->u.var.var);
  dst = trans_pop_exp(tr);
  trans_assign_exp(tr, dst, src);
}

void sem_trans_dec(trans_t* t, A_dec d) {

  if (!d)
    return;

  switch (d->kind) {

    case A_functionDec:

      sem_trans_fundecl(t, d->u.function);
      break;


    case A_varDec:

      sem_trans_vardec(t, d);
      break;


    case A_typeDec:

      sem_trans_nametyl(t, d->u.type);
      break;

  }

  free(d);
}

