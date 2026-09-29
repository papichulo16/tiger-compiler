#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "frame.h"
#include "trans.h"

struct trans_t {
  F_frame f;
};

trans trans_new() { return NULL; }

trans trans_new_link(trans parent) { return NULL; }

void trans_free(trans* t) {}

void trans_add_formal(trans t, S_symbol s) {}

void trans_add_local(trans t, S_symbol s, size_t sz) {}

