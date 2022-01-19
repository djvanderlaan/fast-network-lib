#ifndef localised_random_walk_h
#define localised_random_walk_h

#include "graph.h"
#include <vector>

std::vector<double> localised_random_walk(const Graph& graph, 
    const std::vector<double>& values, std::vector<double>& weights, 
    double alpha = 0.85, unsigned int nstep_max = 1000, double precision = 1E-5,
    unsigned int* nstep = nullptr);

#endif
