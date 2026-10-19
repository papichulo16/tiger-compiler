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

bool oper_is_math(A_oper o) {
  return o == A_plusOp || o == A_minusOp || o == A_timesOp || o == A_divideOp;
}

bool oper_is_cmp(A_oper o) {
  return o == A_eqOp || o == A_neqOp || o == A_ltOp || o == A_leOp || o == A_gtOp || o == A_geOp; 
}

Ty_tyList sem_trans_expl(trans_t* tr, A_expList el) {

  Ty_ty t;
  Ty_tyList tyl = NULL;

  if (!el)
    return NULL;

  t = sem_trans_exp(tr, el->head);
  tyl = Ty_TyList(t, sem_trans_expl(tr, el->tail));

  free(el);
  return tyl;
}

Ty_tyList sem_trans_expl_funcall(trans_t* tr, A_expList el, int reg) {

  Ty_ty t;
  Ty_tyList tyl = NULL;

  if (!el)
    return NULL;

  t = sem_trans_exp(tr, el->head);
  trans_funcall_param(tr, reg);

  tyl = Ty_TyList(t, sem_trans_expl_funcall(tr, el->tail, reg+1));

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

bool g_idx = false;

void handle_g_idx(trans_t* t) {

  T_exp left;
  T_exp right;

  if (!g_idx)
    return;

  left = trans_pop_exp(t);
  right = trans_pop_exp(t);

  trans_off_access(t, left, right);

  g_idx = false;
}

Ty_ty sem_trans_var(trans_t* tr, A_var v) {

  Ty_ty t = NULL;
  size_t off;

  if (!v)
    return NULL;

  switch(v->kind) {

    case A_simpleVar:

      t = sem_sym_var_get(g_symtab, v->u.simple); 
      trans_var_access(tr, v->u.simple);
      handle_g_idx(tr);
      break;

    case A_fieldVar:

      t = sem_trans_var(tr, v->u.field.var);

      if (sem_sym_record_field_off(t, v->u.field.sym, &off))
        trans_field_access(tr, off);

      handle_g_idx(tr);

      t = sem_sym_record_ty_get(t, v->u.field.sym);

      break;

    case A_subscriptVar:

      if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(tr, v->u.subscript.exp)))
        EM_semantic_error(v->pos, "invalid index type");

      trans_arr_idx(tr);
      handle_g_idx(tr);
      g_idx = true;

      t = sem_sym_ty_actual(sem_trans_var(tr, v->u.subscript.var));
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

void rec_def_test(int pos, trans_t* tr, Ty_ty ty, Temp_temp rec_tmp, Ty_fieldList rec, A_efieldList fields) {

  Ty_ty t;
  size_t off;
  T_exp src;

  if (!fields) {

    if (rec)
      EM_semantic_error(pos, "insufficient record field amount");

    return;
  }

  t = sem_trans_exp(tr, fields->head->exp);
  src = trans_pop_exp(tr);

  if (!rec)
    EM_semantic_error(pos, "too much record field amount");
  else if (rec->head->name != fields->head->name || !sem_sym_ty_eq(rec->head->ty, t))
    EM_semantic_error(pos, "invalid type for record");

  if (sem_sym_record_field_off(ty, fields->head->name, &off))
    trans_rec_field_init(tr, rec_tmp, off, src);

  rec_def_test(pos, tr, ty, rec_tmp, rec ? rec->tail : NULL, fields->tail);

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

Ty_ty sem_trans_nil_exp(trans_t* tr) {

  trans_int_exp(tr, 0);
  return Ty_Nil();
}

Ty_ty sem_trans_int_exp(trans_t* tr, A_exp e) {

  trans_int_exp(tr, e->u.intt);
  return Ty_Int();
}

Ty_ty sem_trans_call_exp(trans_t* tr, A_exp e) {

  Ty_ty t = Ty_Void();
  Ty_tyList tl = sem_trans_expl_funcall(tr, e->u.call.args, 0);

  if (!sem_sym_is_fun(g_symtab, e->u.call.func)) {

    EM_semantic_error(e->pos, "symbol is not a valid function");
    t = NULL;
  }

  if (t) {

    t = sem_sym_type_get(g_symtab, e->u.call.func);
    cmp_call_params(e->pos, sem_sym_params_get(g_symtab, e->u.call.func), tl);
    trans_funcall(tr, e->u.call.func);
  }

  sem_tyl_free(tl);

  return t;
}

Ty_ty sem_trans_op_exp(trans_t* tr, A_exp e) {

  Ty_ty left;
  Ty_ty right;
  T_exp src;
  T_exp dst;

  left = sem_trans_exp(tr, e->u.op.left);
  src = trans_pop_exp(tr);

  right = sem_trans_exp(tr, e->u.op.right);
  dst = trans_pop_exp(tr);

  if (!oper_types_ok(e->u.op.oper, left, right))
    EM_semantic_error(e->pos, "invalid operation on type");

  trans_oper_exp(tr, e->u.op.oper, src, dst);

  return Ty_Int();
}

Ty_ty sem_trans_record_exp(trans_t* tr, A_exp e) {

  Ty_ty t;
  Ty_ty actual;
  Temp_temp rec_tmp;

  t = sem_sym_ty_get(e->pos, g_symtab, e->u.record.typ);
  actual = sem_sym_ty_actual(t);

  if (actual && actual->kind != Ty_record) {

    EM_semantic_error(e->pos, "type is not a record");
    actual = NULL;
  }

  rec_tmp = trans_rec_alloc(tr, ty_sz(e->pos, g_symtab, t));
  rec_def_test(e->pos, tr, actual, rec_tmp, actual ? actual->u.record : NULL, e->u.record.fields);
  trans_temp_exp(tr, rec_tmp);

  return t;
}

Ty_ty sem_trans_seq_exp(trans_t* tr, A_exp e) {

  Ty_ty t;
  Ty_tyList tl;

  tl = sem_trans_expl(tr, e->u.seq);
  t = sem_tyl_last(tl);
  sem_tyl_free(tl);

  return t;
}

Ty_ty sem_trans_assign_exp(trans_t* tr, A_exp e) {

  Ty_ty var_ty;
  Ty_ty exp_ty;
  T_exp src;
  T_exp dst;

  var_ty = sem_trans_var(tr, e->u.assign.var);
  dst = trans_pop_exp(tr);
  exp_ty = sem_trans_exp(tr, e->u.assign.exp);
  src = trans_pop_exp(tr);

  trans_assign_exp(tr, dst, src);

  if (!sem_sym_ty_eq(var_ty, exp_ty))
    EM_semantic_error(e->pos, "invalid types for expression");

  return Ty_Void();
}

Ty_ty sem_trans_if_exp(trans_t* tr, A_exp e) {

  Ty_ty t = Ty_Void();
  Ty_ty then_ty;
  Ty_ty else_ty;
  T_exp test = NULL;
  T_stm if_body = NULL;
  T_stm else_body = NULL;

  if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(tr, e->u.iff.test)))
    EM_semantic_error(e->pos, "invalid if test type");

  test = trans_pop_exp(tr);

  then_ty = sem_trans_exp(tr, e->u.iff.then);
  if_body = trans_pop_stm(tr);

  if (!e->u.iff.elsee) {

    if (!sem_sym_ty_eq(Ty_Void(), then_ty))
      EM_semantic_error(e->pos, "if without else must not return a value");

    goto out;
  }

  else_ty = sem_trans_exp(tr, e->u.iff.elsee);
  else_body = trans_pop_stm(tr);
  t = then_ty;

  if (!sem_sym_ty_eq(then_ty, else_ty))
    EM_semantic_error(e->pos, "if then and else types differ");

  if (sem_sym_ty_actual(then_ty) == Ty_Nil())
    t = else_ty;

out:
  trans_if(tr, test, if_body, else_body);

  return t;
}

