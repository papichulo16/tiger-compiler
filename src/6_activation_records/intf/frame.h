#pragma once

#include "temp.h"

typedef struct frame_access_list_t fal_t;

typedef struct {
  int a;
} frame_t;

typedef struct {
  int a;
} frame_access_t;

struct frame_access_list_t {frame_access_t* head; fal_t* tail;};

/*
F_frame F_newFrame(Temp_label name, U_boolList formals);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape);
*/

