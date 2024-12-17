#include <string>
#include <vector>

class Graph {

private:
  int number_of_vertices;
  int number_of_edges;

  std::vector<std::vector<int>> adjacency_list;

public:
  Graph();

  Graph(int number_of_vertices);
  void set_number_of_vertices(int number_of_vertices);
  void set_number_of_edges(int number_of_edges);
  void add_edge(int u, int v, bool directed = false);

  void print_to_stdout();
};

// parser graph from dimacs file
Graph parse_from_dimacs(const std::string &filename);
