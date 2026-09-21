#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"
#include "types.h"
#include "sem_sym.h"

void* sem_sym_get(S_table symtab, S_symbol name) {
  return S_look(symtab, name);
}

bool sem_sym_is_fun(S_table symtab, S_symbol name) {
  symtab_id_t* id = (symtab_id_t *) sem_sym_get(symtab, name);

  return id ? id->kind == ST_FUN : false;
}

Ty_fieldList sem_sym_params_get(S_table symtab, S_symbol name) {

  symtab_id_t* id = sem_sym_get(symtab, name);

  if (!id)
    return NULL;

  return id->params;
}

Ty_ty sem_sym_type_get(S_table symtab, S_symbol name) {

  symtab_id_t* id = sem_sym_get(symtab, name);

  if (!id)
    return NULL;

  return id->ty;
}

Ty_ty sem_sym_var_get(S_table symtab, S_symbol name) {

  symtab_id_t* id = sem_sym_get(symtab, name);

  return id && id->kind == ST_VAR ? id->ty : NULL;
}

Ty_ty sem_sym_ty_actual(Ty_ty t) {

  while (t && t->kind == Ty_name)
    t = t->u.name.ty;

  return t;
}

Ty_ty handle_ty_sym(S_table symtab, S_symbol ty) {

  char* s = S_name(ty);
  symtab_id_t* id;

  if ((!strcmp(s, "int") || !strcmp(s, "INT")) && strlen(s) == 3)
    return Ty_Int();

  if ((!strcmp(s, "string") || !strcmp(s, "STRING")) && strlen(s) == 6)
    return Ty_String();

  id = sem_sym_get(symtab, ty);

  return id && id->kind == ST_TY ? id->ty : NULL;
}

Ty_ty sem_sym_ty_get(int pos, S_table symtab, S_symbol name) {

  Ty_ty t = handle_ty_sym(symtab, name);

  if (!t)
    EM_semantic_error(pos, "type '%s' does not exist", S_name(name));

  return t;
}

Ty_fieldList handle_ty_record(int pos, S_table symtab, A_fieldList fl) {

  Ty_ty ty;
  Ty_field f;

  if (!fl)
    return NULL;

  ty = handle_ty_sym(symtab, fl->head->typ);

  if (!ty)
    EM_semantic_error(pos, "record type '%s' does not exist", S_name(fl->head->typ));

  f = Ty_Field(fl->head->name, ty);

  return Ty_FieldList(f, handle_ty_record(pos, symtab, fl->tail));
}

void handle_ty_params(S_table symtab, A_fieldList fl) {

  if (!fl)
    return;

  sem_sym_var_add(symtab, fl->head->name, handle_ty_sym(symtab, fl->head->typ));

  handle_ty_params(symtab, fl->tail);
}

Ty_field fl_field_get(Ty_fieldList fl, S_symbol s) {

  if (!fl)
    return NULL;

  if (!strcmp(S_name(fl->head->name), S_name(s)) &&
      strlen(S_name(fl->head->name)) == strlen(S_name(s)))
    return fl->head;

  return fl_field_get(fl->tail, s);
}

Ty_ty sem_sym_record_ty_get(Ty_ty rec, S_symbol sub) {

  Ty_field f;

  rec = sem_sym_ty_actual(rec);

  if (!rec || rec->kind != Ty_record)
    return NULL;

  return f = fl_field_get(rec->u.record, sub),
         f ? f->ty : NULL;
}

Ty_ty parse_ty(S_table symtab, A_ty ty) {

  Ty_ty t = NULL;

  if (!ty)
    return NULL;

  switch (ty->kind) {

    case A_nameTy:

      t = sem_sym_ty_get(ty->pos, symtab, ty->u.name);
      break;

    case A_recordTy:

      t = Ty_Record(handle_ty_record(ty->pos, symtab, ty->u.record));
      break;

    case A_arrayTy:

      t = sem_sym_ty_get(ty->pos, symtab, ty->u.array);

      if (t)
        t = Ty_Array(t);

      break;
  }

  return t;
}

