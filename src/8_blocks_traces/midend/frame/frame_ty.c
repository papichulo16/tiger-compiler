#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "frame.h"

frame_field_t* frame_field(S_symbol sym, size_t off, frame_field_t* next) {

  frame_field_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }

  f->sym = sym;
  f->off = off;
  f->next = next;

  return f;
}

frame_field_t* frame_fields_copy(frame_field_t* fields) {

  frame_field_t* f = NULL;

  if (!fields)
    goto end;

  f = frame_field(fields->sym, fields->off, frame_fields_copy(fields->next));

end:
  return f;
}

void frame_fields_free(frame_field_t* fields) {

  frame_field_t* next;

  for (; fields; fields = next) {
    next = fields->next;
    free(fields);
  }
}

// takes ownership of fields
void frame_add_ty(frame_t* frame, S_symbol sym, frame_field_t* fields) {

  frame_ty_t* t = malloc(sizeof(*t));

  if (!t) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }

  t->sym = sym;
  t->fields = fields;
  t->next = frame->tys;

  frame->tys = t;
}

// walks the static links so types from enclosing lets are visible,
// innermost definition wins since tys is pushed to the front
frame_field_t* frame_ty_fields_get(frame_t* frame, S_symbol sym) {

  frame_ty_t* t = NULL;

  for (; frame && !t; frame = frame->static_link)
    for (t = frame->tys; t && t->sym != sym; t = t->next);

  return t ? t->fields : NULL;
}

bool frame_field_off_get(frame_t* frame, S_symbol tyname, S_symbol field, size_t* off) {

  frame_field_t* f = frame_ty_fields_get(frame, tyname);

  while (f && f->sym != field)
    f = f->next;

  if (f)
    *off = f->off;

  return f != NULL;
}

void frame_tys_free(frame_ty_t* tys) {

  frame_ty_t* next;

  for (; tys; tys = next) {
    next = tys->next;
    frame_fields_free(tys->fields);
    free(tys);
  }
}
