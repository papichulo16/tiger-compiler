#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "absyn.h"
#include "assem.h"
#include "frame.h"
#include "graph.h"
#include "errormsg.h"
#include "table.h"
#include "graph.h"
#include "flowgraph.h"
#include "liveness.h"
#include "liveness_set.h"

static int node_list_sz(G_nodeList nl) {

  int c = 0;

  for (;nl; nl = nl->tail)
    c += 1;

  return c;
}

/*
 * the book's algorithm:
 *
 * in[n] = use[n] ∪ (out[n] − def[n])
 * out[n] = ⋃ in[s]
 *        s ∈ succ[n]
 * */
static void liveness_calculation(G_node node) {

  Temp_tempList tmp;
  Temp_tempList tmp2;
  Temp_tempList in;
  Temp_tempList out;

  if (!node)
    return;

  in = fg_in(node);
  out = fg_out(node);

  if (in)
    live_set_free(in);

  tmp = live_set_sub(out, fg_def(node));
  fg_set_in(node, live_set_union(fg_use(node), tmp));

  live_set_free(tmp);
  tmp = NULL;

  for(G_nodeList succ = G_succ(node); succ; succ = succ->tail) {

    tmp2 = tmp;
    tmp = live_set_union(tmp, fg_in(succ->head));

    if (tmp2)
      live_set_free(tmp2);
  }

  if (out) 
    live_set_free(out);

  fg_set_out(node, tmp);
}

static void live_run_liveness(G_nodeList blocks, Temp_tempList* ind, Temp_tempList* outd, int idx) {

  G_node node;

  if (!blocks)
    return;

  // bottom up is more efficient
  live_run_liveness(blocks->tail, ind, outd, idx+1);

  node = blocks->head;

  live_set_free(ind[idx]);
  live_set_free(outd[idx]);
  ind[idx] = live_set_copy(fg_in(node));
  outd[idx] = live_set_copy(fg_out(node));
  liveness_calculation(node);
}

static bool live_run_eq(G_nodeList blocks, Temp_tempList* ind, Temp_tempList* outd, int idx) {

  G_node n;

  if (!blocks)
    return true;

  n = blocks->head;

  return live_set_equal(fg_in(n), ind[idx]) && live_set_equal(fg_out(n), outd[idx]) && live_run_eq(blocks->tail, ind, outd, idx+1); 
}

struct live_graph* live_liveness(G_graph flow) {

  G_nodeList blocks = G_nodes(flow);
  int list_sz = node_list_sz(blocks); 
  Temp_tempList* in_delta = calloc(list_sz, sizeof(Temp_tempList));
  Temp_tempList* out_delta = calloc(list_sz, sizeof(Temp_tempList));
  
  do {

    live_run_liveness(blocks, in_delta, out_delta, 0);

  } while (!live_run_eq(blocks, in_delta, out_delta, 0));

  for (int i = 0; i < list_sz; i++) {

    live_set_free(in_delta[i]);
    live_set_free(out_delta[i]);
  }

  free(in_delta);
  free(out_delta);
  return NULL;
}

