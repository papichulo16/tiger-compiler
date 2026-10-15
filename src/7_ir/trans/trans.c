#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "frame.h"
#include "trans.h"
#include "temp.h"
#include "tree.h"
#include "printtree.h"
#include "stm.h"

tfl_t* all_funs = NULL;

char* regs[] = { "r0", "r1", "r2", "r3", "r4", "r5" };

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
  t->parent = parent;
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

  //printf("added fun '%s'\n", S_name(name));
}

Temp_label trans_fun_label(trans_t* t, S_symbol s) {

  Temp_label label = NULL;

  for (; t && !label; t = t->parent) {

    for (tfl_t* l = t->fun_list; l; l = l->tail) {

      if (l->head && l->head->sym == s) {

        label = l->head->label;
        break;
      }
    }
  }

  return label;
}

void trans_append_child(trans_t* parent, trans_t* child) {

  parent->ir = T_Seq(parent->ir, child->ir);
}

void trans_add_formal(trans_t* t, S_symbol s) {

  //printf("formal var '%s'\n", S_name(s));
  frame_add_formal(t->frame, s);
}

void trans_add_local(trans_t* t, S_symbol s, size_t sz) {

  //printf("local var '%s' with size %d\n", S_name(s), sz);
  frame_add_local(t->frame, s, sz);
}

void trans_prologue(trans_t* t) {

  t->ir = T_Seq(stm_prologue(t), t->ir);
}

void trans_epilogue(trans_t* t) {

  t->ir = T_Seq(t->ir, stm_epilogue(t));
}

bool verify_right_exp(trans_t* t) {

  T_stm init;

  if (!t->ir) {

    printf("no init exp, also no ir cuz of it. should be temporary right??\n");
    return false;
  }

  if (t->ir->kind != T_SEQ) {

    printf("HUHUHUHUHUHUH BIG IR WHOOPS\n");
    exit(-1);
  }

  init = t->ir->u.SEQ.right;

  if (!init) {

    printf("no init exp, should be temporary right??\n");
    return false;
  }

  if (init->kind != T_EXP) {

    // i am not sure if this will happen, but i bet if it does then its a 
    // semantic error anyways
    printf("was there a semantic error?\n");
    return false;
  }

  return true;
}

void trans_var_store(trans_t* t, S_symbol s) {

  if (!verify_right_exp(t))
    return;

  t->ir->u.SEQ.right = stm_var_write(t, t->ir->u.SEQ.right->u.EXP, s);
}

void trans_funcall_param(trans_t* t, int reg) {

  if (!verify_right_exp(t))
    return;

  if (reg > MAX_PARAMS) {

    printf("max params in funcall, i should add the ability to do more soon\n");
    return;
  }

  t->ir->u.SEQ.right = T_Move(T_Name(Temp_namedlabel(regs[reg])), t->ir->u.SEQ.right->u.EXP);
}

void trans_funcall(trans_t* t, S_symbol fun) {

  Temp_label label = trans_fun_label(t, fun);

  if (!label)
    return;

  t->ir = T_Seq(t->ir, T_Exp(T_Call(T_Name(label))));
}

void trans_int_exp(trans_t* t, int n) {

  t->ir = T_Seq(t->ir, T_Exp(T_Const(n)));
}
