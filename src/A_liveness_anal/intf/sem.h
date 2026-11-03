#pragma once

#include "absyn.h"
#include "symbol.h"
#include "trans.h"

extern S_table g_symtab;

trans_frag_list_t* sem_trans_prog(A_exp exp);

