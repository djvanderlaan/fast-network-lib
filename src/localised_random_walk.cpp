#include "localised_random_walk.h"
#include <cmath>
#include <iostream>

class localised_random_walk_computer {
  public:
    localised_random_walk_computer(const Graph& graph_, 
      const std::vector<double>& values_, const std::vector<double>& weights_,
      double alpha_, unsigned int nstep_max_, double precision_) : 
        graph(graph_), values(values_), weights(weights_), 
        alpha(alpha_), nstep(nstep_max_), precision(precision_), 
        xsum(graph.nvertices, 0.0), xcur(graph.nvertices), xprev(values_), 
        wsum(graph.nvertices, 0.0), wcur(graph.nvertices), wprev(weights_) {
      // xprev needs to be initialised to x*weight
      for (vid_t i = 0; i < graph.nvertices; ++i) {
        xprev[i] = xprev[i] * wprev[i];
      }
    }

    bool computation_step() {
      auto p = graph.edges.cbegin();
      auto pw = graph.weights.cbegin();
      bool stop = true;
      for (vid_t i = 0; i < graph.nvertices; ++i) {
        // Calculate current end result; at the end we can compare the new
        // result to this an decide that our result is accurate enough
        const double yprev = xsum[i]/wsum[i];
        degree_t k = graph.degrees[i];
        xcur[i] = 0.0;
        wcur[i] = 0.0;
        for (degree_t j = 0; j < k; ++j, ++p, ++pw) {
          std::cout << *p << "\n";
          wcur[i] += (*pw) * wprev[*p];
          xcur[i] += (*pw) * xprev[*p];
        }
        wsum[i] += (1-alpha) * wcur[i];
        xsum[i] += (1-alpha) * xcur[i];
        wcur[i] *= alpha;
        xcur[i] *= alpha;
        // Calculate new end result for current vertex and compare to 
        // previous result; when larger than precision set stop to 
        // false.
        const double ynew = xsum[i]/wsum[i];
        stop &= std::isnan(ynew) || std::abs(ynew-yprev) < precision;
      }
      return stop;
    }

    unsigned int compute() {
      unsigned int step = 0;
      for (; step < nstep; ++step) {
        const bool stop = computation_step();
        if (stop) break;
        std::swap(xcur, xprev);
        std::swap(wcur, wprev);
      }
      return step;
    }

    std::vector<double> result() {
      std::vector<double> y(graph.nvertices);
      for (vid_t i = 0; i < graph.nvertices; ++i) {
        //if (graph.degrees[i] == 0) y[i] = 0;
        //else y[i] = xsum[i]/wsum[i];
        y[i] = xsum[i]/wsum[i];
      }
      return y;
    }

  private:
    const Graph& graph;
    const std::vector<double>& values;
    const std::vector<double>& weights;
    const double alpha;
    const unsigned int nstep;
    const double precision;
    std::vector<double> xsum, xcur, xprev;
    std::vector<double> wsum, wcur, wprev;
};

std::vector<double> localised_random_walk(const Graph& graph, 
    const std::vector<double>& values, std::vector<double>& weights, 
    double alpha, unsigned int nstep_max, double precision) {
  localised_random_walk_computer computer(graph, values, weights, 
    alpha, nstep_max, precision);
  computer.compute();
  return computer.result();
}

