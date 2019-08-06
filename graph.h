#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <iostream>
#include <memory>
#include <set>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // Default constructor
  Graph<N, E>() = default;

  // Constructor for begin, end iterators
  Graph<N, E>(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end);

  // Constructor for tuple begin, end iterators
  Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator begin, typename std::vector<std::tuple<N, N, E>>::const_iterator end);

  // Constructor for initialiser list of nodes
  Graph<N, E>(typename std::initializer_list<N> list);

  // Copy constructor
  Graph<N, E>(Graph& orig);

  // Move constructor, default one is fine
  Graph<N, E>(Graph &&original) = default;

  // Destructor
  ~Graph<N, E>() = default;

  // Copy assignment
  // Uses logic of copy constructor and std::move to make this abomination
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

  // You HAVE TO declare class Edge prior to creating a vector of type 'Edge'
  class Edge;

  class Node {
   public:
    Node(const N& inputValue) {
      value = inputValue;
    }

    N getValue() const {
      return value;
    }

    void setValue(N newValue) {
      this->value = newValue;
    }

    std::shared_ptr<Edge> getEdge(const N& d, const E& w) const {
      for (const auto& edge : outEdges) {
        if ((edge->getWeight() == w) && (edge->getDest() == d)) {
          return edge;
        }
      }
      // Return null pointer if can't find anything?
      // Or throw exception
      return nullptr;
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

    E getWeight() const {
      return weight;
    }

    // You can only access the contents of a weak_ptr by creating the shared_ptr equivalent (via lock)
    N getSource() const {
      std::shared_ptr<Node> tmp = source.lock();
      return tmp->getValue();
    }

    N getDest() const {
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

    void setDest(std::shared_ptr<Node> newDestination) {
      this->destination = newDestination;
    }

   private:
    // If the shared pointer for node
    // disappears, these weak pointers  will also disappear (good)
    std::weak_ptr<Node> source;
    std::weak_ptr<Node> destination;
    // Weight in type E
    E weight;
  };

  bool InsertNode(const N&);
  bool InsertEdge(const N&, const N&, const E&);
  bool DeleteNode(const N&);
  bool Replace(const N&, const N&);
  void MergeReplace(const N&, const N&);
  void Clear();
  bool erase(const N& src, const N& dst, const E& w);
  bool IsNode(const N&);
  bool IsConnected(const N& src, const N& dst);

  std::vector<N> GetNodes(void) const;
  std::vector<N> GetConnected(const N& src) const;
  std::vector<E> GetWeights(const N& src, const N& dst) const;

  friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g) {
    for (auto const& [key, val] : g.nodegraph) {
      os << key << " (" << std::endl;
      for (std::shared_ptr<Edge> edges: val->outEdges) {
        os << "  " << edges->getDest() << " | " << edges->getWeight() << std::endl;
      }
      os << ")" << std::endl;
    }
  return os;
  }
  friend bool operator==(const gdwg::Graph<N, E>& a, const gdwg::Graph<N, E>& b) {
    // Check nodes
    if (a.GetNodes().size() == b.GetNodes().size()) {
      auto nodeVecA = a.GetNodes();
      auto nodeVecB = b.GetNodes();
      for (unsigned long i = 0; i < nodeVecA.size(); ++i) {
        if(nodeVecA[i] != nodeVecB[i]) return false;

      }
      for (const auto& edgeA : nodeVecA) {
        for(const auto& edgeB: nodeVecB) {
          if(a.GetWeights(edgeA, edgeB) != b.GetWeights(edgeA, edgeB)) return false;
        }
      }
    }
    return true;
  }
  friend bool operator!=(const gdwg::Graph<N, E>& a, const gdwg::Graph<N, E>& b) {
    return (!(a==b));
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
#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
