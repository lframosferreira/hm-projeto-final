#include "graph.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Graph::Graph() {
  this->set_number_of_vertices(0);
  this->set_number_of_edges(0);
}

Graph::Graph(int number_of_vertices) {
  this->set_number_of_vertices(number_of_vertices);
  this->set_number_of_edges(0);
}

void Graph::set_number_of_vertices(int number_of_vertices) {
  this->number_of_vertices = number_of_vertices;
  this->adjacency_list.resize(number_of_vertices + 1);
}

void Graph::set_number_of_edges(int number_of_edges) {
  this->number_of_edges = number_of_edges;
}

void Graph::add_edge(int u, int v, bool directed) {
  assert(this->number_of_vertices >= u and this->number_of_vertices >= v and
         "Can't add edge if there are not enough vertices");
  if (directed) {
    this->adjacency_list.at(u).push_back(v);
  } else {
    this->adjacency_list.at(u).push_back(v);
    this->adjacency_list.at(v).push_back(u);
  }
}

adj_list_t  Graph::get_adjacency_list(){
  return this->adjacency_list;
}

int  Graph::get_number_of_vertices(){
  return this->number_of_vertices;
}

void Graph::print_to_stdout() {
  for (int i = 1; i <= this->number_of_vertices; i++) {
    std::cout << i << std::endl;
    for (auto &neighbour : this->adjacency_list.at(i)) {
      std::cout << neighbour << " ";
    }
    std::cout << std::endl;
  }
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
      char c;
      std::string col;
      int number_of_vertices, number_of_edges;
      std::istringstream iss(line);
      iss >> c >> col >> number_of_vertices >> number_of_edges;
      graph.set_number_of_vertices(number_of_vertices);
      continue;
    }
    if (line.at(0) == 'e') {
      char c;
      int u, v;
      std::istringstream iss(line);
      iss >> c >> u >> v;
      graph.add_edge(u, v);
      continue;
    }
  }
  input.close();
  return graph;
}
