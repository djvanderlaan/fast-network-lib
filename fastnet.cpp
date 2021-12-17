#include <iostream>
#include <iomanip>

#include "graph.h"
#include "pajek.h"
#include "normalise_weights.h"
#include "connected_components.h"



int main(int src, char* argv[]) {
  Graph graph;
  read_pajek("example_networks/karate.pajek", graph);
  //graph.update_positions();

  normalise_weights(graph);

  auto p = graph.edges.begin();
  auto pw = graph.weights.begin();
  for (unsigned int i = 0; i < graph.nvertices; ++i) {
    unsigned int k = graph.degrees[i];
    std::cout << i << " k=" << k << " pos=" << graph.positions[i] << "\n";

    float sum = 0.0;
    for (unsigned int j = 0; j < k; ++j, ++p, ++pw) {
      std::cout << "  -> " << *p << " (" << *pw << ")" << "\n";
      sum += *pw;
    }
  }


  std::cout << "\nComponents\n";
  auto components = connected_components(graph);
  for (vid_t i = 0; i < graph.nvertices; ++i) {
    std::cout << i << "\t\t" << components[i] << "\n";
  }


  return 0;
}

