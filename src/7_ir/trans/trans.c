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

char* regs[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9" };

static char* g_stdlib_names[] = {
  "print", "flush", "getchar", "ord", "chr",
  "size", "substring", "concat", "not", "exit", "__ALLOC"
};

#define STDLIB_FUN_COUNT (sizeof(g_stdlib_names) / sizeof(*g_stdlib_names))

static S_symbol g_stdlib_funs[STDLIB_FUN_COUNT];
static int g_stdlib_init = 0;

static void trans_stdlib_init(void) {

  for (size_t i = 0; i < STDLIB_FUN_COUNT; i++)
    g_stdlib_funs[i] = S_Symbol(g_stdlib_names[i]);

  g_stdlib_init = 1;
}

int trans_is_stdlib_fun(S_symbol s) {

  int found = 0;

  if (!g_stdlib_init)
    trans_stdlib_init();

  for (size_t i = 0; i < STDLIB_FUN_COUNT && !found; i++)
    found = (g_stdlib_funs[i] == s);

  return found;
}

void print_trans(trans_t* t) {

  TRANS_SEM_ERR_RET();
  printStmList(stdout, T_StmList(t->ir, NULL));
}

void trans_append_all_funs(trans_t* t) {

  tfl_t* cur = all_funs;

  TRANS_SEM_ERR_RET();

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

  trans_fun_t* f;

  TRANS_SEM_ERR_RET();

  f = trans_fun(name);
  f->ir = T_Seq(T_Label(f->label), child->ir);

  parent->fun_list = tfl(f, parent->fun_list);
  all_funs = tfl(f, all_funs);

  //printf("added fun '%s'\n", S_name(name));
}

Temp_label trans_fun_label(trans_t* t, S_symbol s) {

  Temp_label label = NULL;

  TRANS_SEM_ERR_RET(NULL);

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

  TRANS_SEM_ERR_RET();

  parent->ir = T_Seq(parent->ir, child->ir);
}

void trans_add_formal(trans_t* t, S_symbol s) {

  TRANS_SEM_ERR_RET();

  //printf("formal var '%s'\n", S_name(s));
  frame_add_formal(t->frame, s);
}

void trans_add_local(trans_t* t, S_symbol s, size_t sz) {

  TRANS_SEM_ERR_RET();

  //printf("local var '%s' with size %d\n", S_name(s), sz);
  frame_add_local(t->frame, s, sz);
}

void trans_add_ty(trans_t* t, S_symbol s, frame_field_t* fields) {

  frame_add_ty(t->frame, s, fields);
}

frame_field_t* trans_ty_fields_get(trans_t* t, S_symbol s) {

  TRANS_SEM_ERR_RET(NULL);

  return frame_ty_fields_get(t->frame, s);
}

void trans_prologue(trans_t* t) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(stm_prologue(t), t->ir);
}

void trans_epilogue(trans_t* t) {

  TRANS_SEM_ERR_RET();

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
    printf("was there a semantic error? kind %d\n", init->kind);
    return false;
  }

  return true;
}

void trans_var_access(trans_t* t, S_symbol s) {

  T_stm stm;

  TRANS_SEM_ERR_RET();

  stm = stm_var_access(t, s);

  // local vars have no frame walk on the left and dont leave an empty seq behind
  // also who would have thought AVL trees would come in handy
  // just rotating so the exp stays on the right, should be harmless
  if (stm->u.SEQ.left)
    t->ir = T_Seq(T_Seq(t->ir, stm->u.SEQ.left), stm->u.SEQ.right);
  else
    t->ir = T_Seq(t->ir, stm->u.SEQ.right);
  free(stm);
}

void trans_arr_idx(trans_t* t) {

  T_exp subs;

  TRANS_SEM_ERR_RET();

  subs = trans_pop_exp(t);
  t->ir = T_Seq(t->ir, T_Exp(T_Binop(T_mul, subs, T_Const(WORD_SZ))));
}

void trans_off_access(trans_t* t, T_exp base, T_exp off) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(T_Mem(T_Binop(T_plus, base, off))));
}

