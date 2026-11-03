/*
 * flowgraph.h - Function prototypes to represent control flow graphs.
 */

#pragma once

#include "graph.h"

Temp_tempList fg_def(G_node n);
Temp_tempList fg_use(G_node n);
G_graph fg_assem_flowgraph(AS_instrList il);

AS_instrList fg_get_start_instr(G_node n);
AS_instrList fg_get_end_instr(G_node n);

void fg_free(G_graph fg);
void fg_print(G_graph fg);

