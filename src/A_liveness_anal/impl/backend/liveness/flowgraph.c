#include <stdio.h>

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

typedef struct fg_node fg_node_t;

struct fg_node {

  Temp_label label;

  AS_instrList start;
  AS_instrList end; 

  Temp_tempList def;
  Temp_tempList use;
};

static G_node fg_find_node(G_graph g, Temp_label l) {

  G_nodeList nl = G_nodes(g);
  fg_node_t* info;

  for (;nl; nl = nl->tail) {

    info = G_nodeInfo(nl->head);

    if (info->label == l)
      return nl->head;
  }

  return NULL;
}

static bool fg_temp_in_list(Temp_temp t, Temp_tempList tl) {

  bool found = false;

  for (; tl && !found; tl = tl->tail)
    found = tl->head == t;

  return found;
}

static Temp_tempList fg_add_temp(Temp_temp t, Temp_tempList tl) {

  if (!fg_temp_in_list(t, tl))
    tl = Temp_TempList(t, tl);

  return tl;
}

static Temp_tempList fg_init_def(G_graph g, AS_instrList s, AS_instrList e) {

  if (s && s->head && s->head->kind == I_MOVE &&
      s->head->u.MOVE.dst)
    return fg_add_temp(s->head->u.MOVE.dst->head,
        fg_init_def(g, s->tail, e));

  if (s)
    return fg_init_def(g, s->tail, e);

  return NULL;
}

static Temp_tempList fg_init_use(G_graph g, AS_instrList s, AS_instrList e) {

  if (s && s->head && s->head->kind == I_MOVE &&
      s->head->u.MOVE.src)
    return fg_add_temp(s->head->u.MOVE.src->head,
        fg_init_use(g, s->tail, e));

  if (s)
    return fg_init_use(g, s->tail, e);

  return NULL;
}

static void fg_add_node(G_graph g, AS_instrList s, AS_instrList e) {

  fg_node_t* node = malloc(sizeof(fg_node_t));

  node->label = s->head->u.LABEL.label;
  node->start = s;
  node->end = e;
  node->def = fg_init_def(g, s, e);
  node->use = fg_init_use(g, s, e);

  G_Node(g, node);
}

static void fg_add_instr_list(G_graph g, AS_instrList il) {

  AS_instrList start = NULL;
  AS_instrList prev = NULL;

  for (AS_instrList cur = il; cur; cur = cur->tail) {

    if (cur->head->kind == I_LABEL) {

      if (start) {

        fg_add_node(g, start, prev);
        start = cur;
        prev = NULL;

      } else start = cur;
    }

    else if (cur->head->kind == I_OPER && cur->head->u.OPER.jumps) {

        fg_add_node(g, start, cur);
        start = NULL;
    }

    prev = cur;
  }

  if (start)
    fg_add_node(g, start, prev);
}

static void fg_node_attach_endpoints(G_graph g, G_node n) {

  fg_node_t* info = G_nodeInfo(n);
  AS_instr end;
  G_node to;

  if (!info->end || !info->end->head)
    return;

  end = info->end->head;

  if ((end->kind != I_OPER || !end->u.OPER.jumps)) {

    if (info->end->tail && 
        (to = fg_find_node(g, info->end->tail->head->u.LABEL.label),
        to)) 
      G_addEdge(n, to);

    return;
  }

  for (Temp_labelList l = info->end->head->u.OPER.jumps->labels;
      l; l = l->tail) {

    if (to = fg_find_node(g, l->head), to)
      G_addEdge(n, to);
  }
}

static void fg_attach_endpoints(G_graph g) {

  G_nodeList nl = G_nodes(g);

  for(;nl; nl = nl->tail) {

    fg_node_attach_endpoints(g, nl->head);
  } 
}

Temp_tempList fg_def(G_node n) {

  fg_node_t* info = G_nodeInfo(n);
  return info->def;
}

Temp_tempList fg_use(G_node n) {

  fg_node_t* info = G_nodeInfo(n);
  return info->use;
}

AS_instrList fg_get_start_instr(G_node n) {

  fg_node_t* info = G_nodeInfo(n);
  return info->start;
}

AS_instrList fg_get_end_instr(G_node n) {

  fg_node_t* info = G_nodeInfo(n);
  return info->end;
}

static void fg_free_temp_list(Temp_tempList tl) {

  Temp_tempList next;

  for (; tl; tl = next) {

    next = tl->tail;
    free(tl);
  }
}

static void fg_free_node(fg_node_t* node) {

  fg_free_temp_list(node->def);
  fg_free_temp_list(node->use);
  free(node);
}

void fg_free(G_graph fg) {

  for (G_nodeList nl = G_nodes(fg); nl; nl = nl->tail)
    fg_free_node(G_nodeInfo(nl->head));

  G_free(fg);
}

G_graph fg_assem_flowgraph(AS_instrList il) {

  G_graph graph = G_Graph();

  fg_add_instr_list(graph, il);
  fg_attach_endpoints(graph);

  return graph;
}

static void show_info(void* info) {

  fg_node_t* node = (fg_node_t *) info;
  char* label;

  if (!node->start || !node->start->head) {

    printf("node start not exist??\n");
    return;
  }

  if (node->start->head->kind != I_LABEL) {

    printf("node start not a label??\n");
    return;
  }

  label = Temp_labelstring(node->label);
  printf("%s, end=\"%s\"\n", label, node->end ? node->end->head->u.OPER.assem : "NULL");
}

void fg_print(G_graph fg) {

  printf("\n===== fg print: ===== \n");
  G_show(stdout, G_nodes(fg), show_info, true);
}

