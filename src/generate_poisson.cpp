#include "generate_poisson.h"

inline int fastrand() {
  static unsigned int g_seed = 123;
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}

inline double runif() {
  return (double)(fastrand()%32768)/(double)32768;
}


Graph generate_poisson(vid_t nvertices, double link_prob) {
  Graph graph(nvertices);
  for (vid_t i = 0; i < nvertices; ++i) {
    for (vid_t j = 0; j < nvertices; ++j) {
      double d = runif();
      if (d < link_prob) graph.add_edge(i, j, 1.0, false);
    }
  }
  return graph;
}

Graph generate_poisson_md(size_t nvertices, double mean_degree) {
  return generate_poisson(nvertices, mean_degree/nvertices);
}

