/*
 * flowgraph.h - Function prototypes to represent control flow graphs.
 */

#pragma once

#include "graph.h"

int node_list_sz(G_nodeList nl);
int instr_list_sz_to(AS_instrList nl, AS_instrList end);

Temp_tempList fg_in(G_node n);
Temp_tempList fg_out(G_node n);
void fg_set_in(G_node n, Temp_tempList in);
void fg_set_out(G_node n, Temp_tempList out);

Temp_tempList fg_def(G_node n);
Temp_tempList fg_use(G_node n);
G_graph fg_assem_flowgraph(AS_instrList il);
G_graph fg_assem_instr_flowgraph(AS_instrList il);

AS_instrList fg_get_start_instr(G_node n);
AS_instrList fg_get_end_instr(G_node n);
AS_instrList fg_copy_node_instrs(G_node n);

void fg_free(G_graph fg);
void fg_print(G_graph fg);

