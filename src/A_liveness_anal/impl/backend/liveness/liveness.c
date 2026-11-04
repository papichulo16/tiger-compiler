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

static void live_run_liveness(G_graph fg) {

}

struct live_graph live_liveness(G_graph flow) {
  
  live_run_liveness(flow);
  return NULL;
}

