/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
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

static bool em_pos_resolve(int pos, int* line, int* col)
{
  IntList lines = linePos;
  int num = lineNum;

  while (lines && lines->i >= pos) {
    lines = lines->rest;
    num--;
  }

  if (lines) {
    *line = num;
    *col = pos - lines->i;
  }

  return lines != NULL;
}

static char* em_line_read(int num)
{
  FILE* f = fopen(fileName, "r");
  char* buf = NULL;
  char* ret = NULL;
  size_t cap = 0;
  int i;

  if (!f) goto out;

  for (i = 1; i <= num; i++)
    if (getline(&buf, &cap, f) == -1) goto out;

  buf[strcspn(buf, "\r\n")] = '\0';
  ret = buf;
  buf = NULL;

out:
  free(buf);
  if (f) fclose(f);
  return ret;
}

void EM_semantic_error(int pos, char* msg, ...)
{
  va_list ap;
  int line = 0;
  int col = 0;
  char* text = NULL;
  bool found = em_pos_resolve(pos, &line, &col);

  EM_err_count += 1;

  if (found) text = em_line_read(line);

  if (fileName) fprintf(stderr, "%s:", fileName);
  if (found) fprintf(stderr, "%d.%d: ", line, col);
  if (text) fprintf(stderr, "\"%s\"\n\t", text);

  va_start(ap, msg);
  vfprintf(stderr, msg, ap);
  va_end(ap);
  fprintf(stderr, "\n");

  free(text);
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

