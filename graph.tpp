// Constructor for begin, end iterators
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end){
  for (auto i = begin; i != end; ++i) {
    nodegraph[*i] = std::make_shared<Node>(*i);
  }
}

// Constructor for tuple begin, end iterators
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin, typename std::vector<std::tuple<N, N, E>>::const_iterator end){
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
gdwg::Graph<N,E>::Graph(typename std::initializer_list<N> list) {
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
template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) {
  if (nodegraph.find(val) == nodegraph.end()) return false;
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) {
  auto it = nodegraph.find(val);
  if (it == nodegraph.end()) {
    nodegraph[val] = std::make_shared<Node>(val);
    return true;
  }
  return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w){
  if(!this->IsNode(src) || !this->IsNode(dst)) {
    throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  auto source = nodegraph.find(src)->second;
  auto destination = nodegraph.find(dst)->second;
  auto edge = std::make_shared<Edge>(source, destination, w);

  // Return false because it already exists
  if (source->getEdge(dst, w) == edge) {
    return false;
  }

  source->outEdges.push_back(edge);
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& node) {
  if(!this->IsNode(node)) return false;

  auto del = nodegraph.find(node)->second;
  // To delete all edges within the node
  for (const auto& edge : del->outEdges) {
    erase(edge->getSource(), edge->getDest(), edge->getWeight());
  }
  // To delete all edges containing node as destination
  for (const auto& [key, val] : nodegraph) {
    for (const auto& edge : val->outEdges) {
      if(edge->getDest() == node) {
        erase(edge->getSource(), edge->getDest(), edge->getWeight());
      }
      (void) key;
    }
  }
  nodegraph.erase(node);
  return true;
}


template <typename N, typename E>
bool gdwg::Graph<N,E>::Replace(const N& oldData, const N& newData) {
  if(this->IsNode(newData)) return false;
  if(!this->IsNode(oldData)) {
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
    (void) key;
  }

  nodegraph.erase(oldData);
  return true;
}

template <typename N, typename E>
void gdwg::Graph<N,E>::MergeReplace(const N& oldData, const N& newData) {
  if(!this->IsNode(oldData) || !this->IsNode(newData)) {
    throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
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
      if(edge->getDest() == oldData) {
        // Change the destination of the current edge to be newData instead
        edge->setDest(newNode);
      }
    }

    // Delete duplicates
    std::vector<std::shared_ptr<Edge>> seen;
    for (auto it = std::begin(nodeEdges); it != std::end(nodeEdges); ++it) {
      auto edgeSeen = val->getEdge((*it)->getDest(), (*it)->getWeight());
      if(std::find(seen.begin(), seen.end(), edgeSeen) != seen.end()) {
        this->erase((*it)->getSource(), (*it)->getDest(), (*it)->getWeight());
      }
      seen.push_back(edgeSeen);
    }
    (void) key;
  }
  this->DeleteNode(oldData);
}

template <typename N, typename E>
void gdwg::Graph<N,E>::Clear() {
  nodegraph.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w){
  // Source node
  auto source = nodegraph.find(src)->second;
  auto found = source->getEdge(dst, w);
  if(found == nullptr) return false;
  source->outEdges.erase(std::remove(source->outEdges.begin(), source->outEdges.end(), found), source->outEdges.end());
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) {
  if(!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  auto source = nodegraph.find(src)->second;
  for (const auto& edge : source->outEdges) {
    if((edge->getSource() == src) && (edge->getDest() == dst)) return true;
  }
  return false;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetNodes(void) const {
  std::vector<N> ret;
  for(const auto& it : nodegraph) {
    ret.push_back(it.first);
  }
  return ret;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetConnected(const N& src) const {
  if(nodegraph.find(src)->second) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  std::vector<N> ret;
  auto source = nodegraph.find(src)->second;
  for (const auto& edge : source->outEdges) {
    auto connected = edge->getDest();
    ret.push_back(connected);
  }
  return ret;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N,E>::GetWeights(const N& src, const N& dst) const {
  if(!nodegraph.find(src)->second || !nodegraph.find(dst)->second) {
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  std::vector<E> ret;
  auto source = nodegraph.find(src)->second;
  for (const auto& edge : source->outEdges) {
    if(edge->getDest() == dst) {
      ret.push_back(edge->getWeight());
    }
  }
  return ret;
}
