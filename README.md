# fast-network-lib

A simple plain graph library for people that know what they are doing and want
performance. The entire graph class consists of one header file with less than a
hundred lines of code. And then there are a number of functions that operate on
this graph object. Using the library is as simple a plopping the relevant source
files into your project.

The graph object itself is defined in the header `graph.h`. It assumes that one
is working with a directed weighted network and is designed to be
as compact as possible. Compactness of the object is important for both memory
use of large graphs, but also performance. Getting the data from memory to the
CPU is in many cases the limiting factor for performance. Therefore, keeping the
object small is important. The graph object also stores the edges and the edge
weights in separate vectors. This means that an algorithm that does not need the
weights does not need to copy the weights unnecessarily to memory.