bool sem_sym_ty_eq(Ty_ty t1, Ty_ty t2) {

  t1 = sem_sym_ty_actual(t1);
  t2 = sem_sym_ty_actual(t2);

  if (!t1 || !t2)
    return false;

  if (t1 == t2)
    return true;

  return (t1->kind == Ty_nil && t2->kind == Ty_record) ||
         (t1->kind == Ty_record && t2->kind == Ty_nil);
}

// should return true if current scope is using it
// false if its not, even if it was defined in a parent scope
// havent done that yet
bool sem_sym_inuse(S_table symtab, S_symbol sym) { return S_look(symtab, sym) != NULL; }

void sem_sym_var_add(S_table symtab, S_symbol varname, Ty_ty ty) {

  symtab_id_t* id = malloc(sizeof(*id));

  id->kind = ST_VAR;
  id->ty = ty;
  id->params = NULL;

  S_enter(symtab, varname, id);
}

void fun_enter(S_table symtab, S_symbol fname, Ty_ty res, Ty_fieldList params) {

  symtab_id_t* id = malloc(sizeof(*id));

  id->kind = ST_FUN;
  id->ty = res;
  id->params = params;

  S_enter(symtab, fname, id);
}

bool sem_sym_fun_add(int pos, S_table symtab, S_symbol fname, S_symbol res, A_fieldList params) {

  Ty_ty t = Ty_Void();

  if (res)
    t = sem_sym_ty_get(pos, symtab, res);

  fun_enter(symtab, fname, t, handle_ty_record(pos, symtab, params));

  return t != NULL;
}

void sem_sym_fun_scope_begin(S_table symtab, A_fieldList params) {

  S_beginScope(symtab);
  handle_ty_params(symtab, params);
}

void sem_sym_ty_dec(S_table symtab, S_symbol tyname) {

  symtab_id_t* id = malloc(sizeof(*id));

  id->kind = ST_TY;
  id->ty = Ty_Name(tyname, NULL);
  id->params = NULL;

  S_enter(symtab, tyname, id);
}

bool sem_sym_ty_def(S_table symtab, S_symbol tyname, A_ty ty) {

  Ty_ty t = parse_ty(symtab, ty);
  symtab_id_t* id = sem_sym_get(symtab, tyname);

  id->ty->u.name.ty = t;

  return t != NULL;
}

bool sem_sym_ty_cycle_chk(int pos, S_table symtab, S_symbol tyname) {

  Ty_ty start = sem_sym_type_get(symtab, tyname);
  Ty_ty slow = start;
  Ty_ty fast = start;
  bool cyclic = false;

  while (!cyclic && fast && fast->kind == Ty_name &&
         fast->u.name.ty && fast->u.name.ty->kind == Ty_name) {

    slow = slow->u.name.ty;
    fast = fast->u.name.ty->u.name.ty;
    cyclic = slow == fast;
  }

  if (cyclic) {

    EM_semantic_error(pos, "type '%s' is recursive without a record or array", S_name(tyname));
    start->u.name.ty = NULL;
  }

  return cyclic;
}

Ty_fieldList std_param(Ty_ty ty, Ty_fieldList rest) {
  return Ty_FieldList(Ty_Field(S_Symbol("_"), ty), rest);
}

void sem_sym_std_add(S_table symtab) {

  Ty_ty i = Ty_Int();
  Ty_ty s = Ty_String();
  Ty_ty v = Ty_Void();

  fun_enter(symtab, S_Symbol("print"), v, std_param(s, NULL));
  fun_enter(symtab, S_Symbol("flush"), v, NULL);
  fun_enter(symtab, S_Symbol("getchar"), s, NULL);
  fun_enter(symtab, S_Symbol("ord"), i, std_param(s, NULL));
  fun_enter(symtab, S_Symbol("chr"), s, std_param(i, NULL));
  fun_enter(symtab, S_Symbol("size"), i, std_param(s, NULL));
  fun_enter(symtab, S_Symbol("substring"), s, std_param(s, std_param(i, std_param(i, NULL))));
  fun_enter(symtab, S_Symbol("concat"), s, std_param(s, std_param(s, NULL)));
  fun_enter(symtab, S_Symbol("not"), i, std_param(i, NULL));
  fun_enter(symtab, S_Symbol("exit"), v, std_param(i, NULL));
}
