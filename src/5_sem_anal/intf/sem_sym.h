#pragma once

typedef struct {

  enum {

    ST_TY,
    ST_FUN,
    ST_VAR

  } kind;

  union {

    Ty_ty ty;

  } u;

} symtab_id_t;

bool sem_sym_inuse(S_table symtab, S_symbol sym);
bool sem_sym_ty_add(S_table symtab, S_symbol tyname, A_ty ty);
Ty_ty sem_sym_type_get(S_table symtab, S_symbol sym);

