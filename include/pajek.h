// Read and write a graph in pajek format
// 
// TODO
//

#ifndef pajek_h
#define pajek_h

#include "graph.h"
#include <string>
#include <ostream>
#include <istream>


void read_pajek(std::istream& stream, Graph& graph);
void read_pajek(const std::string& filename, Graph& graph);

void write_pajek(const Graph& graph, std::ostream& stream);
void write_pajek(const Graph& graph, const std::string& filename);

#endif

