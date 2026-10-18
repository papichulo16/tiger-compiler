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

static int g_stdlib_param_counts[] = {
  1, 0, 0, 1, 1,
  1, 3, 2, 1, 1, 1
};

#define STDLIB_FUN_COUNT (sizeof(g_stdlib_names) / sizeof(*g_stdlib_names))

static S_symbol g_stdlib_funs[STDLIB_FUN_COUNT];
static int g_stdlib_init = 0;

static void trans_stdlib_init(void) {

  for (size_t i = 0; i < STDLIB_FUN_COUNT; i++)
    g_stdlib_funs[i] = S_Symbol(g_stdlib_names[i]);

  g_stdlib_init = 1;
}

static int trans_stdlib_idx(S_symbol s) {

  int idx = -1;

  if (!g_stdlib_init)
    trans_stdlib_init();

  for (size_t i = 0; i < STDLIB_FUN_COUNT && idx < 0; i++)
    if (g_stdlib_funs[i] == s)
      idx = i;

  return idx;
}

int trans_is_stdlib_fun(S_symbol s) { return trans_stdlib_idx(s) >= 0; }

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
  f->param_count = 0;
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

trans_t* trans_new_link_shared_frame(trans_t* parent) {

  trans_t* t = trans_new_link(parent);
  t->frame->stack_size = parent->frame->stack_size;
  t->frame->shared = true;

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
  f->param_count = child->frame->param_count;
  f->ir = T_Seq(T_Label(f->label), child->ir);

  parent->fun_list = tfl(f, parent->fun_list);
  all_funs = tfl(f, all_funs);

  //printf("added fun '%s'\n", S_name(name));
}

static trans_fun_t* trans_fun_get(trans_t* t, S_symbol s) {

  trans_fun_t* f = NULL;

  for (; t && !f; t = t->parent) {

    for (tfl_t* l = t->fun_list; l; l = l->tail) {

      if (l->head && l->head->sym == s) {

        f = l->head;
        break;
      }
    }
  }

  return f;
}

Temp_label trans_fun_label(trans_t* t, S_symbol s) {

  trans_fun_t* f;

  TRANS_SEM_ERR_RET(NULL);

  f = trans_fun_get(t, s);

  return f ? f->label : NULL;
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

bool verify_right_stm(trans_t* t) {

  if (!t->ir) {

    printf("no init exp, also no ir cuz of it. should be temporary right??\n");
    return false;
  }

  if (t->ir->kind != T_SEQ) {

    printf("HUHUHUHUHUHUH BIG IR WHOOPS\n");
    exit(-1);
  }

  if (!t->ir->u.SEQ.right) {

    printf("no init exp, should be temporary right??\n");
    return false;
  }

  return true;
}

bool verify_right_exp(trans_t* t) {

  T_stm init;

  if (!verify_right_stm(t))
    return false;

  init = t->ir->u.SEQ.right;

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

T_op trans_oper_ir(A_oper ast_oper) {

  T_op op = T_plus;

  switch (ast_oper) {

    case A_plusOp:
      op = T_plus;
      break;
    case A_minusOp:
      op = T_minus;
      break;
    case A_timesOp:
      op = T_mul;
      break;
    case A_divideOp:
      op = T_div;
      break;
    case A_andOp:
      op = T_and;
      break;
    case A_orOp:
      op = T_or;
      break;
    case A_eqOp:
      op = T_eq;
      break;
    case A_neqOp:
      op = T_ne;
      break;
    case A_ltOp:
      op = T_lt;
      break;
    case A_leOp:
      op = T_le;
      break;
    case A_gtOp:
      op = T_gt;
      break;
    case A_geOp:
      op = T_ge;
      break;
  }

  return op;
}

void trans_oper_exp(trans_t* t, A_oper ast_oper, T_exp left, T_exp right) {

  TRANS_SEM_ERR_RET();

  if (!left || !right)
    return;

  t->ir = T_Seq(t->ir, T_Exp(T_Op(trans_oper_ir(ast_oper), left, right)));
}

void trans_arr_idx(trans_t* t) {

  T_exp subs;

  TRANS_SEM_ERR_RET();

  subs = trans_pop_exp(t);
  t->ir = T_Seq(t->ir, T_Exp(T_Op(T_mul, subs, T_Const(WORD_SZ))));
}

void trans_off_access(trans_t* t, T_exp base, T_exp off) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(T_Mem(T_Op(T_plus, base, off))));
}

// the main var needs to have been accessed last
void trans_field_access(trans_t* t, size_t off) {

  TRANS_SEM_ERR_RET();

  if (!verify_right_exp(t))
    return;

  t->ir->u.SEQ.right = T_Exp(T_Mem(T_Op(T_plus, t->ir->u.SEQ.right->u.EXP, T_Const(off))));
}

