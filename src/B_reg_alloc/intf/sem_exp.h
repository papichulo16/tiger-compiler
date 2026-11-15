#pragma once

#include "types.h"
#include "absyn.h"
#include "trans.h"

Ty_tyList sem_trans_expl(trans_t* t, A_expList el);
Ty_ty sem_trans_exp(trans_t* t, A_exp e);
