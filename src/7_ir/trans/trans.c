#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "trans.h"
#include "frame.h"
#include "temp.h"
#include "tree.h"
#include "printtree.h"

struct trans_fun_t {
  S_symbol sym;
  Temp_label label;
  T_stm ir;
};

struct trans_t {
  frame_t* frame;
  T_stm ir;
  tfl_t* fun_list;
};

tfl_t* all_funs = NULL;

void print_trans(trans_t* t) { printStmList(stdout, T_StmList(t->ir, NULL)); }

void trans_append_all_funs(trans_t* t) {

  tfl_t* cur = all_funs;

  if (!cur)
    return;

  for (trans_fun_t* c = cur->head; cur && c; cur = cur->tail, c = cur->head) {

    t->ir = T_Seq(t->ir, c->ir);

    if (!cur->tail)
      break;
  }
}

tfl_t* tfl(trans_fun_t* head, tfl_t* tail) {

  tfl_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }
  
  f->head = head;
  f->tail = tail;

  return f;
}

trans_fun_t* trans_fun(S_symbol s) {

  trans_fun_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }
  
  f->sym = s;
  f->label = Temp_newlabel();
  f->ir = NULL;

  return f;
}

trans_t* trans_new() {

  trans_t* t = calloc(1, sizeof(trans_t));
  t->frame = frame_empty();
  t->fun_list = tfl(NULL, NULL);
  
  return t;
}

trans_t* trans_new_link(trans_t* parent) {

  trans_t* t = trans_new();
  frame_link(t->frame, parent->frame);

  return t;
}

static void tfl_free(tfl_t* l) {

  tfl_t* next;

  for (; l; l = next) {

    next = l->tail;

    if (l->head) {
      //trans_free(&l->head->ir);
      free(l->head);
    }

    free(l);
  }
}

void trans_free(trans_t** t) {

  trans_t* tr;

  if (!t)
    return;

  tr = *t;

  if (!tr)
    return;

  frame_free(&tr->frame);
  //tfl_free(tr->fun_list);

  free(tr);
  *t = NULL;
}

void trans_add_fun(trans_t* parent, trans_t* child, S_symbol name) {

  trans_fun_t* f = trans_fun(name);
  f->ir = T_Seq(T_Label(f->label), child->ir);

  parent->fun_list = tfl(f, parent->fun_list);
  all_funs = tfl(f, all_funs);

  printf("added fun '%s'\n", S_name(name));
}

void trans_add_formal(trans_t* t, S_symbol s) {

  //printf("formal var '%s'\n", S_name(s));
  frame_add_formal(t->frame, s);
}

void trans_add_local(trans_t* t, S_symbol s, size_t sz) {

  //printf("local var '%s' with size %d\n", S_name(s), sz);
  frame_add_local(t->frame, s, sz);
}

T_stm find_frame(trans_t* t, S_symbol s) {

  frame_access_t* var;

  if (!t->frame) {

    printf("this shouldnt hit\n");
    return NULL;
  }

  var = frame_local_get(t->frame, s);

  if (var)
    return NULL;

  t->frame = t->frame->static_link;

  return T_Seq(T_Move(T_Name(Temp_namedlabel("fp")), T_Mem(T_Name(Temp_namedlabel("fp")))), find_frame(t, s));
}

void trans_var_access(trans_t* t, T_exp dst, S_symbol s) {

  frame_t* frame = t->frame;
  frame_access_t* var = frame_local_get(frame, s);
  T_stm stm = NULL;

  if (!var) {

    stm = T_Seq(T_Move(T_Name(Temp_namedlabel("r5")), 
            T_Name(Temp_namedlabel("fp"))), find_frame(t, s));
  }

  var = frame_local_get(frame, s);
  stm = T_Seq(stm, T_Move(dst ,T_Mem(T_Binop(T_minus, T_Name(Temp_namedlabel("fp")), T_Const(var->stack_off)))));
  stm = T_Seq(stm, T_Move(T_Name(Temp_namedlabel("fp")), T_Name(Temp_namedlabel("r5"))));

  t->frame = frame;
  t->ir = T_Seq(t->ir, stm);
}

T_stm stm_push(T_exp src) {

  return T_Seq(T_Move(T_Name(Temp_namedlabel("sp")), T_Binop(T_minus, T_Name(Temp_namedlabel("sp")), T_Const(WORD_SZ))), 
      T_Move(T_Mem(T_Name(Temp_namedlabel("sp"))), src));
}

T_stm stm_pop(T_exp dst) {

  return T_Seq(T_Move(dst, T_Mem(T_Name(Temp_namedlabel("sp")))),
      T_Move(T_Name(Temp_namedlabel("sp")), T_Binop(T_plus, T_Name(Temp_namedlabel("sp")), T_Const(WORD_SZ))));
}

void trans_prologue(trans_t* t) {

  T_stm stm = stm_push(T_Name(Temp_namedlabel("fp")));

  if (t->frame->stack_size)
    stm = T_Seq(stm, T_Move(T_Name(Temp_namedlabel("sp")), T_Binop(T_minus, T_Name(Temp_namedlabel("sp")), T_Const(t->frame->stack_size))));

  t->ir = T_Seq(stm, t->ir);
}

void trans_epilogue(trans_t* t) {

  T_stm stm = stm_pop(T_Name(Temp_namedlabel("fp")));

  if (t->frame->stack_size)
    stm = T_Seq(T_Move(T_Name(Temp_namedlabel("sp")), T_Name(Temp_namedlabel("fp"))) , stm);

  t->ir = T_Seq(t->ir, stm);
}

