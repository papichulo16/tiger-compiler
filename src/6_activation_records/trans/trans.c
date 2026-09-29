#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "frame.h"
#include "trans.h"

struct trans_t {
  frame_t* frame;
  trans static_link;
};

trans trans_new() {

  trans t = calloc(1, sizeof(struct trans_t));

  // new frame

  return t;
}

trans trans_new_link(trans parent) {

  trans t = trans_new();
  t->static_link = parent;

  return t;
}

void trans_free(trans* t) {

  trans tr = *t;

  if (!tr)
    return;

  if (tr->frame)
    free(tr->frame);

  free(tr);
  *t = NULL;
}

void trans_add_formal(trans t, S_symbol s) {

  printf("formal var '%s'\n", S_name(s));
}

void trans_add_local(trans t, S_symbol s, size_t sz) {

  printf("local var '%s' with size %d\n", S_name(s), sz);
}

