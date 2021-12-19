#ifndef generate_poisson_h
#define generate_poisson_h

#include "graph.h"

Graph generate_poisson(vid_t nvertices, double link_prob);
Graph generate_poisson_md(size_t nvertices, double mean_degree);

#endif
