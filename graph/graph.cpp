#include "graph.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Graph::Graph()
{
  this->set_number_of_vertices(0);
  this->set_number_of_edges(0);
}

Graph::Graph(int number_of_vertices)
{
  this->set_number_of_vertices(number_of_vertices);
  this->set_number_of_edges(0);
}

void Graph::set_number_of_vertices(int number_of_vertices)
{
  assert(number_of_vertices >= 0 and "Number of vertices must be non-negative");
  assert(number_of_vertices <= NUM_MAX_VERTICES and "Number of vertices must be less than NUM_MAX_VERTICES");
  this->number_of_vertices = number_of_vertices;
  this->adjacency_list.resize(number_of_vertices);
  this->adjacency_matrix.resize(number_of_vertices, std::vector<bool>(number_of_vertices, false));
  this->index2label.resize(number_of_vertices);
  this->label2index.resize(number_of_vertices + 1);
}

void Graph::set_number_of_edges(int number_of_edges)
{
  this->number_of_edges = number_of_edges;
}

void Graph::add_edge(int u, int v, bool directed)
{
  assert(this->number_of_vertices >= u and this->number_of_vertices >= v and
         "Can't add edge if there are not enough vertices");
  if (directed)
  {
    this->adjacency_list.at(u).push_back(v);
  }
  else
  {
    this->adjacency_list.at(u).push_back(v);
    this->adjacency_list.at(v).push_back(u);
  }
}

void Graph::print_to_stdout()
{
  for (int i = 0; i < this->number_of_vertices; i++)
  {
    std::cout << "Vértice: " << this->index2label.at(i) << "; vizinhos: ";
    for (auto &neighbour : this->adjacency_list.at(i))
    {
      std::cout << this->index2label.at(neighbour) << " ";
    }
    std::cout << std::endl;
  }
  for (size_t i = 0; i < this->number_of_vertices; i++)
  {
    for (size_t j = i + 1; j < this->number_of_vertices; j++)
    {
      std::cout << index2label.at(i) << " eh vizinho de " << index2label.at(j) << ": " << (this->is_edge(i, j) ? "Sim" : "Não") << std::endl;
    }
  }
}

int Graph::get_number_of_vertices() const { return this->number_of_vertices; }

bool Graph::is_edge(int u, int v) const
{
  return this->adjacency_matrix.at(u).at(v);
}

void Graph::read_edge_list(const std::string &filename)
{
  std::ifstream input(filename);
  std::string line;
  int number_of_vertices = 0, number_of_edges;
  while (std::getline(input, line))
  {
    if (line.at(0) == 'c')
      continue;
    if (line.at(0) == 'p')
    {
      // parse number of vertices
      char c;
      std::string col;
      std::istringstream iss(line);
      iss >> c >> col >> number_of_vertices >> number_of_edges;
      this->set_number_of_vertices(number_of_vertices);
      continue;
    }
    if (line.at(0) == 'e')
    {
      char c;
      int u, v;
      std::istringstream iss(line);
      iss >> c >> u >> v;
      assert(u <= number_of_vertices and v <= number_of_vertices and "Vertex out of bounds");
      u--;
      v--;
      this->add_edge(u, v);
      continue;
    }
  }
  sort_adjacency_list();
  build_adjacency_matrix();
  input.close();
}

void Graph::sort_adjacency_list()
{
  std::vector<std::pair<std::vector<int>, int>> indexed_list;
  for (size_t i = 0; i < this->adjacency_list.size(); ++i)
  {
    indexed_list.emplace_back(this->adjacency_list[i], i);
  }

  std::sort(indexed_list.begin(), indexed_list.end(),
            [](const std::pair<std::vector<int>, int> &a, const std::pair<std::vector<int>, int> &b)
            {
              return a.first.size() > b.first.size();
            });

  for (size_t i = 0; i < indexed_list.size(); ++i)
  {
    this->index2label[i] = indexed_list[i].second + 1;
    this->label2index[indexed_list[i].second + 1] = i;
  }
  for (auto const [neighbours, label_minus_1] : indexed_list)
  {
    this->adjacency_list[this->label2index[label_minus_1 + 1]].clear();
    for (auto const neighbour : neighbours)
    {
      this->adjacency_list[this->label2index[label_minus_1 + 1]].push_back(this->label2index[neighbour + 1]);
    }
  }
}

void Graph::build_adjacency_matrix()
{
  for (size_t i = 0; i < this->adjacency_list.size(); ++i)
  {
    for (auto &neighbour : this->adjacency_list[i])
    {
      this->adjacency_matrix[i][neighbour] = true;
    }
  }
}

Graph Graph::get_subgraph(std::vector<int> vertices) const
{
  Graph subgraph(vertices.size());
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    for (size_t j = i + 1; j < vertices.size(); ++j)
    {
      if (this->is_edge(vertices[i], vertices[j]))
      {
        subgraph.add_edge(i, j);
      }
    }
  }
  subgraph.sort_adjacency_list();
  subgraph.build_adjacency_matrix();
  return subgraph;
}

void Graph::remove_vertex(int vertex)
{
  for (auto &neighbour : this->adjacency_list[vertex])
  {
    this->adjacency_list[neighbour].erase(std::remove(this->adjacency_list[neighbour].begin(), this->adjacency_list[neighbour].end(), vertex), this->adjacency_list[neighbour].end());
    this->adjacency_matrix[vertex].erase(std::remove(this->adjacency_matrix[vertex].begin(), this->adjacency_matrix[vertex].end(), true), this->adjacency_matrix[vertex].end());
  }
  this->adjacency_list[vertex].clear();
}

int Graph::get_vertex_with_lowest_degree()
{
  int min_degree = this->get_number_of_vertices();
  int vertex = -1;
  for (int i = 0; i < this->get_number_of_vertices(); ++i)
  {
    if (this->adjacency_list[i].size() < min_degree)
    {
      min_degree = this->adjacency_list[i].size();
      vertex = i;
    }
  }
  return vertex;
}

std::vector<std::vector<int>> Graph::get_adjacency_list() const
{
  return this->adjacency_list;
}