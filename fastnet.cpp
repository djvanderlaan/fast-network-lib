#include <iostream>
#include <iomanip>

#include "graph.h"

void normalise_weights(Graph& graph) {
  auto cpw = graph.weights.cbegin();
  auto pw = graph.weights.begin();
  for (unsigned int i = 0; i < graph.nvertices; ++i) {
    unsigned int k = graph.degrees[i];
    float sum = 0.0;
    for (unsigned int j = 0; j < k; ++j, ++cpw) sum += *cpw;
    float scale = 1.0/sum;
    for (unsigned int j = 0; j < k; ++j, ++pw) *pw = *pw * scale;
  }
}


int main(int src, char* argv[]) {
  Graph graph(5);
  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(1, 0);
  graph.add_edge(4, 2);
  graph.add_edge(4, 3);
  graph.add_edge(4, 5);
  graph.add_edge(5, 2);

  normalise_weights(graph);

  auto p = graph.edges.begin();
  auto pw = graph.weights.begin();
  for (unsigned int i = 0; i < graph.nvertices; ++i) {
    unsigned int k = graph.degrees[i];
    std::cout << i << " k=" << k << "\n";

    float sum = 0.0;
    for (unsigned int j = 0; j < k; ++j, ++p, ++pw) {
      std::cout << "  -> " << *p << " (" << *pw << ")" << "\n";
      sum += *pw;
    }
  }


  return 0;
}

