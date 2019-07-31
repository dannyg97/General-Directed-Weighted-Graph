#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <string>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // Required to define
  class const_iterator {};

 private:
  std::string name;
  int value;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
