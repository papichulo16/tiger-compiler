#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"
#include "parse.h"
#include "prabsyn.h"
#include "types.h"
#include "sem.h"
#include "sem_sym.h"
#include "sem_exp.h"
#include "trans.h"
#include "canon.h"

S_table g_symtab = NULL;

void free_global(void** p) {

  free(*p);
  *p = NULL;
}

void sem_trans_prog(A_exp e) {

  trans_t* t;

  if (!e)
    return;

  g_symtab = S_empty();
  t = trans_new();

  sem_sym_std_add(g_symtab);

  sem_trans_exp(t, e);

  trans_append_all_funs(t);
  print_trans(t);
  C_linearize(t->ir);

  if (g_symtab)
    free_global((void **) &g_symtab);
  if (t)
    trans_free(&t);

  trans_strs_free();
}

void comp_err() {

  printf("Compilation error\n");
  exit(-1);
}

int main(int argc, char** argv) {

  A_exp root;
  FILE* fd;

  if (argc < 2) {
    
    fprintf(stderr,"usage: %s filename\n", argv[0]); 
    exit(1);
  }

  root = parse(argv[1]);

  if (!root)
    comp_err();

  fd = fopen("./out", "w+");
  pr_exp(fd, root, 10);
  fclose(fd);

  sem_trans_prog(root);

  if (EM_err_count)
    comp_err();

  return 0;
}

