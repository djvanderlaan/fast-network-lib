#include <iostream>
#include <iomanip>

#include "graph.h"
#include "pajek.h"
#include "normalise_weights.h"
#include "connected_components.h"
#include "generate_poisson.h"
#include "localised_random_walk.h"



int main(int src, char* argv[]) {
  //Graph graph = read_pajek("example_networks/karate.pajek");
  //graph.update_positions();

  std::cout << "generating" << std::endl;
  Graph graph = generate_poisson(100, 1);
  std::cout << "normalising" << std::endl;
  normalise_weights(graph);

  //std::cout << "writing" << std::endl;
  //write_pajek(graph, std::cout);
  //write_pajek(graph, "example_networks/poisson_1E5_100.pajek");
  //std::cout << "finished" << std::endl;

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


  /*
  std::cout << "\nComponents\n";
  auto components = connected_components(graph);
  for (vid_t i = 0; i < graph.nvertices; ++i) {
    std::cout << i << "\t\t" << components[i] << "\n";
  }

  */


  std::vector<double> vertex_weights(graph.nvertices);
  std::vector<double> vertex_values(graph.nvertices);
  for (vid_t i = 0; i < graph.nvertices; ++i) {
    vertex_weights[i] = 1.0;
    vertex_values[i] = 1*((i % 3) == 0);
  }

  auto res = localised_random_walk(graph, vertex_values, vertex_weights, 
    0.85, 400);

  double sum = 0;
  for (vid_t i = 0; i < graph.nvertices; ++i) {
    std::cout << i << " " << vertex_values[i] << ": " << res[i] << "\n";
    sum += res[i];
  }
  std::cout << "mean = " << sum/graph.nvertices << "\n";

  return 0;
}

