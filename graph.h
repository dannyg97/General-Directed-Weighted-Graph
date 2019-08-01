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

  // Quick and easy constructor, will move to .tpp file later
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

   
   private:
    // 'N' datatype for value (value is the name of the node, change later?)
    N value;
    // Keep a vector of all outbound edges
    // Add vector of all inbound edges later 
    std::vector<std::shared_ptr<Edge>> outEdges;
  };

  class Edge {
   public:
    // Edge(nodeSource, nodeDestination, nodeWeight);
    Edge(std::shared_ptr<Node> nodeSource, std::shared_ptr<Node> nodeDestination, const E& nodeWeight) {
      source = nodeSource;
      destination = nodeDestination;
      weight = nodeWeight;
    }

    // If the shared pointer for node
    // disappears, these weak pointers  will also disappear (good)
    std::weak_ptr<Node> source;
    std::weak_ptr<Node> destination;
    // Weight kept in template type E (int, double, float), no need to declare template
    E weight;
  };

  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g){
    os << "\n"; // Debugging here
    for (auto const& [key, val] : g.nodegraph) {
      // Should change 'val' because it's too confusing
      // Shouldn't be printing val->value, just testing to see if it works
      // Realistically it should be printing 'key' and another for loop
      // that loops through the entire set of outward bound edges
      // (in sorted order)
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
