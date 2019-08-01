#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <string>
#include <vector>
#include <map>
#include <memory>


namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // Required to define
  // class const_iterator {};

  // Completely forgot how to use default constructors
  Graph<N, E>() = default;

  Graph<N, E>(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end){
    size = end - begin;
    std::cout << size;
    for (auto i = begin; i != end; ++i ) {
      nodegraph[*i] = std::make_shared<Node>(*i);
       std::cout << *i;
    }
  }

  // You HAVE TO declare class Edge prior to creating a vector of type 'Edge', or it'll chuck an error, because C++ reads top->bottom (I think)
  class Edge;

  class Node {
   public:
    Node(const N& inputValue) {
      value = inputValue;
    }
    N value;

    std::vector<std::shared_ptr<Edge>> outEdges;

  };

  class Edge {
   public:
    Edge(std::shared_ptr<Node> nodeSource, std::shared_ptr<Node> nodeDestination, const E& nodeWeight) {
      source = nodeSource;
      destination = nodeDestination;
      weight = nodeWeight;
    }

    std::weak_ptr<Node> source;
    std::weak_ptr<Node> destination;
    E weight;
  };

  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g){
    os << "\n";
    for (auto const& [key, val] : g.nodegraph) {
      std::cout << key << ':' << val->value << std::endl;
    }
    return os;
  }

 private:
  std::map<N, std::shared_ptr<Node>> nodegraph;
  int size;
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
