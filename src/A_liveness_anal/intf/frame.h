#pragma once

#include "util.h"
#include "symbol.h"

#define MAX_PARAMS 5

typedef struct frame_access_list_t fal_t;
typedef struct frame_t frame_t;
typedef struct frame_field_t frame_field_t;
typedef struct frame_ty_t frame_ty_t;

struct frame_field_t {
  S_symbol sym;
  size_t off;
  frame_field_t* next;
};

struct frame_ty_t {
  S_symbol sym;
  frame_field_t* fields;
  frame_ty_t* next;
};

struct frame_t {

  frame_t* static_link;

  fal_t* formals;
  fal_t* locals;
  frame_ty_t* tys;

  size_t stack_size;
  int param_count;
  bool shared;

};

typedef struct {

  S_symbol sym;

  bool in_stack;
  int stack_off;

  int reg;

} frame_access_t;

struct frame_access_list_t {
  frame_access_t* head; 
  fal_t* tail;
};

frame_t* frame_empty();
void frame_free(frame_t** f);
void frame_link(frame_t* child, frame_t* parent);
void frame_add_formal(frame_t* frame, S_symbol sym);
void frame_add_local(frame_t* frame, S_symbol sym, size_t sz);

frame_access_t* frame_local_get(frame_t* f, S_symbol s);

frame_field_t* frame_field(S_symbol sym, size_t off, frame_field_t* next);
frame_field_t* frame_fields_copy(frame_field_t* fields);
void frame_fields_free(frame_field_t* fields);

void frame_add_ty(frame_t* frame, S_symbol sym, frame_field_t* fields);
frame_field_t* frame_ty_fields_get(frame_t* frame, S_symbol sym);
bool frame_field_off_get(frame_t* frame, S_symbol tyname, S_symbol field, size_t* off);
void frame_tys_free(frame_ty_t* tys);
