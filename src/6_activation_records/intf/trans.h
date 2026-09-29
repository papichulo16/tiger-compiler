#pragma once

typedef struct trans_t* trans;

trans trans_new();
trans trans_new_link(trans parent);

void trans_free(trans* t);

void trans_add_formal(trans t, S_symbol s);