// the main var needs to have been accessed last
void trans_field_access(trans_t* t, size_t off) {

  TRANS_SEM_ERR_RET();

  if (!verify_right_exp(t))
    return;

  t->ir->u.SEQ.right = T_Exp(T_Mem(T_Binop(T_plus, t->ir->u.SEQ.right->u.EXP, T_Const(off))));
}

void trans_var_store(trans_t* t, S_symbol s) {

  TRANS_SEM_ERR_RET();

  if (!verify_right_exp(t))
    return;

  t->ir->u.SEQ.right = stm_var_write(t, t->ir->u.SEQ.right->u.EXP, s);
}

void trans_funcall_param(trans_t* t, int reg) {

  TRANS_SEM_ERR_RET();

  if (!verify_right_exp(t))
    return;

  if (reg > MAX_PARAMS) {

    printf("max params in funcall, i should add the ability to do more soon\n");
    return;
  }

  t->ir->u.SEQ.right = T_Move(T_Name(Temp_namedlabel(regs[reg])), t->ir->u.SEQ.right->u.EXP);
}

void trans_funcall(trans_t* t, S_symbol fun) {

  Temp_label label;

  TRANS_SEM_ERR_RET();

  if (trans_is_stdlib_fun(fun))
    label = Temp_namedlabel(S_name(fun));
  else
    label = trans_fun_label(t, fun);

  if (!label)
    return;

  t->ir = T_Seq(t->ir, T_Exp(T_Call(T_Name(label))));
}

void trans_int_exp(trans_t* t, int n) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(T_Const(n)));
}

// it doesnt do a pop expression, just takes out the actual last expression
T_exp trans_pop_exp(trans_t* t) {

  T_stm top;
  T_exp e = NULL;

  TRANS_SEM_ERR_RET(NULL);

  if (!verify_right_exp(t))
    goto out;

  top = t->ir;
  e = top->u.SEQ.right->u.EXP;

  t->ir = top->u.SEQ.left;
  free(top->u.SEQ.right);
  free(top);

out:
  return e;
}

void trans_alloc_exp(trans_t* t, T_exp e) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(T_Binop(T_mul, e, T_Const(WORD_SZ))));
  trans_funcall_param(t, 0);
  trans_funcall(t, S_Symbol("__ALLOC"));
}

void trans_alloc(trans_t* t, size_t size) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(T_Const(size)));
  trans_funcall_param(t, 0);
  trans_funcall(t, S_Symbol("__ALLOC"));
}

void trans_assign_exp(trans_t* t, T_exp dst, T_exp src) {

  TRANS_SEM_ERR_RET();

  if (!dst || !src)
    return;

  t->ir = T_Seq(t->ir, T_Move(dst, src));
}

// the record ptr is in a tmp while i populate the rec
Temp_temp trans_rec_alloc_exp(trans_t* t, T_exp e) {

  Temp_temp rec = NULL;
  T_exp call;

  TRANS_SEM_ERR_RET(NULL);

  trans_alloc_exp(t, e);
  call = trans_pop_exp(t);

  if (!call)
    goto out;

  rec = Temp_newtemp();
  trans_assign_exp(t, T_Temp(rec), call);

out:
  return rec;
}
// the record ptr is in a tmp while i populate the rec
Temp_temp trans_rec_alloc(trans_t* t, size_t size) {

  Temp_temp rec = NULL;
  T_exp call;

  TRANS_SEM_ERR_RET(NULL);

  trans_alloc(t, size);
  call = trans_pop_exp(t);

  if (!call)
    goto out;

  rec = Temp_newtemp();
  trans_assign_exp(t, T_Temp(rec), call);

out:
  return rec;
}

void trans_rec_field_init(trans_t* t, Temp_temp rec, size_t off, T_exp src) {

  TRANS_SEM_ERR_RET();

  if (!rec)
    return;

  trans_assign_exp(t, T_Mem(T_Binop(T_plus, T_Temp(rec), T_Const(off))), src);
}

void trans_temp_exp(trans_t* t, Temp_temp tmp) {

  TRANS_SEM_ERR_RET();

  if (!tmp)
    return;

  t->ir = T_Seq(t->ir, T_Exp(T_Temp(tmp)));
}

