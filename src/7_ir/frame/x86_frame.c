#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "temp.h"
#include "trans.h"
#include "frame.h"

#ifdef X86_FRAME

int WORD_SZ = 8;

fal_t* fal(frame_access_t* head, fal_t* tail) {

  fal_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }
  
  f->head = head;
  f->tail = tail;

  return f;
}

// off will be used as reg if !in_stack
frame_access_t* frame_access(S_symbol sym, bool in_stack, int off) {

  frame_access_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }
  
  f->sym = sym;
  f->in_stack = in_stack;

  if (in_stack)
    f->stack_off = off;
  else
    f->reg = off;

  return f;
}

frame_t* frame_empty() {

  frame_t* f = calloc(1, sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }

  f->formals = fal(NULL, NULL);
  f->locals = fal(NULL, NULL);

  return f;
}

static void fal_free(fal_t* f) {

  fal_t* next;

  for (; f; f = next) {
    next = f->tail;
    free(f->head);
    free(f);
  }
}

void frame_free(frame_t** f) {

  if (!f || !*f)
    return;

  fal_free((*f)->formals);
  fal_free((*f)->locals);

  free(*f);
  *f = NULL;
}

void frame_link(frame_t* child, frame_t* parent) {

  if (!child || !parent)
    return;

  child->static_link = parent;
  // child->stack_size += WORD_SZ; // beginning of stack should hold prev frame
}

void frame_add_formal(frame_t* frame, S_symbol sym) {

  fal_t* f = frame->formals;
  int count = 0;

  for(; f->head ; f = f->tail, count++);

  if (count > 6)
    return;

  f->head = frame_access(sym, false, count);
  f->tail = fal(NULL, NULL);

  frame->stack_size += WORD_SZ;
}

void frame_add_local(frame_t* frame, S_symbol sym, size_t sz) {

  fal_t* f = frame->locals;

  for(; f->head ; f = f->tail);
  f->head = frame_access(sym, true, frame->stack_size);
  f->tail = fal(NULL, NULL);

  frame->stack_size += sz;
}

frame_access_t* frame_local_get(frame_t* f, S_symbol s) {

  fal_t* fa; 

  if (!f) {
    printf("what the fart\n");
    return NULL;
  }

  for (fa = f->locals; fa && fa->head && fa->head->sym != s
      ; fa = fa->tail );

  return (fa && fa->head) ? fa->head : NULL;

}

#endif

