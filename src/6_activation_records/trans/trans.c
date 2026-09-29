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
  trans_t* static_link;
};

trans_t* trans_new() {

  trans_t* t = calloc(1, sizeof(trans_t));

  // new frame

  return t;
}

trans_t* trans_new_link(trans_t* parent) {

  trans_t* t = trans_new();
  t->static_link = parent;

  return t;
}

void trans_free(trans_t** t) {

  trans_t* tr = *t;

  if (!tr)
    return;

  if (tr->frame)
    free(tr->frame);

  free(tr);
  *t = NULL;
}

void trans_add_formal(trans_t* t, S_symbol s) {

  printf("formal var '%s'\n", S_name(s));
}

void trans_add_local(trans_t* t, S_symbol s, size_t sz) {

  printf("local var '%s' with size %d\n", S_name(s), sz);
}

