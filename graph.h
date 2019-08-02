#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <iostream>
#include <memory>


namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // Required to define
  // class const_iterator {};

  // Completely forgot how to use default constructors
  Graph<N, E>() = default;

  // Constructor for begin, end iterators
  Graph<N, E>(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end){
    size = end - begin;
    for (auto i = begin; i != end; ++i) {
      nodegraph[*i] = std::make_shared<Node>(*i);
    }
  }

  // Constructor for tuple begin, end iterators
  Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator begin, typename std::vector<std::tuple<N, N, E>>::const_iterator end){
    size = end - begin;
    for (auto i = begin; i != end; ++i) {
      auto source_val = std::get<0>(*i);
      auto dest_val = std::get<1>(*i);
      auto weight_val = std::get<2>(*i);

      // If either source or dest doesn't exist, create it
      if (!nodegraph[source_val]) {
        nodegraph[source_val] = std::make_shared<Node>(source_val);
      }
      if (!nodegraph[dest_val]) {
        nodegraph[dest_val] = std::make_shared<Node>(dest_val);
      }

      // Now add edges
      // nodegraph.find(source_val)->second POINTS TO THE NODE
      // ->getValue() finds the value of the node

      auto source = nodegraph.find(source_val)->second;
      auto dest = nodegraph.find(dest_val)->second;
      auto edge = std::make_shared<Edge>(source, dest, weight_val);

      // Will outEdges private later
      // when we do, we'll have to change this
      source->outEdges.push_back(edge);
    }
  }

  // Constructor for iniialiser list of nodes
  Graph<N, E>(typename std::initializer_list<N> list) {
    size = list.size();
    for (auto i = list.begin(); i != list.end(); ++i) {
      nodegraph[*i] = std::make_shared<Node>(*i);
    }
  }
  // Copy constructor

  // Move constructor

  // Destructor
  ~Graph<N, E>() = default;

  // You HAVE TO declare class Edge prior to creating a vector of type 'Edge', or it'll chuck an error, because C++ reads top->bottom (I think)
  class Edge;

  class Node {
   public:
    Node(const N& inputValue) {
      value = inputValue;
    }

    N getValue() {
      return value;
    }

    // Keep a vector of all outbound edges
    // I'll make this private later on :")
    std::vector<std::shared_ptr<Edge>> outEdges;
   private:
    N value;
  };

  class Edge {
   public:
    // Edge(nodeSource, nodeDestination, nodeWeight);
    Edge(std::shared_ptr<Node> nodeSource, std::shared_ptr<Node> nodeDestination, const E& nodeWeight) {
      source = nodeSource;
      destination = nodeDestination;
      weight = nodeWeight;
    }

    E getWeight() {
      return weight;
    }

    // You can only access the contents of a weak_ptr by creating the shared_ptr equivalent (via lock)
    N getSource() {
      std::shared_ptr<Node> tmp = source.lock();
      return tmp->getValue();
    }

    N getDest() {
      std::shared_ptr<Node> tmp = destination.lock();
      return tmp->getValue();
    }

   private:
    // If the shared pointer for node
    // disappears, these weak pointers  will also disappear (good)
    std::weak_ptr<Node> source;
    std::weak_ptr<Node> destination;
    // Weight in type E
    E weight;
  };

  bool insertNode(const N& val) {
    if(nodegraph[val]) return false;
    nodegraph[val] = std::make_shared<Node>(val);
    return true;
  }

  bool insertEdge(const N& src, const N& dst, const E& w){
    auto source = nodegraph.find(src)->second;
    auto destination = nodegraph.find(dst)->second;
    auto edge = std::make_shared<Edge>(source, destination, w);
    source->outEdges.push_back(edge);
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g){
    std::vector<std::shared_ptr<Node>> tmp;
    for (auto const& [key, val] : g.nodegraph) {
      os << key << " (" << std::endl;
      for (std::shared_ptr<Edge> edges: val->outEdges) {
        os << "  " << edges->getDest() << " | " << edges->getWeight() << std::endl;
      }
      os << ")" << std::endl;
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
