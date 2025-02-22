#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <vector>
#include <algorithm>

using adj_list_t = std::vector<std::vector<int>>;

class Graph
{

private:
  int number_of_vertices;
  int number_of_edges;

  std::vector<int> index2label;
  std::vector<int> label2index;
  std::vector<std::vector<int>> adjacency_list;
  std::vector<std::vector<bool>> adjacency_matrix;
  void add_edge(int u, int v, bool directed = false);
  void set_number_of_vertices(int number_of_vertices);
  void set_number_of_edges(int number_of_edges);
  void sort_adjacency_list();
  void build_adjacency_matrix();

public:
  Graph();
  Graph(int number_of_vertices);
  int get_number_of_vertices() const;
  bool is_edge(int u, int v) const;
  void read_edge_list(const std::string &filename);
  Graph get_subgraph(std::vector<int> vertices) const;
  int get_vertex_with_lowest_degree();
  void remove_vertex(int vertex);
  void print_to_stdout();
  std::vector<std::vector<int>> get_adjacency_list() const;
};

constexpr int NUM_MAX_VERTICES = 4000;

#endif