#pragma once

typedef struct {

  enum {

    ST_TY,
    ST_FUN,
    ST_VAR

  } kind;

  Ty_ty ty;
  Ty_fieldList params;

} symtab_id_t;

bool sem_sym_inuse(S_table symtab, S_symbol sym);
bool sem_sym_ty_eq(Ty_ty t1, Ty_ty t2);
bool sem_sym_is_fun(S_table symtab, S_symbol name);

Ty_ty sem_sym_type_get(S_table symtab, S_symbol sym);
Ty_fieldList sem_sym_params_get(S_table symtab, S_symbol sym);
Ty_ty sem_sym_record_ty_get(Ty_ty rec, S_symbol sub);

bool sem_sym_ty_add(S_table symtab, S_symbol tyname, A_ty ty);
Ty_ty sem_sym_var_add(int pos, S_table symtab, S_symbol varname, S_symbol var_ty);

// starts a scope but doesnt end it lol, as long as it works
bool sem_sym_fun_add(int pos, S_table symtab, S_symbol fname, S_symbol res, A_fieldList params);

