#include "graph.h"
#include <cstdlib>
#include <string>

int main(int argc, char **argv) {

  std::string filename = argv[1];

  Graph g = parse_from_dimacs(filename);
  g.print_to_stdout();
  exit(EXIT_SUCCESS);
}
