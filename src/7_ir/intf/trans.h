#pragma once

#include "symbol.h"
#include "absyn.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "errormsg.h"

extern int WORD_SZ;

#define TRANS_SEM_ERR_RET(...) \
  do { if (EM_err_count) return __VA_ARGS__; } while (0)

typedef struct trans_t trans_t;
typedef struct trans_fun_t trans_fun_t;
typedef struct tfl_t tfl_t;

struct trans_fun_t {
  S_symbol sym;
  Temp_label label;
  int param_count;
  T_stm ir;
};

struct trans_t {
  trans_t* parent;
  frame_t* frame;
  T_stm ir;
  tfl_t* fun_list;
};

struct tfl_t { trans_fun_t* head; tfl_t* tail; };

extern char* regs[];

void print_trans(trans_t* t);
trans_t* trans_new();
trans_t* trans_new_link(trans_t* parent);
trans_t* trans_new_link_shared_frame(trans_t* parent);

void trans_free(trans_t** t);

void trans_append_all_funs(trans_t* t);
void trans_add_fun(trans_t* parent, trans_t* child, S_symbol name);
trans_fun_t* trans_fun_get(trans_t* t, S_symbol s);
Temp_label trans_fun_label(trans_t* t, S_symbol s);
void trans_append_child(trans_t* parent, trans_t* child);
void trans_add_formal(trans_t* t, S_symbol s);
void trans_add_local(trans_t* t, S_symbol s, size_t sz);
void trans_add_ty(trans_t* t, S_symbol s, frame_field_t* fields);
frame_field_t* trans_ty_fields_get(trans_t* t, S_symbol s);

T_stm trans_pop_stm(trans_t* t);
T_exp trans_pop_exp(trans_t* t);
T_stmList trans_pop_funcall_params(trans_t* t, int count);

void trans_start_loop(trans_t* t, T_exp test, Temp_label start, Temp_label end);
void trans_end_loop(trans_t* t, Temp_label start, Temp_label end);
void trans_for_loop(trans_t* t, S_symbol var, T_exp lo, T_exp hi);

void trans_assign_exp(trans_t* t, T_exp dst, T_exp src);
void trans_oper_exp(trans_t* t, A_oper ast_oper, T_exp left, T_exp right);

void trans_prologue(trans_t* t);
void trans_epilogue(trans_t* t);

void trans_var_access(trans_t* t, S_symbol s);
void trans_field_access(trans_t* t, size_t off);
void trans_off_access(trans_t* t, T_exp base, T_exp off);
void trans_arr_idx(trans_t* t);
void trans_var_store(trans_t* t, S_symbol s);
void trans_var_store_exp(trans_t* t, S_symbol s, T_exp e);

void trans_funcall_param(trans_t* t, int reg);
void trans_funcall(trans_t* t, S_symbol fun);
int trans_is_stdlib_fun(S_symbol s);
void trans_int_exp(trans_t* t, int n);

void trans_alloc(trans_t* t, size_t size);
void trans_alloc_exp(trans_t* t, T_exp e);
Temp_temp trans_rec_alloc(trans_t* t, size_t size);
Temp_temp trans_rec_alloc_exp(trans_t* t, T_exp e);
Temp_temp trans_arr_alloc(trans_t* t, T_exp sz, T_exp init);
void trans_rec_field_init(trans_t* t, Temp_temp rec, size_t off, T_exp src);
void trans_temp_exp(trans_t* t, Temp_temp tmp);

