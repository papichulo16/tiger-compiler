/*
 * canon.c - Functions to convert the IR trees into basic blocks and traces.
 *
 */
#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "canon.h"

typedef struct expRefList_ *expRefList;
struct expRefList_ {T_exp *head; expRefList tail;};

/* local function prototypes */
static T_stm do_stm(T_stm stm);
static struct stmExp do_exp(T_exp exp);
static C_stmListList mkBlocks(T_stmList stms, Temp_label done);
static T_stmList getNext(void);

static expRefList ExpRefList(T_exp *head, expRefList tail)
{expRefList p = (expRefList) checked_malloc (sizeof *p);
 p->head=head; p->tail=tail;
 return p;
}

/* trans builds the ir as T_Seq(NULL, ...) chains, so NULL stms are nops */
static bool isNop(T_stm x) 
{  return !x || (x->kind == T_EXP && x->u.EXP->kind == T_CONST);
 }

static T_stm seq(T_stm x, T_stm y)
{
 if (isNop(x)) return y;
 if (isNop(y)) return x;
 return T_Seq(x,y);
}

static bool commute(T_stm x, T_exp y)
{
 if (isNop(x)) return TRUE;
 /* NAMEs double as registers (sp, fp, r0..), which stms can clobber */
 if (y->kind == T_CONST) return TRUE;
 return FALSE;
}

struct stmExp {T_stm s; T_exp e;};

static T_stm reorder(expRefList rlist) {
   if (!rlist) return T_Exp(T_Const(0)); /* nop */
   else if ((*rlist->head)->kind==T_CALL) {
      Temp_temp t = Temp_newtemp();
      *rlist->head = T_Eseq(T_Move(T_Temp(t),*rlist->head),T_Temp(t));
      return reorder(rlist);
    }
   else {
      struct stmExp hd = do_exp(*rlist->head);
      T_stm s = reorder(rlist->tail);
      if (commute(s,hd.e)) {
	 *rlist->head=hd.e;
         return seq(hd.s,s);
      } else {
        Temp_temp t = Temp_newtemp();
        *rlist->head=T_Temp(t);
        return seq(hd.s, seq(T_Move(T_Temp(t),hd.e), s));
      }
    }
 }

/* registers are either temps or NAMEs like sp, fp, r0 */
static bool is_reg(T_exp exp)
{
 return exp->kind == T_TEMP || exp->kind == T_NAME;
}

/* args are MOVE(reg, exp) stms that must run right before the call, so only
   their srcs get hoisted out, keeping nested calls from clobbering the regs */
static expRefList get_call_rlist(T_exp exp)
{
 struct expRefList_ head = {NULL, NULL};
 expRefList last = &head;
 T_stmList args;
 /* a NAME fun is a code label, not a register, so it never needs hoisting */
 if (exp->u.CALL.fun->kind != T_NAME)
   last = last->tail = ExpRefList(&exp->u.CALL.fun, NULL);
 for (args = exp->u.CALL.args; args; args = args->tail) {
   T_stm s = args->head;
   assert(s->kind == T_MOVE && is_reg(s->u.MOVE.dst));
   last = last->tail = ExpRefList(&s->u.MOVE.src, NULL);
 }
 return head.tail;
}

static struct stmExp StmExp(T_stm stm, T_exp exp) {
  struct stmExp x;
  x.s = stm;
  x.e = exp;
  return x;
}

static struct stmExp do_exp(T_exp exp)
{
  switch(exp->kind) {
  case T_OP: 
    return StmExp(reorder(ExpRefList(&exp->u.OP.left, 
		       ExpRefList(&exp->u.OP.right, NULL))),
		  exp);
  case T_MEM: 
    return StmExp(reorder(ExpRefList(&exp->u.MEM, NULL)), exp);
  case T_ESEQ:
    {struct stmExp x = do_exp(exp->u.ESEQ.exp);
     return StmExp(seq(do_stm(exp->u.ESEQ.stm), x.s), x.e);
    }
  case T_CALL:    
      return StmExp(reorder(get_call_rlist(exp)), exp);
  default:
    return StmExp(reorder(NULL), exp);
  }
}

