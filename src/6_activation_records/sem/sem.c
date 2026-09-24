#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"
#include "parse.h"
#include "prabsyn.h"
#include "types.h"
#include "sem.h"
#include "sem_sym.h"

void sem_trans_decl(A_decList dl);
void sem_trans_fundecl(A_fundecList fl);
void sem_trans_nametyl(A_nametyList nl);
void sem_trans_nametyl_chk(A_nametyList nl);
void sem_trans_fieldl(A_fieldList fl);
Ty_tyList sem_trans_expl(A_expList el);

void sem_trans_field(A_field f);
void sem_trans_fundec(A_fundec f);
void sem_trans_namety(A_namety n);
void sem_trans_dec(A_dec d);
Ty_ty sem_trans_exp(A_exp e);

S_table g_symtab = NULL;

bool oper_is_math(A_oper o) {
  return o == A_plusOp || o == A_minusOp || o == A_timesOp || o == A_divideOp;
}

bool oper_is_cmp(A_oper o) {
  return o == A_eqOp || o == A_neqOp || o == A_ltOp || o == A_leOp || o == A_gtOp || o == A_geOp; 
}

Ty_tyList sem_trans_expl(A_expList el) {

  Ty_ty t;
  Ty_tyList tyl = NULL;

  if (!el)
    return NULL;

  t = sem_trans_exp(el->head);
  tyl = Ty_TyList(t, sem_trans_expl(el->tail));

  free(el);
  return tyl;
}

Ty_ty sem_tyl_last(Ty_tyList tl) {

  if (!tl)
    return Ty_Void();

  if (!tl->tail)
    return tl->head;

  return sem_tyl_last(tl->tail);
}

void sem_tyl_free(Ty_tyList tl) {

  if (!tl)
    return;

  sem_tyl_free(tl->tail);
  free(tl);
}

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

