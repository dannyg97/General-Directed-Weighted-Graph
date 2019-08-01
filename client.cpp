#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  std::vector<std::string> v{"hello", "how", "are"};
  gdwg::Graph<std::string, int> g{v.begin(), v.end()};
  std::cout << g;


//  g.InsertNode("hello");
//  g.InsertNode("how");
//  g.InsertNode("are");
//  g.InsertNode("you?");
//
//  g.InsertEdge("hello", "how", 5);
//  g.InsertEdge("hello", "are", 8);
//  g.InsertEdge("hello", "are", 2);
//
//  g.InsertEdge("how", "you?", 1);
//  g.InsertEdge("how", "hello", 4);
//
//  g.InsertEdge("are", "you?", 3);
//
//  std::cout << g << '\n';
//
//  gdwg::Graph<std::string, int> g2{g};
//
//  std::cout << g2 << "\n";
//
//  // This is a structured binding.
//  // https://en.cppreference.com/w/cpp/language/structured_binding
//  // It allows you to unpack your tuple.
//  for (const auto& [from, to, weight] : g) {
//    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
//  }
}
