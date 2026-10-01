#pragma once

typedef struct frame_access_list_t fal_t;
typedef struct frame_t frame_t;

struct frame_t {

  frame_t* static_link;

  fal_t* formals;
  fal_t* locals;

  size_t stack_size;

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
void frame_link(frame_t* child, frame_t* parent);
void frame_add_formal(frame_t* frame, S_symbol sym);
void frame_add_local(frame_t* frame, S_symbol sym, size_t sz);

/*
F_frame F_newFrame(Temp_label name, U_boolList formals);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape);
*/