void sem_trans_decl(A_decList dl) {

  if (!dl)
    return;

  if (dl->head) {

    sem_dec_merge(dl);
    sem_trans_dec(dl->head);
  }

  sem_trans_decl(dl->tail);

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

void sem_trans_fundecl(A_fundecList fl) {

  if (!fl)
    return;

  sem_trans_fundecl_head(fl, fl);

  if (fl->head) 
    sem_trans_fundec(fl->head);

  sem_trans_fundecl(fl->tail);

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

void sem_trans_nametyl_def(A_nametyList all, A_nametyList cur) {

  if (!cur)
    return;

  if (!namety_dup(all, cur))
    sem_sym_ty_def(g_symtab, cur->head->name, cur->head->ty);

  sem_trans_nametyl_def(all, cur->tail);
}

void sem_trans_nametyl(A_nametyList nl) {

  if (!nl)
    return;

  sem_trans_nametyl_dec(nl, nl);
  sem_trans_nametyl_def(nl, nl);
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

void sem_trans_fundec(A_fundec f) {

  Ty_ty res;

  if (!f)
    return;

  res = sem_sym_type_get(g_symtab, f->name);

  sem_sym_fun_scope_begin(g_symtab, f->params);

  if (!sem_sym_ty_eq(res, sem_trans_exp(f->body)))
    EM_semantic_error(f->pos, "function body type does not match its return type");

  S_endScope(g_symtab);

  free(f);
}

void sem_trans_namety(A_namety n) {

  if (!n)
    return;

  sem_sym_ty_cycle_chk(n->ty->pos, g_symtab, n->name);

  free(n);
}

void sem_trans_vardec(A_dec d) {

  Ty_ty ty = NULL;
  Ty_ty init = sem_trans_exp(d->u.var.init);

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

  sem_sym_var_add(g_symtab, d->u.var.var, ty);
}

void sem_trans_dec(A_dec d) {

  if (!d)
    return;

  switch (d->kind) {

    case A_functionDec:

      sem_trans_fundecl(d->u.function);
      break;

    case A_varDec:

      sem_trans_vardec(d);
      break;

    case A_typeDec:

      sem_trans_nametyl(d->u.type);
      break;
  }

  free(d);
}

Ty_ty sem_trans_var(A_var v) {

  Ty_ty t = NULL;

  if (!v)
    return NULL;

  switch(v->kind) {

    case A_simpleVar:

      t = sem_sym_var_get(g_symtab, v->u.simple); 
      break;

    case A_fieldVar:

      t = sem_trans_var(v->u.field.var);
      t = sem_sym_record_ty_get(t, v->u.field.sym);
      break;

    case A_subscriptVar:

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(v->u.subscript.exp)))
        EM_semantic_error(v->pos, "invalid index type");

      t = sem_sym_ty_actual(sem_trans_var(v->u.subscript.var));
      t = t && t->kind == Ty_array ? t->u.array : NULL;
      break;
  }

  if (!t)
    EM_semantic_error(v->pos, "bad variable expression");
  
  free(v);
  return t;
}

void cmp_call_params(int pos, Ty_fieldList rec, Ty_tyList call) {

  if (!call && rec) {

    EM_semantic_error(pos, "insufficient call param amount");
    return;
  }

  if (call && !rec) {

    EM_semantic_error(pos, "too much call param amount");
    return;
  }

  if (!call && !rec)
    return;

  if (!sem_sym_ty_eq(rec->head->ty, call->head))
    EM_semantic_error(pos, "invalid call param type");

  cmp_call_params(pos, rec->tail, call->tail);
}

void rec_def_test(int pos, Ty_fieldList rec, A_efieldList fields) {

  Ty_ty t;

  if (!fields) {

    if (rec)
      EM_semantic_error(pos, "insufficient record field amount");

    return;
  }

  t = sem_trans_exp(fields->head->exp);

  if (!rec)
    EM_semantic_error(pos, "too much record field amount");
  else if (rec->head->name != fields->head->name || !sem_sym_ty_eq(rec->head->ty, t))
    EM_semantic_error(pos, "invalid type for record");

  rec_def_test(pos, rec ? rec->tail : NULL, fields->tail);

  free(fields->head);
  free(fields);
}

bool oper_types_ok(A_oper o, Ty_ty l, Ty_ty r) {

  Ty_ty a;

  if (oper_is_math(o) || o == A_andOp || o == A_orOp)
    return sem_sym_ty_eq(Ty_Int(), l) && sem_sym_ty_eq(Ty_Int(), r);

  if (!sem_sym_ty_eq(l, r))
    return false;

  a = sem_sym_ty_actual(l);

  if (o == A_eqOp || o == A_neqOp)
    return a->kind != Ty_void;

  return a->kind == Ty_int || a->kind == Ty_string;
}

Ty_ty sem_trans_exp(A_exp e) {

  Ty_ty t = Ty_Void();
  Ty_ty t2 = NULL;
  Ty_ty t3 = NULL;
  Ty_tyList tl = NULL;

  if (!e)
    return t;

  switch (e->kind) {

    case A_varExp:

      t = sem_trans_var(e->u.var);
      break;

    case A_nilExp:

      t = Ty_Nil();
      break;

    case A_intExp:

      t = Ty_Int();
      break;

    case A_stringExp:

      t = Ty_String();
      break;

    case A_callExp:
      
      tl = sem_trans_expl(e->u.call.args); 

      if (!sem_sym_is_fun(g_symtab, e->u.call.func)) {

        EM_semantic_error(e->pos, "symbol is not a valid function");
        t = NULL;
      }

      if (t) {

        t = sem_sym_type_get(g_symtab, e->u.call.func);
        cmp_call_params(e->pos, sem_sym_params_get(g_symtab, e->u.call.func), tl);
      }

      sem_tyl_free(tl);

      break;

    case A_opExp:

      t = sem_trans_exp(e->u.op.left);
      t2 = sem_trans_exp(e->u.op.right);

      if (!oper_types_ok(e->u.op.oper, t, t2))
        EM_semantic_error(e->pos, "invalid operation on type");

      t = Ty_Int();

      break;

    case A_recordExp:

      t = sem_sym_ty_get(e->pos, g_symtab, e->u.record.typ);
      t2 = sem_sym_ty_actual(t);

      if (t2 && t2->kind != Ty_record) {

        EM_semantic_error(e->pos, "type is not a record");
        t2 = NULL;
      }

      rec_def_test(e->pos, t2 ? t2->u.record : NULL, e->u.record.fields);

      break;

    case A_seqExp:

      tl = sem_trans_expl(e->u.seq);
      t = sem_tyl_last(tl);
      sem_tyl_free(tl);

      break;

    case A_assignExp:

      t = sem_trans_var(e->u.assign.var);
      t2 = sem_trans_exp(e->u.assign.exp);

      if (!sem_sym_ty_eq(t, t2))
        EM_semantic_error(e->pos, "invalid types for expression");

      t = Ty_Void();

      break;

    case A_ifExp:

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(e->u.iff.test)))
        EM_semantic_error(e->pos, "invalid if test type");

      t2 = sem_trans_exp(e->u.iff.then);

      if (!e->u.iff.elsee) {

        if (!sem_sym_ty_eq(Ty_Void(), t2))
          EM_semantic_error(e->pos, "if without else must not return a value");

        break;
      }

      t3 = sem_trans_exp(e->u.iff.elsee);
      t = t2;

      if (!sem_sym_ty_eq(t2, t3))
        EM_semantic_error(e->pos, "if then and else types differ");

      if (sem_sym_ty_actual(t2) == Ty_Nil())
        t = t3;

      break;

    case A_whileExp:

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(e->u.whilee.test)))
        EM_semantic_error(e->pos, "invalid while test type");

      if (!sem_sym_ty_eq(Ty_Void(), sem_trans_exp(e->u.whilee.body)))
        EM_semantic_error(e->pos, "while body must not return a value");

      break;

    case A_forExp:

      t2 = sem_trans_exp(e->u.forr.lo);
      t3 = sem_trans_exp(e->u.forr.hi);

      if (!sem_sym_ty_eq(Ty_Int(), t2) || !sem_sym_ty_eq(Ty_Int(), t3))
        EM_semantic_error(e->pos, "invalid for loop lo/hi types");

      S_beginScope(g_symtab);

      sem_sym_var_add(g_symtab, e->u.forr.var, Ty_Int());

      if (!sem_sym_ty_eq(Ty_Void(), sem_trans_exp(e->u.forr.body)))
        EM_semantic_error(e->pos, "for body must not return a value");

      S_endScope(g_symtab);

      break;

    case A_breakExp:
      break;

    case A_letExp:

      S_beginScope(g_symtab);

      sem_trans_decl(e->u.let.decs);
      t = sem_trans_exp(e->u.let.body);

      S_endScope(g_symtab);

      break;

    case A_arrayExp:

      t = sem_sym_ty_get(e->pos, g_symtab, e->u.array.typ);

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(e->u.array.size)))
        EM_semantic_error(e->pos, "invalid size type");

      t2 = sem_trans_exp(e->u.array.init);
      t3 = sem_sym_ty_actual(t);

      if (!t3) 
        break;

      if (t3->kind != Ty_array) {
        EM_semantic_error(e->pos, "type is not an array");
        break;
      }

      if (!sem_sym_ty_eq(t3->u.array, t2))
        EM_semantic_error(e->pos, "arr init val not of valid type");

      break;
  }

  free(e);
  return t;
}

void sem_trans_prog(A_exp e) {

  if (!e)
    return;

  g_symtab = S_empty();
  sem_sym_std_add(g_symtab);

  sem_trans_exp(e);

  if (g_symtab) {

    free(g_symtab);
    g_symtab = NULL;
  }
}

void comp_err() {

  printf("Compilation error\n");
  exit(-1);
}

int main(int argc, char** argv) {

  A_exp root;
  FILE* fd;

  if (argc < 2) {
    
    fprintf(stderr,"usage: %s filename\n", argv[0]); 
    exit(1);
  }

  root = parse(argv[1]);

  if (!root)
    comp_err();

  fd = fopen("./out", "w+");
  pr_exp(fd, root, 10);
  fclose(fd);

  sem_trans_prog(root);

  if (EM_err_count)
    comp_err();

  return 0;
}

