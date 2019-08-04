#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <iostream>
#include <memory>

#include "assignments/dg/graph.tpp"

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // Required to define
  // class const_iterator {};

  // Default constructor
  Graph<N, E>() = default;

  // Constructor for begin, end iterators
  Graph<N, E>(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end){
    for (auto i = begin; i != end; ++i) {
      nodegraph[*i] = std::make_shared<Node>(*i);
    }
  }

  // Constructor for tuple begin, end iterators
  Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator begin, typename std::vector<std::tuple<N, N, E>>::const_iterator end){
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

      // Will make this private later
      // when we do, we'll have to change this
      source->outEdges.push_back(edge);
    }
  }

  // Constructor for iniialiser list of nodes
  Graph<N, E>(typename std::initializer_list<N> list) {
    for (auto i = list.begin(); i != list.end(); ++i) {
      nodegraph[*i] = std::make_shared<Node>(*i);
    }
  }

  // Copy constructor
  Graph<N, E>(Graph& orig) {
    for (const auto& i : orig.nodegraph) {
      InsertNode(i.first);
    }

    // Entering the node
    for (const auto& j : orig.nodegraph) {
      // Entering the node's vector of edges
      for (const auto& k : j.second->outEdges) {
        // Entering the node's edge's src, dst, and weight
        N src = k->getSource();
        N dst = k->getDest();
        E weight = k->getWeight();
        InsertEdge(src, dst, weight);
      }
    }
  }

  // Move constructor, default one is fine
  Graph<N, E>(Graph &&original) = default;

  // Destructor
  ~Graph<N, E>() = default;

  // Copy assignment
  // Uses logic of copy constructor
  // and std::move to make this abomination
  Graph<N, E>& operator=(const Graph<N, E>& orig) {
    Graph<N, E> tmp;
    for (const auto& i : orig.nodegraph) {
      tmp.InsertNode(i.first);
    }
    // Entering the node
    for (const auto& j : orig.nodegraph) {
      // Entering the node's vector of edges
      for (const auto& k : j.second->outEdges) {
        // Entering the node's edge's src, dst, and weight
        N src = k->getSource();
        N dst = k->getDest();
        E weight = k->getWeight();
        tmp.InsertEdge(src, dst, weight);
      }
    }
    *this = std::move(tmp);
    return *this;
  }

  // Move assignment, default one is fine
  Graph<N, E>& operator=(Graph<N, E>&&) = default;

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

    N& getValueRef() {
      return value;
    }

    std::vector<std::shared_ptr<Edge>> outEdges;

    //iterator related help-functions
    bool empty(){
      return outEdges.empty();
    }
    //begin and end functions for iterator over inner edges
    //TODO: SORT BEFORE DOING THIS
    typename std::vector<std::shared_ptr<Edge>>::iterator begin(){
      return outEdges.begin();
    }
    typename std::vector<std::shared_ptr<Edge>>::iterator end(){
      return outEdges.end();
    }

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

    E& getWeightRef() {
      return weight;
    }

    N& getSourceRef() {
      std::shared_ptr<Node> tmp = source.lock();
      return tmp->getValueRef();
    }

    N& getDestRef() {
      std::shared_ptr<Node> tmp = destination.lock();
      return tmp->getValueRef();
    }


   private:
    // If the shared pointer for node
    // disappears, these weak pointers  will also disappear (good)
    std::weak_ptr<Node> source;
    std::weak_ptr<Node> destination;
    // Weight in type E
    E weight;
  };

  bool InsertNode(const N& val) {
    if(nodegraph[val]) return false;
    nodegraph[val] = std::make_shared<Node>(val);
    return true;
  }

  // TODO: Add false
  bool InsertEdge(const N& src, const N& dst, const E& w){
    auto source = nodegraph.find(src)->second;
    auto destination = nodegraph.find(dst)->second;
    auto edge = std::make_shared<Edge>(source, destination, w);
    source->outEdges.push_back(edge);
    return true;
  }

  // TODO: Add false (and make it actually work)
