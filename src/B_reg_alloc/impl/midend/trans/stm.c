#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "trans.h"
#include "frame.h"
#include "temp.h"
#include "tree.h"
#include "stm.h"

struct tracker {
  Temp_label l;
  void* n;
};

struct tracker* track_head = NULL;

void loop_tracker_push(Temp_label v) {

  struct tracker* t = malloc(sizeof(struct tracker));

  t->l = v;
  t->n = track_head;

  track_head = t;
}

Temp_label loop_tracker_pop() {

  struct tracker* p = track_head;
  Temp_label l = NULL;

  if (p) {

    l = p->l;
    track_head = (struct tracker *) p->n;

    free(p);
  }

  return l;
}

static Temp_label loop_tracker_peek() {

  if (track_head)
    return track_head->l;

  return NULL;
}

T_stm stm_push(T_exp src) {

  return T_Seq(T_Move(T_Name(Temp_namedlabel("sp")), T_Op(T_minus, T_Name(Temp_namedlabel("sp")), T_Const(WORD_SZ))), 
      T_Move(T_Mem(T_Name(Temp_namedlabel("sp"))), src));
}

T_stm stm_pop(T_exp dst) {

  return T_Seq(T_Move(dst, T_Mem(T_Name(Temp_namedlabel("sp")))),
      T_Move(T_Name(Temp_namedlabel("sp")), T_Op(T_plus, T_Name(Temp_namedlabel("sp")), T_Const(WORD_SZ))));
}

T_stm stm_resv_frame(int stack_sz) {
  return T_Move(T_Name(Temp_namedlabel("sp")), T_Op(T_minus, T_Name(Temp_namedlabel("sp")), T_Const(stack_sz)));
}


T_stm stm_prologue(trans_t* t, bool is_fun) {

  T_stm stm = stm_push(T_Name(Temp_namedlabel("fp")));

  if (is_fun)
    stm = T_Seq(stm, stm_push(T_Name(Temp_namedlabel(regs[0]))));

  stm = T_Seq(stm, T_Move(T_Name(Temp_namedlabel("fp")), T_Name(Temp_namedlabel("sp"))));

  if (t->frame->stack_size) {

    stm = T_Seq(stm, stm_resv_frame(t->frame->stack_size));
    stm = T_Seq(stm, stm_store_formals(t));
  }

  return stm;
}

T_stm stm_epilogue(trans_t* t, bool is_fun) {

  T_stm stm = stm_pop(T_Name(Temp_namedlabel("fp")));

  if (is_fun)
    stm = T_Seq(stm, stm_pop(T_Name(Temp_namedlabel("fp"))));

  if (t->frame->stack_size)
    stm = T_Seq(T_Move(T_Name(Temp_namedlabel("sp")), T_Name(Temp_namedlabel("fp"))), stm);

  return stm;
}

T_stm stm_store_formals(trans_t* t) {

  T_stm stm = NULL;
  frame_t* f = t->frame;
  frame_access_t* local;

  for (fal_t* cur = f->formals; cur && cur->head; 
      cur = cur->tail) {

    // r0 holds the static link, params start at r1
    stm = T_Seq(stm, stm_var_write(t, T_Name(Temp_namedlabel(regs[cur->head->reg + 1])), cur->head->sym));
  }

  return stm;
}

T_stm stm_var_access(trans_t* t, S_symbol s) {

  frame_t* frame = t->frame;
  frame_access_t* var;
  T_stm stm = NULL;
  T_exp frame_exp;

  if (!s)
    return NULL;

  var = frame_local_get(frame, s);

  // the book says the first param should always be a "static link"
  // to the expected stack frame. that makes sense to the issue I 
  // came up with the other day

  // find correct frame
  frame_exp = T_Name(Temp_namedlabel("fp"));

  for (;!frame_local_get(frame, s); frame = frame->static_link) {

    if (!frame->shared)
      frame_exp = T_Mem(frame_exp);
  }

  var = frame_local_get(frame, s);
  stm = T_Seq(stm, T_Exp(T_Mem(T_Op(T_minus, frame_exp, T_Const(var->stack_off)))));

  return stm;
}

