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

  AS_instrList start;
  AS_instrList end; 
};

static void fg_add_node(G_graph g, AS_instrList s, AS_instrList e) {

  fg_node_t* node = malloc(sizeof(fg_node_t));

  node->start = s;
  node->end = e;

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
}

G_graph fg_assem_flowgraph(AS_instrList il) {

  G_graph graph = G_Graph();

  fg_add_instr_list(graph, il);

  return graph;
}

static void show_info(void* info) {

  fg_node_t* node = (fg_node_t *) info;
  AS_instr instr;
  char* label;

  if (!node->start || !node->start->head) {

    printf("node start not exist??\n");
    return;
  }

  if (node->start->head->kind != I_LABEL) {

    printf("node start not a label??\n");
    return;
  }

  instr = node->start->head;

  label = Temp_labelstring(instr->u.LABEL.label);
  printf("%s, end=\"%s\"\n", label, node->end ? node->end->head->u.OPER.assem : "NULL");
}

void fg_print(G_graph fg) {

  printf("\n===== fg print: ===== \n");
  G_show(stdout, G_nodes(fg), show_info);
}

