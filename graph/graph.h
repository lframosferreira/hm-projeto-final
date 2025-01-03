#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>

using  adj_list_t = std::vector<std::vector<int>>; 

class Graph {

private:
  int number_of_vertices;
  int number_of_edges;

  adj_list_t adjacency_list;

public:
  Graph();

  Graph(int number_of_vertices);
  void set_number_of_vertices(int number_of_vertices);
  void set_number_of_edges(int number_of_edges);
  void add_edge(int u, int v, bool directed = false);
  adj_list_t  get_adjacency_list();
  int get_number_of_vertices();

  void print_to_stdout();
};

// parser graph from dimacs file
Graph parse_from_dimacs(const std::string &filename);
#endif