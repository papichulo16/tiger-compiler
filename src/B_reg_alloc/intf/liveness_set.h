#pragma once

#include <stdbool.h>

#include "temp.h"

bool live_set_contains(Temp_tempList set, Temp_temp val);
bool live_set_subset(Temp_tempList a, Temp_tempList b);
bool live_set_equal(Temp_tempList a, Temp_tempList b);
void live_set_free(Temp_tempList set);

Temp_tempList live_set(Temp_tempList tl);
Temp_tempList live_set_copy(Temp_tempList set);
Temp_tempList live_set_union(Temp_tempList a, Temp_tempList b);
Temp_tempList live_set_sub(Temp_tempList a, Temp_tempList b);

