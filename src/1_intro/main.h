#include "util.h"

typedef struct {

  string id;
  int val;

  void* next;

} intvar_node_t;

typedef struct {

  int val;
  void* next;

} explist_t;

// these pass ownership, then free at the end
void parse_stm(A_stm stm);
int eval_exp(A_exp exp);
explist_t* eval_explist(A_expList expl);

