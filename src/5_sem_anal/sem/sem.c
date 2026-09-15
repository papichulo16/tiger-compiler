#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"
#include "types.h"
#include "sem.h"

void sem_trans_decl(A_decList dl);
void sem_trans_fundecl(A_fundecList fl);
void sem_trans_nametyl(A_nametyList nl);
void sem_trans_fieldl(A_fieldList fl);

void sem_trans_field(A_field f);
void sem_trans_fundec(A_fundec f);
void sem_trans_namety(A_namety n);
void sem_trans_dec(A_dec d);
void sem_trans_exp(A_exp e);

S_table g_symtab = NULL;

void sem_trans_decl(A_decList dl) {

  if (!dl)
    return;

  if (dl->head) 
    sem_trans_dec(dl->head);

  sem_trans_decl(dl->tail);

  free(dl);
}

void sem_trans_fieldl(A_fieldList fl) {

  if (!fl)
    return;

  if (fl->head)
    sem_trans_field(fl->head);

  sem_trans_fieldl(fl->tail);

  free(fl);
}

void sem_trans_fundecl(A_fundecList fl) {

  if (!fl)
    return;

  if (fl->head) 
    sem_trans_fundec(fl->head);

  sem_trans_fundecl(fl->tail);

  free(fl);
}

void sem_trans_nametyl(A_nametyList nl) {

  if (!nl)
    return;

  if (nl->head) 
    sem_trans_namety(nl->head);

  sem_trans_nametyl(nl->tail);

  free(nl);
}

void sem_trans_field(A_field f) {

  if (!f)
    return;


}

void sem_trans_fundec(A_fundec f) {

  if (!f)
    return;

  // handle
  S_symbol n = f->name;
  S_symbol r = f->result;
  A_fieldList p = f->params; 

  sem_trans_exp(f->body);

  free(f);
}

void sem_trans_namety(A_namety n) {

  if (!n)
    return;

  // handle
  if (!sem_sym_ty_add(g_symtab, n->name, n->ty))
    printf("semantic error adding type\n");

  free(n);
}

void sem_trans_dec(A_dec d) {

  if (!d)
    return;

  switch (d->kind) {

    case A_functionDec:

      sem_trans_fundecl(d->u.function);
      break;

    case A_varDec:

      // handle
      S_symbol v = d->u.var.var;
      Ty_ty t = sem_sym_type_get(g_symtab, d->u.var.typ);

      sem_trans_exp(d->u.var.init);

      if (!t || sem_sym_inuse(g_symtab, v))
        break;

      //S_enter(g_symtab, v, )

      break;

    case A_typeDec:

      sem_trans_nametyl(d->u.type);
      break;

  }

  free(d);
}

void sem_trans_exp(A_exp e) {

  if (!e)
    return;

  switch (e->kind) {

    case A_varExp:
      break;

    case A_nilExp:
      break;

    case A_intExp:
      break;

    case A_stringExp:
      break;

    case A_callExp:
      break;

    case A_opExp:
      break;

    case A_recordExp:
      break;

    case A_seqExp:
      break;

    case A_assignExp:
      break;

    case A_ifExp:
      break;

    case A_whileExp:
      break;

    case A_forExp:
      break;

    case A_breakExp:
      break;

    case A_letExp:

      S_beginScope(g_symtab);

      sem_trans_decl(e->u.let.decs);
      sem_trans_exp(e->u.let.body);

      S_endScope(g_symtab);

      break;

    case A_arrayExp:
      break;
  }

  free(e);
}

void sem_trans_prog(A_exp e) {

  if (!e)
    return;

  g_symtab = S_empty();

  sem_trans_exp(e);

  if (g_symtab) {

    free(g_symtab);
    g_symtab = NULL;
  }
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
    return -1;

  fd = fopen("./out", "w+");
  pr_exp(fd, root, 10);
  fclose(fd);

  sem_trans_prog(root);

  return 0;
}

