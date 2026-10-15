#pragma once

#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"

extern int WORD_SZ;

typedef struct trans_t trans_t;
typedef struct trans_fun_t trans_fun_t;
typedef struct tfl_t tfl_t;

struct trans_fun_t {
  S_symbol sym;
  Temp_label label;
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

void trans_free(trans_t** t);

void trans_append_all_funs(trans_t* t);
void trans_add_fun(trans_t* parent, trans_t* child, S_symbol name);
Temp_label trans_fun_label(trans_t* t, S_symbol s);
void trans_append_child(trans_t* parent, trans_t* child);
void trans_add_formal(trans_t* t, S_symbol s);
void trans_add_local(trans_t* t, S_symbol s, size_t sz);

void trans_prologue(trans_t* t);
void trans_epilogue(trans_t* t);

void trans_var_store(trans_t* t, S_symbol s);
void trans_funcall_param(trans_t* t, int reg);
void trans_funcall(trans_t* t, S_symbol fun);
void trans_int_exp(trans_t* t, int n);

