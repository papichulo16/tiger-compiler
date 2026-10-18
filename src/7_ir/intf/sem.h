#pragma once

#include "absyn.h"
#include "symbol.h"

extern S_table g_symtab;

void sem_trans_prog(A_exp exp);

