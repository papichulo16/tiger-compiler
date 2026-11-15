#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "trans.h"

static trans_frag_list_t* trans_frag_list(trans_frag_t* head, trans_frag_list_t* tail) {

  trans_frag_list_t* l = checked_malloc(sizeof(*l));

  l->head = head;
  l->tail = tail;

  return l;
}

static trans_frag_t* trans_proc_frag(Temp_label label, T_stm body, frame_t* frame) {

  trans_frag_t* f = checked_malloc(sizeof(*f));

  f->kind = TRANS_PROC_FRAG;
  f->u.proc.label = label;
  f->u.proc.body = body;
  f->u.proc.frame = frame;

  return f;
}

static trans_frag_t* trans_str_frag(Temp_label label, string str) {

  trans_frag_t* f = checked_malloc(sizeof(*f));

  f->kind = TRANS_STR_FRAG;
  f->u.str.label = label;
  f->u.str.str = str;

  return f;
}

static trans_frag_list_t* trans_frags_take_strs(trans_frag_list_t* frags) {

  trans_str_t* next;

  for (trans_str_t* s = trans_strs_take(); s; s = next) {

    next = s->next;
    frags = trans_frag_list(trans_str_frag(s->label, s->str), frags);
    free(s);
  }

  return frags;
}

static trans_frag_list_t* trans_frags_take_funs(trans_frag_list_t* frags) {

  tfl_t* next;

  for (tfl_t* l = trans_funs_take(); l; l = next) {

    next = l->tail;
    frags = trans_frag_list(trans_proc_frag(l->head->label, l->head->ir, l->head->frame), frags);
    free(l->head);
    free(l);
  }

  return frags;
}

trans_frag_list_t* trans_frags_take(trans_t* t) {

  trans_frag_list_t* frags = NULL;
  Temp_label main_label = Temp_namedlabel("tigermain");

  frags = trans_frags_take_strs(frags);
  frags = trans_frags_take_funs(frags);
  frags = trans_frag_list(trans_proc_frag(main_label, T_Seq(T_Label(main_label), t->ir), t->frame), frags);

  t->ir = NULL;
  t->frame = NULL;

  return frags;
}

void trans_frags_free(trans_frag_list_t** l) {

  trans_frag_list_t* next;

  if (!l)
    return;

  for (trans_frag_list_t* cur = *l; cur; cur = next) {

    next = cur->tail;

    if (cur->head->kind == TRANS_PROC_FRAG)
      frame_free(&cur->head->u.proc.frame);
    else
      free(cur->head->u.str.str);

    free(cur->head);
    free(cur);
  }

  *l = NULL;
}

