#include <algorithm>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

// Constructor for begin, end iterators
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin,
                         typename std::vector<N>::const_iterator end) {
  for (auto i = begin; i != end; ++i) {
    nodegraph[*i] = std::make_shared<Node>(*i);
  }
}

// Constructor for tuple begin, end iterators
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
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

// Constructor for initialiser list of nodes
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::initializer_list<N> list) {
  for (auto i = list.begin(); i != list.end(); ++i) {
    nodegraph[*i] = std::make_shared<Node>(*i);
  }
}

// Copy constructor
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(Graph& orig) {
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

// Checks if node exists
template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) {
  if (nodegraph.find(val) == nodegraph.end())
    return false;
  return true;
}

// Inserts node of value 'val' into graph
template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) {
  auto it = nodegraph.find(val);
  if (it == nodegraph.end()) {
    nodegraph[val] = std::make_shared<Node>(val);
    return true;
  }
  return false;
}

// Inserts edge of weight 'w' between src and dst nodes
template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  if (!this->IsNode(src) || !this->IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  auto source = nodegraph.find(src)->second;
  auto destination = nodegraph.find(dst)->second;
  auto edge = std::make_shared<Edge>(source, destination, w);

  // Return false because it already exists
  for (const auto& e : source->outEdges) {
    if (e->getDest() == dst && e->getWeight() == w) {
      return false;
    }
  }
  source->outEdges.push_back(edge);
  return true;
}

// Deletes node and all corresponding edges
template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& node) {
  if (nodegraph.find(node) == nodegraph.end())
    return false;

  auto del = nodegraph.find(node)->second;
  // To delete all edges within the node
  for (const auto& edge : del->outEdges) {
    erase(edge->getSource(), edge->getDest(), edge->getWeight());
  }
  // To delete all edges containing node as destination
  for (const auto& [key, val] : nodegraph) {
    for (const auto& edge : val->outEdges) {
      if (edge->getDest() == node) {
        erase(edge->getSource(), edge->getDest(), edge->getWeight());
      }
      (void)key;
    }
  }
  nodegraph.erase(node);
  return true;
}

// Replaces node name with a new name/value
template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
  if (this->IsNode(newData))
    return false;
  if (!this->IsNode(oldData)) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }

  std::shared_ptr<Node> replaced = nodegraph[oldData];
  nodegraph[newData] = replaced;
  replaced->setValue(newData);

  for (const auto& [key, val] : nodegraph) {
    auto nodeEdges = val->outEdges;
    for (const auto& edge : nodeEdges) {
      // If destination is the old node
      if (edge->getDest() == oldData) {
        // Change the destination of the current edge to be newData instead
        edge->setDest(nodegraph.find(newData)->second);
      }
    }
    (void)key;
  }

  nodegraph.erase(oldData);
  return true;
}

// Replaces node with an existing node and transfers it's edges to the new replacement
template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  if (!this->IsNode(oldData) || !this->IsNode(newData)) {
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }

  std::shared_ptr<Node> newNode = nodegraph[newData];
  std::shared_ptr<Node> oldNode = nodegraph[oldData];

  // Erase node oldData from nodegraph completely
  // Only instance of node OldData is in the shared pointer above

  for (const auto& edge : oldNode->outEdges) {
    edge->setDest(newNode);
    newNode->outEdges.push_back(edge);
  }

  // Standard iteration through nodegraph
  for (const auto& [key, val] : nodegraph) {
    auto nodeEdges = val->outEdges;
    for (const auto& edge : nodeEdges) {
      // If destination is the old node
      if (edge->getDest() == oldData) {
        // Change the destination of the current edge to be newData instead
        edge->setDest(newNode);
      }
    }

    // Delete duplicates
    std::vector<std::shared_ptr<Edge>> seen;
    for (auto it = std::begin(nodeEdges); it != std::end(nodeEdges); ++it) {
      auto edgeSeen = val->getEdge((*it)->getDest(), (*it)->getWeight());
      if (std::find(seen.begin(), seen.end(), edgeSeen) != seen.end()) {
        this->erase((*it)->getSource(), (*it)->getDest(), (*it)->getWeight());
      }
      seen.push_back(edgeSeen);
    }
    (void)key;
  }
  this->DeleteNode(oldData);
}

