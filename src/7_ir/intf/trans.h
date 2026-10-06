#pragma once

extern int WORD_SZ;

typedef struct trans_t trans_t;
typedef struct trans_fun_t trans_fun_t;
typedef struct tfl_t tfl_t;

struct tfl_t { trans_fun_t* head; tfl_t* tail; };

void print_trans(trans_t* t);
trans_t* trans_new();
trans_t* trans_new_link(trans_t* parent);

void trans_free(trans_t** t);

void trans_append_all_funs(trans_t* t);
void trans_add_fun(trans_t* parent, trans_t* child, S_symbol name);
void trans_add_formal(trans_t* t, S_symbol s);
void trans_add_local(trans_t* t, S_symbol s, size_t sz);

void trans_prologue(trans_t* t);
void trans_epilogue(trans_t* t);
