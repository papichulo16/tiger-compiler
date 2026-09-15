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
Ty_ty sem_sym_type_get(S_table symtab, S_symbol sym);

bool sem_sym_ty_add(S_table symtab, S_symbol tyname, A_ty ty);
bool sem_sym_var_add(int pos, S_table symtab, S_symbol varname, S_symbol var_ty);

// starts a scope but doesnt end it lol, as long as it works
bool sem_sym_fun_add(int pos, S_table symtab, S_symbol fname, S_symbol res, A_fieldList params);

