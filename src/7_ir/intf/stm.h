#pragma once

#include "symbol.h"
#include "tree.h"
#include "trans.h"

T_stm stm_prologue(trans_t* t);
T_stm stm_epilogue(trans_t* t);

T_stm stm_resv_frame(int stack_sz);
T_stm stm_store_formals(trans_t* t);
T_stm stm_var_access(trans_t* t, S_symbol s);
T_stm stm_var_read(trans_t* t, T_exp dst, S_symbol s);
T_stm stm_var_write(trans_t* t, T_exp src, S_symbol s);

T_stm stm_pop(T_exp dst);
T_stm stm_push(T_exp src);

void loop_tracker_push(Temp_label v);
Temp_label loop_tracker_pop();

T_stm stm_loop_start(T_exp test, Temp_label start, Temp_label end);
T_stm stm_loop_end(Temp_label start, Temp_label end);
T_stm stm_for_loop(trans_t* t, S_symbol var, T_exp lo, T_exp hi, T_stm body, Temp_label end);
T_stm stm_if(T_exp iff, T_stm then, T_stm elsee);
T_stm stm_break();
T_stm stm_arr_init(Temp_temp arr, Temp_temp sz, T_exp init);
