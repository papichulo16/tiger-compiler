/*
 * flowgraph.h - Function prototypes to represent control flow graphs.
 */

#pragma once

#include "graph.h"

Temp_tempList fg_def(G_node n);
Temp_tempList fg_use(G_node n);
bool fg_is_move(G_node n);
G_graph fg_assem_flowgraph(AS_instrList il);

void fg_print(G_graph fg);

