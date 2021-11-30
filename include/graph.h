#ifndef graph_h
#define graph_h

#include <vector>
#include <stdexcept>

typedef unsigned int vid_t;
typedef unsigned int degree_t;
typedef float weight_t;


class Graph {
  public:
    Graph(vid_t nvertices = 0, size_t nedges = 0) : nvertices(nvertices), 
      degrees(nvertices), edges(nedges), weights(nedges) {
    };

    void add_vertex(vid_t id) {
      if (id >= nvertices) {
        nvertices = id+1;
        degrees.resize(nvertices);
      }
    }

    void add_edge(vid_t src, vid_t dst, weight_t weight = 1.0) {
      add_vertex(std::max(src, dst));
      if (src >= last_edge_src) {
        degrees[src] = degrees[src] + 1;
        edges.push_back(dst);
        weights.push_back(weight);
        last_edge_src = src;
      } else {
        throw std::runtime_error("Edges need to be added in order.");
      }
    }


  public:
    vid_t nvertices;
    std::vector<degree_t> degrees;
    std::vector<vid_t> edges;
    std::vector<weight_t> weights;
    vid_t last_edge_src = 0;
};


#endif