T_stm stm_var_read(trans_t* t, T_exp dst, S_symbol s) {

  T_stm stm = stm_var_access(t, s);
  stm->u.SEQ.right = T_Move(dst, stm->u.SEQ.right->u.EXP);

  return stm;
}

T_stm stm_var_write(trans_t* t, T_exp src, S_symbol s) {

  T_stm stm = stm_var_access(t, s);
  stm->u.SEQ.right = T_Move(stm->u.SEQ.right->u.EXP, src);

  return stm;
}

// returns the frame walk from a variable access
// writes to e the actual var access from inside the frame
static T_stm stm_var_split(trans_t* t, S_symbol s, T_exp* e) {

  T_stm stm = stm_var_access(t, s);
  T_stm walk = stm->u.SEQ.left;

  *e = stm->u.SEQ.right->u.EXP;

  free(stm->u.SEQ.right);
  free(stm);

  return walk;
}

T_stm stm_loop_start(T_exp test, Temp_label start, Temp_label end) {

  loop_tracker_push(end);
  return T_Seq(T_Label(start), T_Cjump(test, NULL, end));
}

T_stm stm_loop_end(Temp_label start, Temp_label end) {

  Temp_label e = loop_tracker_pop();

  if (end != e) {

    fprintf(stderr, "TRACKER MISMATCH ERROR\n");
    exit(-1);
  }

  return T_Seq(T_Jump(T_Name(start), Temp_LabelList(start, NULL)), T_Label(end));
}

T_stm stm_for_loop(trans_t* t, S_symbol var, T_exp lo, T_exp hi, T_stm body, Temp_label end) {

  T_stm stm;
  T_exp idx;
  Temp_label start = Temp_newlabel();

  // init
  stm = stm_var_write(t, lo, var);

  // check, uses the variable from init
  stm = T_Seq(stm, stm_var_split(t, var, &idx));
  stm = T_Seq(stm, stm_loop_start(T_Op(T_le, idx, hi), start, end));

  stm = T_Seq(stm, body);

  // post increment
  stm = T_Seq(stm, stm_var_split(t, var, &idx));
  stm = T_Seq(stm, stm_var_write(t, T_Op(T_plus, idx, T_Const(1)), var));

  stm = T_Seq(stm, stm_loop_end(start, end));

  return stm;
}

T_stm stm_break() {

  Temp_label l = loop_tracker_peek();
  return T_Jump(T_Name(l), Temp_LabelList(l, NULL));
}

T_stm stm_if(T_exp iff, T_stm then, T_stm elsee) {

  T_stm stm;
  Temp_label l_else = Temp_newlabel();
  Temp_label l_end = Temp_newlabel();

  stm = T_Seq(T_Cjump(iff, NULL, elsee ? l_else : l_end), then);

  if (elsee)
    stm = T_Seq(stm, T_Seq(T_Jump(T_Name(l_end), Temp_LabelList(l_end, NULL)), T_Seq(T_Label(l_else), elsee)));

  return T_Seq(stm, T_Label(l_end));
}

T_stm stm_arr_init(Temp_temp arr, Temp_temp sz, T_exp init) {

  T_stm stm;
  Temp_temp init_tmp = Temp_newtemp();
  Temp_temp idx = Temp_newtemp();
  Temp_label start = Temp_newlabel();
  Temp_label end = Temp_newlabel();

  stm = T_Move(T_Temp(init_tmp), init);
  stm = T_Seq(stm, T_Move(T_Temp(idx), T_Const(0)));
  stm = T_Seq(stm, stm_loop_start(T_Op(T_ne, T_Temp(idx), T_Temp(sz)), start, end));

  stm = T_Seq(stm, T_Move(T_Mem(T_Op(T_plus, T_Temp(arr), T_Temp(idx))), T_Temp(init_tmp)));
  stm = T_Seq(stm, T_Move(T_Temp(idx), T_Op(T_plus, T_Temp(idx), T_Const(WORD_SZ))));

  stm = T_Seq(stm, stm_loop_end(start, end));

  return stm;
}

