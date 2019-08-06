#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  // Default constructor
  Graph<N, E>() = default;

  // Constructor for begin, end iterators
  Graph<N, E>(typename std::vector<N>::const_iterator begin,
              typename std::vector<N>::const_iterator end);

  // Constructor for tuple begin, end iterators
  Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
              typename std::vector<std::tuple<N, N, E>>::const_iterator end);

  // Constructor for initialiser list of nodes
  Graph<N, E>(typename std::initializer_list<N> list);

  // Copy constructor
  Graph<N, E>(Graph& orig);

  // Move constructor, default one is fine
  Graph<N, E>(Graph&& original) = default;

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
    explicit Node(const N& inputValue) { value = inputValue; }

    N getValue() const { return value; }

    // for iterator
    N& getValueRef();

    void setValue(N newValue) { this->value = newValue; }

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

    std::vector<std::shared_ptr<Edge>> outEdges;

    // iterator related help-functions
    bool empty() { return outEdges.empty(); }
    // begin and end functions for iterator over inner edges

    static bool edgeSort(std::shared_ptr<Graph::Edge> v1, std::shared_ptr<Graph::Edge> v2) {
      if (v1->getSourceRef() < v2->getSourceRef())
        return true;
      else if (v2->getSourceRef() < v1->getSourceRef())
        return false;
      if (v1->getDestRef() < v2->getDestRef())
        return true;
      else if (v2->getDestRef() < v1->getDestRef())
        return false;
      if (v1->getWeightRef() < v2->getWeightRef())
        return true;
      else if (v2->getWeightRef() < v1->getWeightRef())
        return false;

      return true;
    }
    typename std::vector<std::shared_ptr<Edge>>::iterator begin() {
      sort(outEdges.begin(), outEdges.end(), edgeSort);
      return outEdges.begin();
    }
    typename std::vector<std::shared_ptr<Edge>>::iterator end() {
      sort(outEdges.begin(), outEdges.end(), edgeSort);
      return outEdges.end();
    }
    typename std::vector<std::shared_ptr<Edge>>::reverse_iterator rbegin() {
      sort(outEdges.begin(), outEdges.end(), edgeSort);
      return outEdges.rbegin();
    }
    typename std::vector<std::shared_ptr<Edge>>::reverse_iterator rend() {
      sort(outEdges.begin(), outEdges.end(), edgeSort);
      return outEdges.rend();
    }

   private:
    N value;
  };

  class Edge {
   public:
    // Edge(nodeSource, nodeDestination, nodeWeight);
    Edge(std::shared_ptr<Node> nodeSource,
         std::shared_ptr<Node> nodeDestination,
         const E& nodeWeight) {
      source = nodeSource;
      destination = nodeDestination;
      weight = nodeWeight;
    }

    E getWeight() const { return weight; }

    // You can only access the contents of a weak_ptr by creating the shared_ptr equivalent (via
    // lock)
    N getSource() const;
    N getDest() const;

    // for iterators
    E& getWeightRef();
    N& getSourceRef();
    N& getDestRef();

    void setDest(std::shared_ptr<Node> newDestination) { this->destination = newDestination; }

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
      // auto i = val->outEdges;
      sort(val->outEdges.begin(), val->outEdges.end(), Node::edgeSort);
      for (std::shared_ptr<Edge> edges : val->outEdges) {
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
      for (int i = 0; i < static_cast<int>(nodeVecA.size()); ++i) {
        if (nodeVecA[i] != nodeVecB[i])
          return false;
      }
      for (const auto& edgeA : nodeVecA) {
        for (const auto& edgeB : nodeVecB) {
          if (a.GetWeights(edgeA, edgeB) != b.GetWeights(edgeA, edgeB))
            return false;
        }
      }
    } else {
      return false;
    }
    return true;
  }
  friend bool operator!=(const gdwg::Graph<N, E>& a, const gdwg::Graph<N, E>& b) {
    return (!(a == b));
  }

  class const_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    // using pointer = std::tuple<N, N, E>*;
    using difference_type = int;

   public:
    //*, ++, --, == and !=
    reference operator*() const;

    const_iterator operator++();

    const_iterator operator++(int);

    const_iterator operator--();
    const_iterator operator--(int);

    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      return ((lhs.node_it_ == rhs.node_it_) &&
              (lhs.node_it_ == lhs.sentinel_ || lhs.edge_it_ == rhs.edge_it_));
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    typename std::map<N, std::shared_ptr<Node>>::iterator node_it_;  // out-most iterator
    typename std::map<N, std::shared_ptr<Node>>::iterator sentinel_;
    // end of nodes
    typename std::vector<std::shared_ptr<Edge>>::iterator edge_it_;  // edge_  inner iterator*/

    friend class Graph;
    const_iterator(const decltype(node_it_)& node1_it,
                   const decltype(sentinel_)& sentinel,
                   const decltype(edge_it_)& edge_it)
      : node_it_{node1_it}, sentinel_{sentinel}, edge_it_{edge_it} {}
  };

  class const_reverse_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    // using pointer = std::tuple<N, N, E>*;
    using difference_type = int;

   public:
    //*, ++, --, == and !=
    reference operator*() const;
    const_reverse_iterator operator++();
    const_reverse_iterator operator++(int);
    const_reverse_iterator operator--();
    const_reverse_iterator operator--(int);

    friend bool operator==(const const_reverse_iterator& lhs, const const_reverse_iterator& rhs) {
      return ((lhs.node_it_ == rhs.node_it_) &&
              (lhs.node_it_ == lhs.sentinel_ || lhs.edge_it_ == rhs.edge_it_));
    }

    friend bool operator!=(const const_reverse_iterator& lhs, const const_reverse_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:
    typename std::map<N, std::shared_ptr<Node>>::reverse_iterator node_it_;  // out-most iterator
    typename std::map<N, std::shared_ptr<Node>>::reverse_iterator sentinel_;
    // typename std::vector<std::vector<N>>::iterator node2_it_; // other_node_container_for_node
    typename std::vector<std::shared_ptr<Edge>>::reverse_iterator
        edge_it_; /*other_edge_container_for_node_pair(edge_  inner iterator*/

    friend class Graph;
    const_reverse_iterator(const decltype(node_it_)& node1_it,
                           const decltype(sentinel_)& sentinel,
                           const decltype(edge_it_)& edge_it)
      : node_it_{node1_it}, sentinel_{sentinel}, edge_it_{edge_it} {}
  };

  const_iterator begin();
  const_iterator end();
  const_reverse_iterator rbegin();
  const_reverse_iterator rend();
  const_iterator cbegin();
  const_iterator cend();
  const_reverse_iterator crbegin();
  const_reverse_iterator crend();
  const_iterator erase(const_iterator it);
  const_iterator find(const N& source, const N& dest, const E& weight);

 private:
  std::map<N, std::shared_ptr<Node>> nodegraph;
};

}  // namespace gdwg
#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
