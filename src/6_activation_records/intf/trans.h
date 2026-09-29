#pragma once

typedef struct trans_t trans_t;

trans_t* trans_new();
trans_t* trans_new_link(trans_t* parent);

void trans_free(trans_t** t);

void trans_add_formal(trans_t* t, S_symbol s);
void trans_add_local(trans_t* t, S_symbol s, size_t sz);
