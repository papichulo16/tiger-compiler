#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "types.h"
#include "frame.h"
#include "trans.h"
#include "temp.h"
#include "tree.h"
#include "printtree.h"

tfl_t* all_funs = NULL;
static trans_str_t* g_strs = NULL;

static void print_strs(void) {

  for (trans_str_t* s = g_strs; s; s = s->next)
    printf("%s: \"%s\"\n", Temp_labelstring(s->label), s->str);
}

void print_trans(trans_t* t) {

  TRANS_SEM_ERR_RET();
  printStmList(stdout, T_StmList(t->ir, NULL));
  print_strs();
}

Temp_label trans_str_label(string s) {

  trans_str_t* str;

  for (str = g_strs; str; str = str->next)
    if (!strcmp(str->str, s))
      goto out;

  str = checked_malloc(sizeof(*str));
  str->label = Temp_newlabel();
  str->str = String(s);
  str->next = g_strs;
  g_strs = str;

out:
  return str->label;
}

void trans_strs_free(void) {

  trans_str_t* next;

  for (; g_strs; g_strs = next) {

    next = g_strs->next;
    free(g_strs->str);
    free(g_strs);
  }
}

void trans_append_all_funs(trans_t* t) {

  tfl_t* cur = all_funs;

  TRANS_SEM_ERR_RET();

  if (!cur)
    return;

  for (trans_fun_t* c = cur->head; cur && c; cur = cur->tail, c = cur->head) {

    t->ir = T_Seq(t->ir, c->ir);

    if (!cur->tail)
      break;
  }
}

tfl_t* tfl(trans_fun_t* head, tfl_t* tail) {

  tfl_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }
  
  f->head = head;
  f->tail = tail;

  return f;
}

trans_fun_t* trans_fun(S_symbol s) {

  trans_fun_t* f = malloc(sizeof(*f));

  if (!f) {

    fprintf(stderr, "malloc gay\n");
    exit(-1);
  }
  
  f->sym = s;
  f->label = Temp_newlabel();
  f->param_count = 0;
  f->ir = NULL;

  return f;
}

trans_t* trans_new() {

  trans_t* t = calloc(1, sizeof(trans_t));
  t->frame = frame_empty();
  t->fun_list = tfl(NULL, NULL);
  
  return t;
}

trans_t* trans_new_link(trans_t* parent) {

  trans_t* t = trans_new();
  t->parent = parent;
  frame_link(t->frame, parent->frame);

  return t;
}

trans_t* trans_new_link_shared_frame(trans_t* parent) {

  trans_t* t = trans_new_link(parent);
  t->frame->stack_size = parent->frame->stack_size;
  t->frame->shared = true;

  return t;
}

static void tfl_free(tfl_t* l) {

  tfl_t* next;

  for (; l; l = next) {

    next = l->tail;

    if (l->head) {
      //trans_free(&l->head->ir);
      free(l->head);
    }

    free(l);
  }
}

void trans_free(trans_t** t) {

  trans_t* tr;

  if (!t)
    return;

  tr = *t;

  if (!tr)
    return;

  frame_free(&tr->frame);
  //tfl_free(tr->fun_list);

  free(tr);
  *t = NULL;
}

trans_fun_t* trans_declare_fun(trans_t* parent, S_symbol name, int param_count) {

  trans_fun_t* f;

  TRANS_SEM_ERR_RET(NULL);

  f = trans_fun(name);
  f->param_count = param_count;

  parent->fun_list = tfl(f, parent->fun_list);
  all_funs = tfl(f, all_funs);

  return f;
}

void trans_define_fun(trans_fun_t* f, trans_t* child) {

  TRANS_SEM_ERR_RET();

  f->ir = T_Seq(T_Label(f->label), child->ir);
}

trans_fun_t* trans_fun_get(trans_t* t, S_symbol s) {

  trans_fun_t* f = NULL;

  for (; t && !f; t = t->parent) {

    for (tfl_t* l = t->fun_list; l; l = l->tail) {

      if (l->head && l->head->sym == s) {

        f = l->head;
        break;
      }
    }
  }

  return f;
}

Temp_label trans_fun_label(trans_t* t, S_symbol s) {

  trans_fun_t* f;

  TRANS_SEM_ERR_RET(NULL);

  f = trans_fun_get(t, s);

  return f ? f->label : NULL;
}

void trans_append_child(trans_t* parent, trans_t* child) {

  TRANS_SEM_ERR_RET();

  parent->ir = T_Seq(parent->ir, child->ir);
}

void trans_add_formal(trans_t* t, S_symbol s) {

  TRANS_SEM_ERR_RET();

  //printf("formal var '%s'\n", S_name(s));
  frame_add_formal(t->frame, s);
}

void trans_add_local(trans_t* t, S_symbol s, size_t sz) {

  TRANS_SEM_ERR_RET();

  //printf("local var '%s' with size %d\n", S_name(s), sz);
  frame_add_local(t->frame, s, sz);
}

void trans_add_ty(trans_t* t, S_symbol s, frame_field_t* fields) {

  frame_add_ty(t->frame, s, fields);
}

frame_field_t* trans_ty_fields_get(trans_t* t, S_symbol s) {

  TRANS_SEM_ERR_RET(NULL);

  return frame_ty_fields_get(t->frame, s);
}
