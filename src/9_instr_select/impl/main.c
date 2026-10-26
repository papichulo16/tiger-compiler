#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "errormsg.h"
#include "absyn.h"
#include "parse.h"
#include "tree.h"
#include "canon.h"
#include "printtree.h"
#include "trans.h"
#include "sem.h"
#include "assem.h"
#include "codegen.h"

static void comp_err(void) {

  printf("Compilation error\n");
  exit(-1);
}

static void main_print_instrs(FILE* out, AS_instrList instrs) {

  for (; instrs; instrs = instrs->tail) {

    if (instrs->head->kind != I_LABEL)
      fprintf(out, "    ");

    AS_print(out, instrs->head, Temp_name());
    fprintf(out, "\n");
  }
}

static void main_do_proc(FILE* out, Temp_label label, T_stm body) {

  T_stmList stms = C_traceSchedule(C_basicBlocks(C_linearize(body)));
  AS_instrList instr_list = codegen(stms);

  fprintf(out, "BEGIN %s\n", Temp_labelstring(label));

  fprintf(out, "IR\n");
  printStmList(out, stms);

  fprintf(out, "\nASM\n");
  main_print_instrs(out, instr_list);

  fprintf(out, "END %s\n\n", Temp_labelstring(label));
}

static void main_emit_frags(FILE* out, trans_frag_list_t* frags) {

  for (; frags; frags = frags->tail) {

    trans_frag_t* f = frags->head;

    if (f->kind == TRANS_PROC_FRAG)
      main_do_proc(out, f->u.proc.label, f->u.proc.body);
    else
      fprintf(out, "%s: \"%s\"\n", Temp_labelstring(f->u.str.label), f->u.str.str);
  }
}

int main(int argc, char** argv) {

  A_exp root;
  trans_frag_list_t* frags;
  char outfile[256];
  FILE* out;

  if (argc != 2) {

    fprintf(stderr, "usage: %s file.tig\n", argv[0]);
    exit(1);
  }

  root = parse(argv[1]);

  if (!root)
    comp_err();

  frags = sem_trans_prog(root);

  if (EM_err_count)
    comp_err();

  snprintf(outfile, sizeof(outfile), "%s.s", argv[1]);
  out = fopen(outfile, "w");

  if (!out) {

    perror(outfile);
    exit(1);
  }

  main_emit_frags(out, frags);

  fclose(out);
  trans_frags_free(&frags);
  trans_strs_free();

  return 0;
}

