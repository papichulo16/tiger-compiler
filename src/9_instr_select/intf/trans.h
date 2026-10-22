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
  frame_t* frame;
};

struct trans_t {
  trans_t* parent;
  frame_t* frame;
  T_stm ir;
  tfl_t* fun_list;
};

struct tfl_t { trans_fun_t* head; tfl_t* tail; };

typedef struct trans_str_t trans_str_t;

struct trans_str_t {
  Temp_label label;
  string str;
  trans_str_t* next;
};

typedef struct trans_frag_t trans_frag_t;
typedef struct trans_frag_list_t trans_frag_list_t;

struct trans_frag_t {

  enum { TRANS_PROC_FRAG, TRANS_STR_FRAG } kind;

  union {

    struct { Temp_label label; T_stm body; frame_t* frame; } proc;
    struct { Temp_label label; string str; } str;

  } u;
};

struct trans_frag_list_t { trans_frag_t* head; trans_frag_list_t* tail; };

extern char* regs[];

void print_trans(trans_t* t);
trans_t* trans_new();
trans_t* trans_new_link(trans_t* parent);
trans_t* trans_new_link_shared_frame(trans_t* parent);

void trans_free(trans_t** t);

void trans_append_all_funs(trans_t* t);
Temp_label trans_str_label(string s);
void trans_strs_free(void);
trans_str_t* trans_strs_take(void);
tfl_t* trans_funs_take(void);

trans_frag_list_t* trans_frags_take(trans_t* t);
void trans_frags_free(trans_frag_list_t** l);
trans_fun_t* trans_declare_fun(trans_t* parent, S_symbol name, int param_count);
void trans_define_fun(trans_fun_t* f, trans_t* child);
trans_fun_t* trans_fun_get(trans_t* t, S_symbol s);
Temp_label trans_fun_label(trans_t* t, S_symbol s);
void trans_append_child(trans_t* parent, trans_t* child);
void trans_add_formal(trans_t* t, S_symbol s);
void trans_add_local(trans_t* t, S_symbol s, size_t sz);
void trans_add_ty(trans_t* t, S_symbol s, frame_field_t* fields);
frame_field_t* trans_ty_fields_get(trans_t* t, S_symbol s);

T_stm trans_pop_stm_to(trans_t* t, T_stm to);
T_stm trans_pop_stm(trans_t* t);
T_exp trans_pop_exp(trans_t* t);
T_stmList trans_pop_funcall_params(trans_t* t, int count);

void trans_start_loop(trans_t* t, T_exp test, Temp_label start, Temp_label end);
void trans_end_loop(trans_t* t, Temp_label start, Temp_label end);
void trans_for_loop(trans_t* t, S_symbol var, T_exp lo, T_exp hi, Temp_label end);
void trans_if(trans_t* t, T_exp iff, T_stm then, T_stm elsee);
void trans_break(trans_t* t);

void trans_assign_exp(trans_t* t, T_exp dst, T_exp src);
void trans_oper_exp(trans_t* t, A_oper ast_oper, T_exp left, T_exp right);

void trans_prologue(trans_t* t, bool is_fun);
void trans_epilogue(trans_t* t, bool is_fun);

void trans_var_access(trans_t* t, S_symbol s);
void trans_field_access(trans_t* t, size_t off);
void trans_off_access(trans_t* t, T_exp base, T_exp off);
void trans_arr_idx(trans_t* t);
void trans_var_store(trans_t* t, S_symbol s);
void trans_var_store_exp(trans_t* t, S_symbol s, T_exp e);

int trans_find_callee_static_link(trans_t* t, S_symbol call); // returns 1 if not stdlib
void trans_funcall_param(trans_t* t, int reg, T_stm s);
void trans_funcall(trans_t* t, S_symbol fun);
int trans_is_stdlib_fun(S_symbol s);
void trans_int_exp(trans_t* t, int n);
void trans_string_exp(trans_t* t, string s);

void trans_alloc(trans_t* t, size_t size);
void trans_alloc_exp(trans_t* t, T_exp e);
Temp_temp trans_rec_alloc(trans_t* t, size_t size);
Temp_temp trans_rec_alloc_exp(trans_t* t, T_exp e);
Temp_temp trans_arr_alloc(trans_t* t, T_exp sz, T_exp init);
void trans_rec_field_init(trans_t* t, Temp_temp rec, size_t off, T_exp src);
void trans_temp_exp(trans_t* t, Temp_temp tmp);

void trans_tracker_push(Temp_label v);
Temp_label trans_tracker_pop();

