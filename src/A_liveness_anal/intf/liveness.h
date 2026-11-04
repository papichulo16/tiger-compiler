
typedef struct live_move_list live_move_list_t;

struct live_move_list {
  G_node src, dst;
  live_move_list_t* tail;
};

live_move_list_t* live_move_list(G_node src, G_node dst, live_move_list_t* tail);

struct live_graph { 
  G_graph graph; 
  live_move_list_t* moves; 
};

Temp_temp live_gtemp(G_node n);
struct live_graph* live_liveness(G_graph flow);

