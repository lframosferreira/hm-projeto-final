#include "graph.h"
#include <cassert>
#include <fstream>
#include <sstream>

Graph::Graph() {
  this->set_number_of_vertices(0);
  this->set_number_of_edges(0);
}

Graph::Graph(int number_of_vertices) {
  this->set_number_of_vertices(number_of_vertices);
  this->set_number_of_edges(0);
  this->adjacency_list.resize(number_of_vertices + 1);
}

void Graph::set_number_of_vertices(int number_of_vertices) {
  assert(this->number_of_vertices != 0 &&
         "Can't set number of vertices in already initialized graph");
  this->number_of_vertices = number_of_vertices;
}

void Graph::set_number_of_edges(int number_of_edges) {
  this->number_of_edges = number_of_edges;
}

Graph parse_from_dimacs(const std::string &filename) {

  Graph graph;
  std::ifstream input(filename);
  std::string line;
  while (std::getline(input, line)) {
    if (line.at(0) == 'c')
      continue;
    if (line.at(0) == 'p') {
      // parse number of vertices
      continue;
    }
    if (line.at(0) == 'e') {
      char c;
      int u, v;
      std::istringstream iss(line);
      iss >> c >> u >> v;
    }
  }
  input.close();
  return graph;
}
