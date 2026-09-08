/*
 * parse.c - Parse source file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"

extern int yyparse(void);
extern A_exp absyn_root;

/* parse source file fname; 
   return abstract syntax data structure */
A_exp parse(string fname) 
{
  EM_reset(fname);

  if (yyparse() || EM_err_count) {

    fprintf(stdout, "parse failed\n");

    return NULL;
  } 

  fprintf(stdout, "parse passed\n");

  return absyn_root;
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

  //fd = fopen("./out", "w+");
  pr_exp(fd, root, 10);
  fclose(fd);

  return 0;
}