//  bool erase(const N& src, const N& dst, const E& w){
//    auto source = nodegraph.find(src)->second;
//    auto destination = nodegraph.find(dst)->second;
//    auto edge = std::make_shared<Edge>(source, destination, w);
//    std::cout << source->outEdges->getWeight();
//    return true;
//  }

  bool IsNode(const N& val) {
    if (!nodegraph[val]) return false;
    return true;
  }

  bool IsConnected(const N& src, const N& dst) {
    auto source = nodegraph.find(src)->second;
    for (const auto& edge : source->outEdges) {
      if((edge->getSource() == src) && (edge->getDest() == dst))        return true;
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g){
    for (auto const& [key, val] : g.nodegraph) {
      os << key << " (" << std::endl;
      for (std::shared_ptr<Edge> edges: val->outEdges) {
        os << "  " << edges->getDest() << " | " << edges->getWeight() << std::endl;
              os <<"wtf "<< edges->getSource() << std::endl;

      }
      os << ")" << std::endl;
    }
    return os;
  }

  // TODO: Make it actually work
  friend bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&) {
    return true;
  }


  class const_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    //using pointer = std::tuple<N, N, E>*;
    using difference_type = int;
   public:

    //*, ++, --, == and !=
    reference operator*() const {
      return {edge_it_->get()->getSourceRef(),edge_it_->get()->getDestRef(),edge_it_->get()->getWeightRef()};
    }

    const_iterator operator++() {
      ++edge_it_;
      if (edge_it_ == node_it_->second->end()) {
        do {
          ++node_it_;
        } while (node_it_ != sentinel_ && node_it_->second->begin() == node_it_->second->end());
        if (node_it_ != sentinel_) {
          edge_it_ = node_it_->second->begin();
        }
      }
      return *this;
    }
    const_iterator operator++(int) {
      auto copy{*this};
      ++(*this);
      return copy;
    }
  /*TODO: backwards iterating
    const_iterator operator--() {
      node_ = node_->next.get();
      return *this;
    }
    const_iterator operator--(int) {
      auto copy{*this};
      --(*this);
      return copy;
    }
  */
    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      return (lhs.node_it_ == rhs.node_it_ && lhs.edge_it_ == rhs.edge_it_);
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    //pointer operator->() const { return &(operator*()); }
    //Edge* edge_;
    //explicit const_iterator(Edge* edge): edge_{edge} {};
    typename std::map<N,std::shared_ptr<Node>>::iterator node_it_; // out-most iterator
    typename std::map<N,std::shared_ptr<Node>>::iterator sentinel_;
    //typename std::vector<std::vector<N>>::iterator node2_it_; // other_node_container_for_node
    typename std::vector<std::shared_ptr<Edge>>::iterator edge_it_; /*other_edge_container_for_node_pair(edge_  inner iterator*/

    friend class Graph;
    const_iterator(const decltype(node_it_)& node1_it, const decltype(sentinel_)& sentinel, const decltype(edge_it_)& edge_it): node_it_{node1_it}, sentinel_{sentinel}, edge_it_{edge_it} {}
  };

  std::vector<std::shared_ptr<Node>> GetNodeClasses(){
    std::vector<std::shared_ptr<Node>> v;
    for(const auto& it : nodegraph) {
      v.push_back(it.second);
    }
    return v;
  }
/*
  const_iterator begin(){
    auto node_vec = GetNodeClasses();
    std::vector<std::shared_ptr<Edge>> edge_vec= node_vec.at(0).outEdges;
    auto sentinel = node_vec.end();
    return const_iterator{node_vec.begin(),sentinel,edge_vec.begin()};
  }
  const_iterator end(){
    auto node_vec = GetNodeClasses();
    std::vector<std::shared_ptr<Edge>> edge_vec= node_vec.rend()->outEdges;
    auto sentinel = node_vec.end();
    return const_iterator{node_vec.begin(),sentinel,edge_vec.begin()};
  }*/

  const_iterator begin(){
    // What if the first element is empty?
    if (auto first = std::find_if(nodegraph.begin(), nodegraph.end(), [] (const std::pair<N,std::shared_ptr<Node>>& s) { return !((s.second)->empty()); }); first != nodegraph.end()) {
      return const_iterator{first, nodegraph.end(), first->second->begin()};
    }
    return end();
  }

  const_iterator end(){
    return const_iterator{nodegraph.end(), nodegraph.end(), {}};
  }


 private:
  std::map<N, std::shared_ptr<Node>> nodegraph;
};

}  // namespace gdwg

#endif  // ASSIGNMENTS_DG_GRAPH_H_