/* processes stm so that it contains no ESEQ nodes */
static T_stm do_stm(T_stm stm)
{
  if (!stm)
    return T_Exp(T_Const(0));
  switch (stm->kind) {
  case T_SEQ: 
    return seq(do_stm(stm->u.SEQ.left), do_stm(stm->u.SEQ.right));
  case T_JUMP:
    return seq(reorder(ExpRefList(&stm->u.JUMP.exp, NULL)), stm);
  case T_CJUMP:
    return seq(reorder(ExpRefList(&stm->u.CJUMP.cond, NULL)), stm);
  case T_MOVE:
    if (is_reg(stm->u.MOVE.dst) && stm->u.MOVE.src->kind == T_CALL)
      return seq(reorder(get_call_rlist(stm->u.MOVE.src)), stm);
    else if (is_reg(stm->u.MOVE.dst))
      return seq(reorder(ExpRefList(&stm->u.MOVE.src, NULL)), stm);
    else if (stm->u.MOVE.dst->kind == T_MEM)
      return seq(reorder(ExpRefList(&stm->u.MOVE.dst->u.MEM, 
			 ExpRefList(&stm->u.MOVE.src, NULL))), stm);
    else if (stm->u.MOVE.dst->kind == T_ESEQ) {
      T_stm s = stm->u.MOVE.dst->u.ESEQ.stm;
      stm->u.MOVE.dst = stm->u.MOVE.dst->u.ESEQ.exp;
      return do_stm(T_Seq(s, stm));
    }
    assert(0); /* dst should be temp, register name or mem only */
  case T_EXP:
    if (stm->u.EXP->kind == T_CALL)
         return seq(reorder(get_call_rlist(stm->u.EXP)), stm);
    else return seq(reorder(ExpRefList(&stm->u.EXP, NULL)), stm);
 default:
    return stm;
 }
}

/* linear gets rid of the top-level SEQ's, producing a list */
static T_stmList linear(T_stm stm, T_stmList right)
{
 if (isNop(stm))
   return right;
 if (stm->kind == T_SEQ) 
   return linear(stm->u.SEQ.left,linear(stm->u.SEQ.right,right));
 else return T_StmList(stm, right);
}

/* From an arbitrary Tree statement, produce a list of cleaned trees
   satisfying the following properties:
      1.  No SEQ's or ESEQ's
      2.  The parent of every CALL is an EXP(..) or a MOVE(TEMP t,..) */
T_stmList C_linearize(T_stm stm)
{
    return linear(do_stm(stm), NULL);
}

static C_stmListList StmListList(T_stmList head, C_stmListList tail)
{C_stmListList p = (C_stmListList) checked_malloc (sizeof *p);
 p->head=head; p->tail=tail;
 return p;
}
 
/* a NULL CJUMP label means fall through, so point it at a label on the
   following stm, since every block after a CJUMP must start with one */
static T_stmList fill_fall_through(T_stm s, T_stmList rest)
{
 Temp_label lab;
 if (s->kind != T_CJUMP || (s->u.CJUMP.tr && s->u.CJUMP.fa))
   return rest;
 if (rest && rest->head->kind == T_LABEL)
   lab = rest->head->u.LABEL;
 else {
   lab = Temp_newlabel();
   rest = T_StmList(T_Label(lab), rest);
 }
 if (!s->u.CJUMP.tr) s->u.CJUMP.tr = lab;
 if (!s->u.CJUMP.fa) s->u.CJUMP.fa = lab;
 return rest;
}

/* Go down a list looking for end of basic block */
static C_stmListList next(T_stmList prevstms, T_stmList stms, Temp_label done)
{
  if (!stms)
    return next(prevstms,
		T_StmList(T_Jump(T_Name(done), Temp_LabelList(done, NULL)),
			  NULL), done);
  if (stms->head->kind == T_JUMP || stms->head->kind == T_CJUMP) {
    C_stmListList stmLists;
    prevstms->tail = stms;
    stms->tail = fill_fall_through(stms->head, stms->tail);
    stmLists = mkBlocks(stms->tail, done);
    stms->tail = NULL;
    return stmLists;
  } 
  else if (stms->head->kind == T_LABEL) {
    Temp_label lab = stms->head->u.LABEL;
    return next(prevstms, T_StmList(T_Jump(T_Name(lab), Temp_LabelList(lab, NULL)), 
			     stms), done);
  }
  else {
    prevstms->tail = stms;
    return next(stms, stms->tail, done);
  }
}

