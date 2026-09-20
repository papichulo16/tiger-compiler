#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"
#include "types.h"
#include "sem.h"
#include "sem_sym.h"

void sem_trans_decl(A_decList dl);
void sem_trans_fundecl(A_fundecList fl);
void sem_trans_nametyl(A_nametyList nl);
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

  Ty_tyList tyl = NULL;

  if (!el)
    return NULL;

  if (!el->head) 
    goto cleanup;

  tyl = Ty_TyList(sem_trans_exp(el->head), sem_trans_expl(el->tail));

cleanup:
  free(el);
  return tyl;
}

void sem_trans_decl(A_decList dl) {

  if (!dl)
    return;

  if (dl->head) 
    sem_trans_dec(dl->head);

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

void sem_trans_fundecl(A_fundecList fl) {

  if (!fl)
    return;

  if (fl->head) 
    sem_trans_fundec(fl->head);

  sem_trans_fundecl(fl->tail);

  free(fl);
}

void sem_trans_nametyl(A_nametyList nl) {

  if (!nl)
    return;

  if (nl->head) 
    sem_trans_namety(nl->head);

  sem_trans_nametyl(nl->tail);

  free(nl);
}

void sem_trans_field(A_field f) {

  if (!f)
    return;


}

void sem_trans_fundec(A_fundec f) {

  if (!f)
    return;

  // handle
  S_symbol n = f->name;
  S_symbol r = f->result;
  A_fieldList p = f->params; 

  if (!sem_sym_fun_add(f->pos, g_symtab, n, r, p))
    printf("semantic error adding function\n");

  sem_trans_exp(f->body);
  S_endScope(g_symtab);

  free(f);
}

void sem_trans_namety(A_namety n) {

  if (!n)
    return;

  // handle
  if (!sem_sym_ty_add(g_symtab, n->name, n->ty))
    printf("semantic error adding type\n");

  free(n);
}

void sem_trans_dec(A_dec d) {

  if (!d)
    return;

  switch (d->kind) {

    case A_functionDec:

      sem_trans_fundecl(d->u.function);
      break;

    case A_varDec:

      // handle
      S_symbol v = d->u.var.var;
      S_symbol t = d->u.var.typ;
      Ty_ty ty = sem_sym_var_add(d->pos, g_symtab, v, t);

      if (!ty)
        printf("semantic error adding var\n");

      if (!sem_sym_ty_eq(ty, sem_trans_exp(d->u.var.init)))
        EM_semantic_error(d->pos, "incompatible exp type and var type");

      break;

    case A_typeDec:

      sem_trans_nametyl(d->u.type);
      break;
  }

  free(d);
}

Ty_ty sem_trans_var(A_var v) {

  Ty_ty t;

  if (!v)
    return NULL;

  switch(v->kind) {

    case A_simpleVar:

      t = sem_sym_type_get(g_symtab, v->u.simple); 
      break;

    case A_fieldVar:

      t = sem_trans_var(v->u.field.var);
      t = sem_sym_record_ty_get(t, v->u.field.sym);
      break;

    case A_subscriptVar:

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(v->u.subscript.exp)))
        EM_semantic_error(v->pos, "invalid index type");

      t = sem_trans_var(v->u.subscript.var);
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

void rec_def_test(int pos, Ty_ty t, A_efieldList fields) {

  Ty_ty t1;
  Ty_ty t2;

  if (!fields || !fields->head)
    return;

  t1 = sem_sym_record_ty_get(t, fields->head->name);
  t2 = sem_trans_exp(fields->head->exp);

  if (!sem_sym_ty_eq(t1, t2))
    EM_semantic_error(pos, "invalid type for record");

  rec_def_test(pos, t, fields->tail);
}

Ty_ty sem_trans_exp(A_exp e) {

  Ty_ty t = Ty_Void();
  Ty_ty t2 = NULL;
  Ty_tyList tl = NULL;

  if (!e)
    return NULL;

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
      
      if (!sem_sym_is_fun(g_symtab, e->u.call.func)) {
        EM_semantic_error(e->pos, "symbol is not a valid function");

        break;
      }

      t = sem_sym_type_get(g_symtab, e->u.call.func);
      tl = sem_trans_expl(e->u.call.args); 
      cmp_call_params(e->pos, sem_sym_params_get(g_symtab, e->u.call.func), tl);

      break;

    case A_opExp:

      t = sem_trans_exp(e->u.op.left);
      t2 = sem_trans_exp(e->u.op.right);
      
      if (!(oper_is_math(e->u.op.oper) || oper_is_cmp(e->u.op.oper))) 
        break;

      if (!sem_sym_ty_eq(Ty_Int(), t) || !sem_sym_ty_eq(Ty_Int(), t2))
        EM_semantic_error(e->pos, "invalid operation on type");

      break;

    case A_recordExp:

      t = sem_sym_type_get(g_symtab, e->u.record.typ);
      rec_def_test(e->pos, t, e->u.record.fields);

      break;

    case A_seqExp:

      sem_trans_expl(e->u.seq);
      break;

    case A_assignExp:

      if (!sem_sym_ty_eq(sem_trans_var(e->u.assign.var), sem_trans_exp(e->u.assign.exp)))
        EM_semantic_error(e->pos, "invalid types for expression");

      break;

    case A_ifExp:

      sem_trans_exp(e->u.iff.test);
      sem_trans_exp(e->u.iff.then);
      sem_trans_exp(e->u.iff.elsee);

      break;

    case A_whileExp:

      sem_trans_exp(e->u.whilee.test);
      sem_trans_exp(e->u.whilee.body);

      break;

    case A_forExp:


      S_beginScope(g_symtab);

      sem_sym_var_add(e->pos, g_symtab, e->u.forr.var, S_Symbol("int"));

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(e->u.forr.lo)) ||
        sem_sym_ty_eq(Ty_Int(), sem_trans_exp(e->u.forr.hi)))
        EM_semantic_error(e->pos, "invalid for loop lo/hi types");

      sem_trans_exp(e->u.forr.body);

      S_endScope(g_symtab);

      break;

    case A_breakExp:
      break;

    case A_letExp:

      S_beginScope(g_symtab);

      sem_trans_decl(e->u.let.decs);
      sem_trans_exp(e->u.let.body);

      S_endScope(g_symtab);

      break;

    case A_arrayExp:

      t = sem_sym_type_get(g_symtab, e->u.array.typ);

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(e->u.array.size)))
        EM_semantic_error(e->pos, "invalid size type");

      t2 = sem_trans_exp(e->u.array.init);

      if (t->kind != Ty_array) {
        EM_semantic_error(e->pos, "type is not an array");
        break;
      }

      if (!sem_sym_ty_eq(t->u.array, t2))
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