Ty_ty sem_trans_while_exp(trans_t* tr, A_exp e) {

  T_exp test;
  Temp_label start;
  Temp_label end;

  if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(tr, e->u.whilee.test)))
    EM_semantic_error(e->pos, "invalid while test type");

  test = trans_pop_exp(tr);
  start = Temp_newlabel();
  end = Temp_newlabel();

  trans_start_loop(tr, test, start, end);

  if (!sem_sym_ty_eq(Ty_Void(), sem_trans_exp(tr, e->u.whilee.body)))
    EM_semantic_error(e->pos, "while body must not return a value");

  trans_end_loop(tr, start, end);

  return Ty_Void();
}

Ty_ty sem_trans_for_exp(trans_t* tr, A_exp e) {

  Ty_ty lo_ty;
  Ty_ty hi_ty;
  T_exp lo;
  T_exp hi;
  Temp_label end;
  trans_t* t_temp;

  lo_ty = sem_trans_exp(tr, e->u.forr.lo);
  lo = trans_pop_exp(tr);

  hi_ty = sem_trans_exp(tr, e->u.forr.hi);
  hi = trans_pop_exp(tr);

  if (!sem_sym_ty_eq(Ty_Int(), lo_ty) || !sem_sym_ty_eq(Ty_Int(), hi_ty))
    EM_semantic_error(e->pos, "invalid for loop lo/hi types");

  S_beginScope(g_symtab);
  t_temp = trans_new_link_shared_frame(tr);

  sem_sym_var_add(e->pos, t_temp, g_symtab, e->u.forr.var, Ty_Int(), false);

  end = Temp_newlabel();
  trans_tracker_push(end);

  if (!sem_sym_ty_eq(Ty_Void(), sem_trans_exp(t_temp, e->u.forr.body)))
    EM_semantic_error(e->pos, "for body must not return a value");

  trans_for_loop(t_temp, e->u.forr.var, lo, hi, end);
  trans_tracker_pop();

  trans_append_child(tr, t_temp);
  trans_free(&t_temp);
  S_endScope(g_symtab);

  return Ty_Void();
}

