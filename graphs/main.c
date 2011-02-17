#include "graphs.h"

int main(void)
{
  struct s_list_graph *graph;

  graph = create_list_graph(1, 1);

  graph = add_list_top(graph, 1);
  graph = add_list_top(graph, 2);
  graph = add_list_top(graph, 3);
  graph = add_list_top(graph, 4);
  graph = add_list_top(graph, 5);
  graph = add_list_top(graph, 6);
  
  graph = add_list_arc(graph, 1, 2, 1);
  graph = add_list_arc(graph, 1, 3, -2);

  graph = add_list_arc(graph, 2, 4, -2);
  graph = add_list_arc(graph, 2, 6, 3);

  graph = add_list_arc(graph, 3, 2, 1);
  graph = add_list_arc(graph, 3, 4, 5);
  graph = add_list_arc(graph, 3, 5, 4);

  graph = add_list_arc(graph, 5, 6, -1);

  graph = add_list_arc(graph, 6, 4, -5);
  
  print_list_graph(graph);
  Bellman_list_graph(graph);

  /*graph = create_list_graph(1, 1);
  
  graph = add_list_top(graph, 1);
  graph = add_list_top(graph, 2);
  graph = add_list_top(graph, 3);
  graph = add_list_top(graph, 4);
  graph = add_list_top(graph, 5);
  graph = add_list_top(graph, 6);

  graph = add_list_arc(graph, 1, 2, 10);
  graph = add_list_arc(graph, 1, 3, 3);
  graph = add_list_arc(graph, 1, 5, 6);
  
  graph = add_list_arc(graph, 2, 1, 0);

  graph = add_list_arc(graph, 3, 2, 4);
  graph = add_list_arc(graph, 3, 5, 2);

  graph = add_list_arc(graph, 4, 3, 1);
  graph = add_list_arc(graph, 4, 5, 3);

  graph = add_list_arc(graph, 5, 2, 0);
  graph = add_list_arc(graph, 5, 6, 1);

  graph = add_list_arc(graph, 6, 1, 2);
  
  print_list_graph(graph);
  
  Dijkstra_list_graph(graph);*/

  return (0);
}