// Completely clears the graph of it's nodes and edges
template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  nodegraph.clear();
}

// Removes an edge from the graph
template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) {
  // Source node
  auto source = nodegraph.find(src)->second;
  auto found = source->getEdge(dst, w);
  if (found == nullptr)
    return false;
  // Uses erase-remove idiom (?)
  source->outEdges.erase(std::remove(source->outEdges.begin(), source->outEdges.end(), found),
                         source->outEdges.end());
  return true;
}

// Finds all nodes connected between src and dest
template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  auto source = nodegraph.find(src)->second;
  for (const auto& edge : source->outEdges) {
    if ((edge->getSource() == src) && (edge->getDest() == dst))
      return true;
  }
  return false;
}

// Creates a vector containing all nodes in the DG
template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes(void) const {
  std::vector<N> ret;
  for (const auto& it : nodegraph) {
    ret.push_back(it.first);
  }
  sort(ret.begin(), ret.end());
  return ret;
}

// Creates a vector containing all edges the node contains
template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
  if (nodegraph.find(src) == nodegraph.end()) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  std::vector<N> ret;
  auto source = nodegraph.find(src)->second;
  for (const auto& edge : source->outEdges) {
    auto connected = edge->getDest();
    ret.push_back(connected);
  }
  sort(ret.begin(), ret.end());
  return ret;
}

