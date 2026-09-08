#include <stdio.h>
#include "util.h"
#include "errormsg.h"

extern int yyparse(void);
extern int yydebug;

void parse(string fname) 
{

  EM_reset(fname);
  //yydebug = 1;

  if (yyparse() == 0) /* parsing worked */
    fprintf(stderr,"Parsing successful!\n");
  else 
    fprintf(stderr,"Parsing failed\n");

  printf("%d errors caught by parser\n", EM_err_count);
}

int main(int argc, char **argv) {
 if (argc!=2) {fprintf(stderr,"usage: %s filename\n", argv[0]); exit(1);}
 parse(argv[1]);
 return 0;
}
