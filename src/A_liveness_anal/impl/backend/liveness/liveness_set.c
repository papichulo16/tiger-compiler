#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "temp.h"
#include "liveness_set.h"

bool live_set_contains(Temp_tempList set, Temp_temp val) {

  bool found = false;

  for (; set && !found; set = set->tail)
    found = set->head == val;

  return found;
}

void live_set_free(Temp_tempList set) {

  Temp_tempList next;

  for (; set; set = next) {

    next = set->tail;
    free(set);
  }
}

Temp_tempList live_set(Temp_tempList tl) {

  Temp_tempList set = NULL;

  for (; tl; tl = tl->tail) {

    if (!live_set_contains(set, tl->head))
      set = Temp_TempList(tl->head, set);
  }

  return set;
}

Temp_tempList live_set_copy(Temp_tempList set) {

  Temp_tempList copy = NULL;

  for (; set; set = set->tail)
    copy = Temp_TempList(set->head, copy);

  return copy;
}

Temp_tempList live_set_union(Temp_tempList a, Temp_tempList b) {

  Temp_tempList result = live_set_copy(a);

  for (; b; b = b->tail) {

    if (!live_set_contains(a, b->head))
      result = Temp_TempList(b->head, result);
  }

  return result;
}

// a - b
Temp_tempList live_set_sub(Temp_tempList a, Temp_tempList b) {

  Temp_tempList result = NULL;

  for (; a; a = a->tail) {

    if (!live_set_contains(b, a->head))
      result = Temp_TempList(a->head, result);
  }

  return result;
}
