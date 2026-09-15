/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"


bool anyErrors= FALSE;

static string fileName = "";

static int lineNum = 1;

int EM_tokPos=0;

extern FILE *yyin;

typedef struct intList {int i; struct intList *rest;} *IntList;

static IntList intList(int i, IntList rest) 
{IntList l= checked_malloc(sizeof *l);
 l->i=i; l->rest=rest;
 return l;
}

static IntList linePos=NULL;

typedef struct {
  int pos;
  int linenum;
  char* line;

  void* next;
} srcline_t;

srcline_t* srcline_head = NULL;

void EM_new_srcline (int pos, char* line) {

  srcline_t* t = malloc(sizeof(*t));

  t->pos = pos;
  t->linenum = lineNum;
  t->line = line;
  t->next = srcline_head;

  srcline_head = t;
}

srcline_t* EM_srcline(int pos) {

  srcline_t* cur = srcline_head;

  for (; cur ; cur = (srcline_t *)cur->next) {

    if (!cur->next)
      return cur;

    if (((srcline_t *) cur->next)->pos < pos && pos > cur->pos)
      return cur->next;
  }

  return NULL;
}

void EM_semantic_error(int pos, char* msg, ...) {

  va_list ap;
  srcline_t* line = EM_srcline(pos);

  EM_err_count += 1;

  if (fileName) fprintf(stderr,"%s:",fileName);
  if (line) fprintf(stderr,"%d.%d: \"%s\"\n\t", line->linenum, line->pos, line->line);

  va_start(ap,msg);
  vfprintf(stderr, msg, ap);
  va_end(ap);
  fprintf(stderr,"\n");
}

void EM_newline(void)
{lineNum++;
 linePos = intList(EM_tokPos, linePos);
}

void EM_error(int pos, char *message,...)
{va_list ap;
 IntList lines = linePos; 
 int num=lineNum;
 

  anyErrors=TRUE;
  while (lines && lines->i >= pos) 
       {lines=lines->rest; num--;}

  if (fileName) fprintf(stderr,"%s:",fileName);
  if (lines) fprintf(stderr,"%d.%d: ", num, pos-lines->i);
  va_start(ap,message);
  vfprintf(stderr, message, ap);
  va_end(ap);
  fprintf(stderr,"\n");

}

void EM_reset(string fname)
{
 anyErrors=FALSE; fileName=fname; lineNum=1;
 linePos=intList(0,NULL);
 yyin = fopen(fname,"r");
 if (!yyin) {EM_error(0,"cannot open"); exit(1);}
}