/* Create the beginning of a basic block */
static C_stmListList mkBlocks(T_stmList stms, Temp_label done)
{
  if (!stms) { 
    return NULL;
  }
  if (stms->head->kind != T_LABEL) {
    return mkBlocks(T_StmList(T_Label(Temp_newlabel()), stms), done);
  }
  /* else there already is a label */
  return StmListList(stms, next(stms, stms->tail, done));
}

        /* basicBlocks : Tree.stm list -> (Tree.stm list list * Tree.label)
	       From a list of cleaned trees, produce a list of
	 basic blocks satisfying the following properties:
	      1. and 2. as above;
	      3.  Every block begins with a LABEL;
              4.  A LABEL appears only at the beginning of a block;
              5.  Any JUMP or CJUMP is the last stm in a block;
              6.  Every block ends with a JUMP or CJUMP;
           Also produce the "label" to which control will be passed
           upon exit.
        */
struct C_block C_basicBlocks(T_stmList stmList)
{
  struct C_block b;
  b.label = Temp_newlabel(); 
  b.stmLists = mkBlocks(stmList, b.label); 

  return b;
}

static S_table block_env;
static struct C_block global_block;

static T_stmList getLast(T_stmList list)
{
  T_stmList last = list;
  while (last->tail->tail) last = last->tail;
  return last;
}

/* conds are arbitrary exps, so only relational ops can be flipped in place */
static T_exp not_cond(T_exp cond)
{
 if (cond->kind == T_OP && cond->u.OP.op >= T_eq)
   return T_Op(T_notRel(cond->u.OP.op), cond->u.OP.left, cond->u.OP.right);
 return T_Op(T_eq, cond, T_Const(0));
}

static void trace(T_stmList list)
{
  T_stmList last = getLast(list);
  T_stm lab = list->head;
  T_stm s = last->tail->head;
  S_enter(block_env, lab->u.LABEL, NULL);
  if (s->kind == T_JUMP) {
    T_stmList target = (T_stmList) S_look(block_env, s->u.JUMP.jumps->head);
    if (!s->u.JUMP.jumps->tail && target) {
      last->tail = target; /* merge the 2 lists removing JUMP stm */
      trace(target);
    }
    else last->tail->tail = getNext(); /* merge and keep JUMP stm */
  }
  /* we want false label to follow CJUMP */
  else if (s->kind == T_CJUMP) {
    T_stmList tr_list = (T_stmList) S_look(block_env, s->u.CJUMP.tr);
    T_stmList fa_list = (T_stmList) S_look(block_env, s->u.CJUMP.fa);
    if (fa_list) {
      last->tail->tail = fa_list;
      trace(fa_list);
    }
    else if (tr_list) { /* convert so that existing label is a false label */
      last->tail->head = T_Cjump(not_cond(s->u.CJUMP.cond),
				 s->u.CJUMP.fa, s->u.CJUMP.tr);
      last->tail->tail = tr_list;
      trace(tr_list);
    }
    else {
      Temp_label fa = Temp_newlabel();
      last->tail->head = T_Cjump(s->u.CJUMP.cond, s->u.CJUMP.tr, fa);
      last->tail->tail = T_StmList(T_Label(fa), getNext());
    }
  }
  else assert(0);
}

/* get the next block from the list of stmLists, using only those that have
 * not been traced yet */
static T_stmList getNext()
{
  if (!global_block.stmLists)
    return T_StmList(T_Label(global_block.label), NULL);
  else {
    T_stmList s = global_block.stmLists->head;
    if (S_look(block_env, s->head->u.LABEL)) {/* label exists in the table */
      trace(s);
      return s;
    }
    else {
      global_block.stmLists = global_block.stmLists->tail;
      return getNext();
    }
  }
}
         /* traceSchedule : Tree.stm list list * Tree.label -> Tree.stm list
            From a list of basic blocks satisfying properties 1-6,
            along with an "exit" label,
	    produce a list of stms such that:
	      1. and 2. as above;
              7. Every CJUMP(_,t,f) is immediately followed by LABEL f.
            The blocks are reordered to satisfy property 7; also
	    in this reordering as many JUMP(T.NAME(lab)) statements
            as possible are eliminated by falling through into T.LABEL(lab).
         */
T_stmList C_traceSchedule(struct C_block b)
{ C_stmListList sList;
  block_env = S_empty();
  global_block = b;

  for (sList=global_block.stmLists; sList; sList=sList->tail) {
    S_enter(block_env, sList->head->head->u.LABEL, sList->head);
  }

  return getNext();
}

