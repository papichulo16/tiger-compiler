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

T_stm find_frame(trans_t* t, S_symbol s) {

  frame_access_t* var;
  bool shared;

  if (!t->frame) {

    printf("this shouldnt hit\n");
    return NULL;
  }

  var = frame_local_get(t->frame, s);

  if (var)
    return NULL;

  shared = t->frame->shared;
  t->frame = t->frame->static_link;

  if (shared)
    return find_frame(t, s);

  return T_Seq(T_Move(T_Name(Temp_namedlabel("fp")), T_Mem(T_Name(Temp_namedlabel("fp")))), find_frame(t, s));
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


T_stm stm_prologue(trans_t* t) {

  T_stm stm = T_Seq(stm_push(T_Name(Temp_namedlabel("fp"))), 
      T_Move(T_Name(Temp_namedlabel("fp")), T_Name(Temp_namedlabel("sp"))));

  if (t->frame->stack_size) {

    stm = T_Seq(stm, stm_resv_frame(t->frame->stack_size));
    stm = T_Seq(stm, stm_store_formals(t));
  }

  return stm;
}

T_stm stm_epilogue(trans_t* t) {

  T_stm stm = stm_pop(T_Name(Temp_namedlabel("fp")));

  if (t->frame->stack_size)
    stm = T_Seq(T_Move(T_Name(Temp_namedlabel("sp")), T_Name(Temp_namedlabel("fp"))) , stm);

  return stm;
}

T_stm stm_store_formals(trans_t* t) {

  T_stm stm = NULL;
  frame_t* f = t->frame;
  frame_access_t* local;

  for (fal_t* cur = f->formals; cur && cur->head; 
      cur = cur->tail) {

    stm = T_Seq(stm, stm_var_write(t, T_Name(Temp_namedlabel(regs[cur->head->reg])), cur->head->sym));
  }

  return stm;
}

T_stm stm_var_access(trans_t* t, S_symbol s) {

  frame_t* frame = t->frame;
  frame_access_t* var;
  T_stm stm = NULL;
  bool found = true;

  if (!s)
    return NULL;

  var = frame_local_get(frame, s);

  // THIS IS WRONG!!!!!!
  // FIX LATER!!!
  // the book says the first param should always be a "static link"
  // to the expected stack frame. that makes sense to the issue I 
  // came up with the other day
  if (!var) {

    // MAX_PARAMS+1 is old fp
    // MAX_PARAMS+2 is fp of variable location

    found = false;

    // find correct frame
    stm = T_Seq(T_Move(T_Name(Temp_namedlabel(regs[MAX_PARAMS+1])), 
            T_Name(Temp_namedlabel("fp"))), find_frame(t, s));

    // store fp
    stm = T_Seq(stm, T_Move(T_Name(Temp_namedlabel(regs[MAX_PARAMS+2])), T_Name(Temp_namedlabel("fp"))));

    // restore fp
    stm = T_Seq(stm, T_Move(T_Name(Temp_namedlabel("fp")), T_Name(Temp_namedlabel(regs[MAX_PARAMS+1]))));
  }

  var = frame_local_get(t->frame, s);
  stm = T_Seq(stm, T_Exp(T_Mem(T_Op(T_minus, T_Name(Temp_namedlabel(found ? "fp" : regs[MAX_PARAMS+2])), T_Const(var->stack_off)))));

  t->frame = frame;
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