Ty_ty sem_trans_array_exp(trans_t* tr, A_exp e) {

  Ty_ty t;
  Ty_ty init_ty;
  Ty_ty actual;
  T_exp hi;
  T_exp body;
  Temp_temp rec_tmp;

  t = sem_sym_ty_get(e->pos, g_symtab, e->u.array.typ);

  if (!sem_sym_ty_eq(Ty_Int(), sem_trans_exp(tr, e->u.array.size)))
    EM_semantic_error(e->pos, "invalid size type");

  // hi
  hi = trans_pop_exp(tr);
  hi = T_Op(T_mul, hi, T_Const(WORD_SZ));

  init_ty = sem_trans_exp(tr, e->u.array.init);
  actual = sem_sym_ty_actual(t);

  // body
  body = trans_pop_exp(tr);

  if (!actual) 
    goto out;

  if (actual->kind != Ty_array) {
    EM_semantic_error(e->pos, "type is not an array");
    goto out;
  }

  if (!sem_sym_ty_eq(actual->u.array, init_ty))
    EM_semantic_error(e->pos, "arr init val not of valid type");

  rec_tmp = trans_arr_alloc(tr, hi, body);
  trans_temp_exp(tr, rec_tmp);

out:
  return t;
}

Ty_ty sem_trans_let_exp(trans_t* tr, A_exp e) {

  Ty_ty t;
  trans_t* t_temp;

  S_beginScope(g_symtab);
  t_temp = trans_new_link(tr);

  sem_trans_decl(t_temp, e->u.let.decs);
  t = sem_trans_exp(t_temp, e->u.let.body);

  trans_prologue(t_temp);
  trans_epilogue(t_temp);
  trans_append_child(tr, t_temp);

  trans_free(&t_temp);
  S_endScope(g_symtab);

  return t;
}

Ty_ty sem_trans_exp(trans_t* tr, A_exp e) {

  Ty_ty t = Ty_Void();

  if (!e)
    return t;

  switch (e->kind) {

    case A_varExp:

      t = sem_trans_var(tr, e->u.var);
      break;

    case A_nilExp:

      t = sem_trans_nil_exp(tr);
      break;

    case A_intExp:

      t = sem_trans_int_exp(tr, e);
      break;

    case A_stringExp:

      t = Ty_String();
      break;

    case A_callExp:

      t = sem_trans_call_exp(tr, e);
      break;

    case A_opExp:

      t = sem_trans_op_exp(tr, e);
      break;

    case A_recordExp:

      t = sem_trans_record_exp(tr, e);
      break;

    case A_seqExp:

      t = sem_trans_seq_exp(tr, e);
      break;

    case A_assignExp:

      t = sem_trans_assign_exp(tr, e);
      break;

    case A_ifExp:

      t = sem_trans_if_exp(tr, e);
      break;

    case A_whileExp:

      t = sem_trans_while_exp(tr, e);
      break;

    case A_forExp:

      t = sem_trans_for_exp(tr, e);
      break;

    case A_breakExp:
      trans_break(tr);
      break;

    case A_letExp:

      t = sem_trans_let_exp(tr, e);
      break;

    case A_arrayExp:

      t = sem_trans_array_exp(tr, e);
      break;
  }

  free(e);
  return t;
}

