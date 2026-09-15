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

Ty_ty sem_sym_type_get(S_table symtab, S_symbol name) {

  symtab_id_t* id = sem_sym_get(symtab, name);

  if (!id)
    return NULL;

  return id->u.ty;
}

Ty_ty handle_ty_sym(S_table symtab, S_symbol ty) {

  char* s = S_name(ty);

  if (!strcmp(s, "int") || !strcmp(s, "INT"))
    return Ty_Int();

  if (!strcmp(s, "string") || !strcmp(s, "STRING"))
    return Ty_String();

  return sem_sym_type_get(symtab, ty);
}

Ty_fieldList handle_ty_record(S_table symtab, A_fieldList fl) {

  Ty_ty ty;
  Ty_field f;

  if (!fl)
    return NULL;

  ty = handle_ty_sym(symtab, fl->head->typ);

  if (!ty)
    printf("undefined type '%s'\n", S_name(fl->head->typ));

  f = Ty_Field(fl->head->name, ty);

  return Ty_FieldList(f, handle_ty_record(symtab, fl->tail));   
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

      fl = handle_ty_record(symtab, ty->u.record);

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

// should return true if current scope is using it 
// false if its not, even if it was defined in a parent scope
// havent done that yet
bool sem_sym_inuse(S_table symtab, S_symbol sym) { return S_look(symtab, sym) != NULL; }

bool sem_sym_ty_add (S_table symtab, S_symbol tyname, A_ty ty) {

  Ty_ty t;
  symtab_id_t* id;

  if (t = parse_ty(symtab, ty), 
      !t || sem_sym_inuse(symtab, tyname))
    return false;

  id = malloc(sizeof(*id));

  id->kind = ST_TY;
  id->u.ty = t;

  S_enter(symtab, tyname, id);

  return true;
}

