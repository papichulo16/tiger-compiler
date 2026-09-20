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

Ty_ty handle_ty_sym(S_table symtab, S_symbol ty) {

  char* s = S_name(ty);

  if ((!strcmp(s, "int") || !strcmp(s, "INT")) && strlen(s) == 3)
    return Ty_Int();

  if ((!strcmp(s, "string") || !strcmp(s, "STRING")) && strlen(s) == 6)
    return Ty_String();

  return sem_sym_type_get(symtab, ty);
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

void handle_ty_params(int pos, S_table symtab, A_fieldList fl) {

  Ty_ty ty;

  if (!fl)
    return;

  sem_sym_var_add(pos, symtab, fl->head->name, fl->head->typ);

  handle_ty_params(pos, symtab, fl->tail);   
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

  if (!rec || rec->kind != Ty_record)
    return NULL;

  return f = fl_field_get(rec->u.record, sub), 
         f ? f->ty : NULL;
}

Ty_ty parse_ty(S_table symtab, A_ty ty) {

  Ty_ty t;
  Ty_fieldList fl;

  if (!ty)
    return NULL;

  switch (ty->kind) {

    case A_nameTy:
      return handle_ty_sym(symtab, ty->u.array);

    case A_recordTy:

      fl = handle_ty_record(ty->pos, symtab, ty->u.record);

      if (!fl)
        return NULL;

      return Ty_Record(fl);

    case A_arrayTy:

      t = handle_ty_sym(symtab, ty->u.array);

      if (!t)
        return NULL;

      return Ty_Array(t);
  }
}

bool sem_sym_ty_eq(Ty_ty t1, Ty_ty t2) {

  if (!t1 || !t2)
    return NULL;

  printf("ty eq: ");
  Ty_print(t1);
  printf(" -- ");
  Ty_print(t2);
  putchar('\n');

  return t1 == t2;
}

// should return true if current scope is using it 
// false if its not, even if it was defined in a parent scope
// havent done that yet
bool sem_sym_inuse(S_table symtab, S_symbol sym) { return S_look(symtab, sym) != NULL; }

Ty_ty sem_sym_var_add(int pos, S_table symtab, S_symbol varname, S_symbol var_ty) {

  Ty_ty t = Ty_Void();
  symtab_id_t* id;

  if (sem_sym_inuse(symtab, varname)) {

    EM_semantic_error(pos, "variable '%s' is already defined", S_name(varname));
    return NULL;
  }

  if (!var_ty)
    goto dec;

  if (t = handle_ty_sym(symtab, var_ty), !t) {

    EM_semantic_error(pos, "type '%s' does not exist", S_name(var_ty));
    return NULL;
  }

dec:
  id = malloc(sizeof(*id));

  id->kind = ST_VAR;
  id->ty = t;
  id->params = NULL;

  S_enter(symtab, varname, id);

  return t;
}

bool sem_sym_fun_add(int pos, S_table symtab, S_symbol fname, S_symbol res, A_fieldList params) {

  Ty_ty t = Ty_Void();
  symtab_id_t* id;

  if (sem_sym_inuse(symtab, fname)) {

    EM_semantic_error(pos, "function '%s' is already defined", S_name(fname));
    return false;
  }

  if (!res)
    goto dec;

  if (t = handle_ty_sym(symtab, res), !t) {
    EM_semantic_error(pos, "type '%s' does not exist", S_name(res));
    return false;
  }

dec:
  id = malloc(sizeof(*id));

  id->kind = ST_FUN;
  id->ty = t;
  id->params = handle_ty_record(pos, symtab, params);

  S_enter(symtab, fname, id);

  S_beginScope(symtab);
  handle_ty_params(pos, symtab, params);

  return true;
}

bool sem_sym_ty_add (S_table symtab, S_symbol tyname, A_ty ty) {

  Ty_ty t;
  symtab_id_t* id;

  if (t = parse_ty(symtab, ty), 
      !t || sem_sym_inuse(symtab, tyname))
    return false;

  id = malloc(sizeof(*id));

  id->kind = ST_TY;
  id->ty = t;
  id->params = NULL;

  S_enter(symtab, tyname, id);

  return true;
}