// Creates a vector containing all edges/weights that connects src and dst
template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  if (nodegraph.find(src) == nodegraph.end() || nodegraph.find(dst) == nodegraph.end()) {
    throw std::out_of_range(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  std::vector<E> ret;
  auto source = nodegraph.find(src)->second;
  for (const auto& edge : source->outEdges) {
    if (edge->getDest() == dst) {
      ret.push_back(edge->getWeight());
    }
  }
  sort(ret.begin(), ret.end());
  return ret;
}

// Edge function, shows dest
template <typename N, typename E>
N gdwg::Graph<N, E>::Edge::getDest() const {
  std::shared_ptr<Node> tmp = destination.lock();
  return tmp->getValue();
}

// Edge function, shows source
template <typename N, typename E>
N gdwg::Graph<N, E>::Edge::getSource() const {
  std::shared_ptr<Node> tmp = source.lock();
  return tmp->getValue();
}

// Iterator related functions

// in graph
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N& source, const N& dest, const E& weight) {
  auto it = begin();
  for (; it != end(); ++it) {
    auto tuple = *it;
    if (std::get<0>(tuple) == source && std::get<1>(tuple) == dest && std::get<2>(tuple) == weight)
      return it;
  }
  return it;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(const_iterator it) {
  if (it != end()) {
    auto oldSource = std::get<0>(*it);
    auto oldDest = std::get<1>(*it);
    auto oldWeight = std::get<2>(*it);
    ++it;
    if (erase(oldSource, oldDest, oldWeight) == false)
      it = end();
    return it;
  }
  return end();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() {
  return begin();
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() {
  return end();
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crbegin() {
  return rbegin();
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crend() {
  return rend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() {
  // What if the first element is empty?
  if (auto first = std::find_if(
          nodegraph.begin(), nodegraph.end(),
          [](const std::pair<N, std::shared_ptr<Node>>& s) { return !((s.second)->empty()); });
      first != nodegraph.end()) {
    return const_iterator{first, nodegraph.end(), first->second->begin()};
  }
  return end();
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() {
  return const_iterator{nodegraph.end(), nodegraph.end(), {}};
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rbegin() {
  // What if the first element is empty?
  if (auto first = std::find_if(
          nodegraph.rbegin(), nodegraph.rend(),
          [](const std::pair<N, std::shared_ptr<Node>>& s) { return !((s.second)->empty()); });
      first != nodegraph.rend()) {
    return const_reverse_iterator{first, nodegraph.rend(), first->second->rbegin()};
  }
  return rend();
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rend() {
  return const_reverse_iterator{nodegraph.rend(), nodegraph.rend(), {}};
}

// in node
template <typename N, typename E>
N& gdwg::Graph<N, E>::Node::getValueRef() {
  return value;
}

// in edge
template <typename N, typename E>
E& gdwg::Graph<N, E>::Edge::getWeightRef() {
  return weight;
}

template <typename N, typename E>
N& gdwg::Graph<N, E>::Edge::getSourceRef() {
  std::shared_ptr<Node> tmp = source.lock();
  return tmp->getValueRef();
}

template <typename N, typename E>
N& gdwg::Graph<N, E>::Edge::getDestRef() {
  std::shared_ptr<Node> tmp = destination.lock();
  return tmp->getValueRef();
}

// Iterator functions

//*, ++, --, == and !=
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator::reference gdwg::Graph<N, E>::const_iterator::
operator*() const {
  return {edge_it_->get()->getSourceRef(), edge_it_->get()->getDestRef(),
          edge_it_->get()->getWeightRef()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::operator++() {
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

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::operator++(int) {
  auto copy{*this};
  ++(*this);
  return copy;
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::operator--() {
  if (node_it_ == sentinel_) {
    do {
      --node_it_;
      edge_it_ = node_it_->second->begin();
    } while (node_it_->second->begin() == node_it_->second->end());
    while (edge_it_ != node_it_->second->end())
      ++edge_it_;
    --edge_it_;  // 1 before the end
    return *this;
  }
  if (edge_it_ == node_it_->second->begin()) {
    do {
      --node_it_;
      edge_it_ = node_it_->second->begin();
    } while (node_it_->second->begin() == node_it_->second->end());
    while (edge_it_ != node_it_->second->end())
      ++edge_it_;
    --edge_it_;  // 1 before the end
    return *this;
  }
  // case for internally going back
  --edge_it_;
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::const_iterator::operator--(int) {
  auto copy{*this};
  --(*this);
  return copy;
}

// reverse iterator functions

//*, ++, --, == and !=
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator::reference
    gdwg::Graph<N, E>::const_reverse_iterator::operator*() const {
  return {edge_it_->get()->getSourceRef(), edge_it_->get()->getDestRef(),
          edge_it_->get()->getWeightRef()};
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::const_reverse_iterator::
operator++() {
  ++edge_it_;
  if (edge_it_ == node_it_->second->rend()) {
    do {
      ++node_it_;
    } while (node_it_ != sentinel_ && node_it_->second->rbegin() == node_it_->second->rend());
    if (node_it_ != sentinel_) {
      edge_it_ = node_it_->second->rbegin();
    }
  }
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::const_reverse_iterator::
operator++(int) {
  auto copy{*this};
  ++(*this);
  return copy;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::const_reverse_iterator::
operator--() {
  if (node_it_ == sentinel_) {
    do {
      --node_it_;
      edge_it_ = node_it_->second->rbegin();
    } while (node_it_->second->rbegin() == node_it_->second->rend());
    while (edge_it_ != node_it_->second->rend())
      ++edge_it_;
    --edge_it_;  // 1 before the end
    return *this;
  }
  if (edge_it_ == node_it_->second->rbegin()) {
    do {
      --node_it_;
      edge_it_ = node_it_->second->rbegin();
    } while (node_it_->second->rbegin() == node_it_->second->rend());
    while (edge_it_ != node_it_->second->rend())
      ++edge_it_;
    --edge_it_;  // 1 before the end
    return *this;
  }
  // case for internally going back
  --edge_it_;
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::const_reverse_iterator::
operator--(int) {
  auto copy{*this};
  --(*this);
  return copy;
}