void trans_var_store_exp(trans_t* t, S_symbol s, T_exp e) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, stm_var_write(t, e, s));
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

  Temp_label label = NULL;
  trans_fun_t* f;
  T_stmList params;
  int param_count = 0;
  int stdlib_idx;

  TRANS_SEM_ERR_RET();

  stdlib_idx = trans_stdlib_idx(fun);

  if (stdlib_idx >= 0) {

    label = Temp_namedlabel(S_name(fun));
    param_count = g_stdlib_param_counts[stdlib_idx];

  } else if ((f = trans_fun_get(t, fun))) {

    label = f->label;
    param_count = f->param_count;
  }

  if (!label)
    return;

  params = trans_pop_funcall_params(t, param_count);
  t->ir = T_Seq(t->ir, T_Exp(T_Call(T_Name(label), params)));
}

void trans_int_exp(trans_t* t, int n) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(T_Const(n)));
}

T_stm trans_pop_stm(trans_t* t) {

  T_stm top;
  T_stm s = NULL;

  TRANS_SEM_ERR_RET(NULL);

  if (!verify_right_stm(t))
    goto out;

  top = t->ir;
  s = top->u.SEQ.right;

  t->ir = top->u.SEQ.left;
  free(top);

out:
  return s;
}

T_exp trans_pop_exp(trans_t* t) {

  T_stm s;
  T_exp e = NULL;

  TRANS_SEM_ERR_RET(NULL);

  if (!verify_right_exp(t))
    goto out;

  s = trans_pop_stm(t);
  e = s->u.EXP;

  free(s);

out:
  return e;
}

T_stmList trans_pop_funcall_params(trans_t* t, int count) {

  T_stmList params = NULL;
  T_stm s;

  TRANS_SEM_ERR_RET(NULL);

  for (int i = 0; i < count; i++) {

    s = trans_pop_stm(t);

    if (!s)
      break;

    params = T_StmList(s, params);
  }

  return params;
}

void trans_assign_exp(trans_t* t, T_exp dst, T_exp src) {

  TRANS_SEM_ERR_RET();

  if (!dst || !src)
    return;

  t->ir = T_Seq(t->ir, T_Move(dst, src));
}

void trans_alloc_exp(trans_t* t, T_exp e) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Exp(e));
  trans_funcall_param(t, 0);
  trans_funcall(t, S_Symbol("__ALLOC"));
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

void trans_alloc(trans_t* t, size_t size) { trans_alloc_exp(t, T_Const(size)); }
Temp_temp trans_rec_alloc(trans_t* t, size_t size) { trans_rec_alloc_exp(t, T_Const(size)); }

void trans_rec_field_init(trans_t* t, Temp_temp rec, size_t off, T_exp src) {

  TRANS_SEM_ERR_RET();

  if (!rec)
    return;

  trans_assign_exp(t, T_Mem(T_Op(T_plus, T_Temp(rec), T_Const(off))), src);
}

void trans_temp_exp(trans_t* t, Temp_temp tmp) {

  TRANS_SEM_ERR_RET();

  if (!tmp)
    return;

  t->ir = T_Seq(t->ir, T_Exp(T_Temp(tmp)));
}

void trans_start_loop(trans_t* t, T_exp test, Temp_label start, Temp_label end) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Seq(T_Label(start), T_Cjump(test, NULL, end)));
}

void trans_end_loop(trans_t* t, Temp_label start, Temp_label end) {

  TRANS_SEM_ERR_RET();

  t->ir = T_Seq(t->ir, T_Seq(T_Jump(T_Name(start), Temp_LabelList(start, NULL)), T_Label(end)));
}

void trans_for_loop(trans_t* t, S_symbol var, T_exp lo, T_exp hi) {

  T_stm body;
  T_exp idx;
  Temp_label start;
  Temp_label end;

  TRANS_SEM_ERR_RET();

  body = t->ir;
  t->ir = NULL;

  start = Temp_newlabel();
  end = Temp_newlabel();

  // init
  trans_var_store_exp(t, var, lo);

  // loop check, I had to create the access exp and then pop  
  trans_var_access(t, var);
  idx = trans_pop_exp(t);
  trans_start_loop(t, T_Op(T_ne, idx, hi), start, end);

  t->ir = T_Seq(t->ir, body);

  // post increment
  trans_var_access(t, var);
  idx = trans_pop_exp(t);
  trans_var_store_exp(t, var, T_Op(T_plus, idx, T_Const(1)));

  trans_end_loop(t, start, end);
}

