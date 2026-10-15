#pragma once

#include "symbol.h"
#include "tree.h"
#include "trans.h"

T_stm stm_prologue(trans_t* t);
T_stm stm_epilogue(trans_t* t);

T_stm stm_resv_frame(int stack_sz);
T_stm stm_store_formals(trans_t* t);
T_stm stm_var_read(trans_t* t, T_exp dst, S_symbol s);
T_stm stm_var_write(trans_t* t, T_exp src, S_symbol s);

T_stm stm_pop(T_exp dst);
T_stm stm_push(T_exp src);

