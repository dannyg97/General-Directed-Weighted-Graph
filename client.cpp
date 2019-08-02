#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  // Test basic iterator constructor works
  std::vector<std::string> v{"hello", "how", "are"};
  gdwg::Graph<std::string, int> a{v.begin(), v.end()};
  std::cout << a << std::endl;

  // Test tuple iterator constructor works
  std::string s1{"Hello"};
  std::string s2{"Bye"};
  std::string s3{"Gday"};
  auto e1 = std::make_tuple(s1, s2, 5.4);
  auto e2 = std::make_tuple(s2, s3, 7.6);
  auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
  gdwg::Graph<std::string, double> b{e.begin(), e.end()};
  std::cout << b << std::endl;

  // Test initialiser list constructor works
  gdwg::Graph<char, std::string> c{'a', 'b', 'x', 'y'};
  std::cout << c << std::endl;

  // Test insert node works
  std::cout << "Inserting node" << std::endl;
  c.insertNode('c');
  std::cout << c << std::endl;

  // Test insert edge works
  std::cout << "Inserting edge" << std::endl;
  c.insertEdge('a', 'b', "five");
  std::cout << c << std::endl;
}
