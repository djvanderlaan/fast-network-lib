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

std::vector<vid_t> connected_components(const Graph& graph) {
  std::vector<vid_t> components(graph.nvertices);
  // Assign each vertex to it's own component
  for (vid_t i = 0; i < graph.nvertices; ++i) components[i] = i;

  bool components_changed = true;
  do {
    components_changed = false;
    auto pe1 = graph.edges.begin();
    for (vid_t i = 0; i < graph.nvertices; ++i) {
      const degree_t k = graph.degrees[i];
      vid_t comp_id = components[i];
      bool comp_changed = false;
      auto pe2 = pe1;
      for (degree_t j = 0; j < k; ++j, pe1++) {
        const vid_t dcomp_id = components[*pe1];
        if (comp_id != dcomp_id) {
          comp_changed = true;
          comp_id = std::max(comp_id, dcomp_id);
        }
      }
      if (comp_changed) {
        for (degree_t j = 0; j < k; ++j, pe2++) {
          components[*pe2] = comp_id;
        }
        components[i] = comp_id;
        components_changed = true;
      }
    }
  } while (components_changed);
  return components;
}


int main(int src, char* argv[]) {
  Graph graph(7);
  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(1, 0);
  graph.add_edge(4, 3);
  graph.add_edge(4, 5);
  graph.add_edge(5, 4);
  graph.add_edge(0, 1);
  graph.add_edge(6, 7);
  graph.add_edge(7, 8);
  graph.add_edge(9, 10);
  graph.add_edge(10, 11);

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

