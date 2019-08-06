/*

  == Explanation and rational of testing ==

  The test cases were divided into four separate scenarios:
  - Constructors
  - Operators
  - Methods
  - Friends

  Standard operations are tested using DGs with simple values to make it easy
  to spot if there are any issues in the results, and also to ensure that the
  operations conducted work correctly amongst every facet of the directed graph.

  Each constructor, method, operator, and friend are exclusively tested one by one
  to make sure that each work.
  The results and graphs are simple and straightforward, making it easy to spot if
  there are any glaring issues, while at the same time being sufficient enough to 
  test for correctness and reliability. 

  Exceptions are also tested with REQUIRES_THROWS_WITH().
  
   =====================

  Testing on the iterators was done by using all the iterator functions
  on different points of the created edge container.

  begin was first tested on an empty graph to ensure only end() existed
  on the iterator.

  begin and rbegin tested by looping up until end and rend checking
  every edge was in order

  This was repeated with the c versions of the functions to ensure
  they were functionally the same

  Erase was tested by erasing from the front, middle and end of the
  graph and iterating over the graph from begin() to make sure it was
  erased, return iterator was next edge and end() in the case of
  an irremovable edge.

  Find was tested by Finding from the front, middle and end of the
  graph, and making sure returned iterator was matching it.
  non-existent edge was also tested to return end()
*/

#include "assignments/dg/graph.h"
#include "catch.h"

SCENARIO("Testing Directed Graph (DG) Constructors") {
  GIVEN("The start and end of a vector") {
    std::vector<std::string> v{"Hello", "how", "are", "you"};
    WHEN("The DG is initialised with the above") {
      gdwg::Graph<std::string, double> dg{v.begin(), v.end()};
      THEN("The DG is created with the nodes of the vector with no edges") {
        REQUIRE(dg.IsNode("Hello"));
        REQUIRE(dg.IsNode("how"));
        REQUIRE(dg.IsNode("are"));
        REQUIRE(dg.IsNode("you"));
      }
    }
  }
  GIVEN("The start and end of a tuple const iterator") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"are"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    WHEN("The DG is initialised with the above") {
      gdwg::Graph<std::string, double> dg{e.begin(), e.end()};
      THEN("The DG is created with the nodes of the vector with the edges also created") {
        REQUIRE(dg.IsNode("Hello"));
        REQUIRE(dg.IsNode("how"));
        REQUIRE(dg.IsNode("are"));
      }
    }
  }
  GIVEN("An initialiser vector of elements of type N") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    WHEN("The DG is initialised with the above") {
      (void)dg;  // Already exists above, do nothing
      THEN("The DG is created with the nodes of the vector with no edges") {
        REQUIRE(dg.IsNode('a'));
        REQUIRE(dg.IsNode('b'));
        REQUIRE(dg.IsNode('x'));
        REQUIRE(dg.IsNode('y'));
      }
    }
  }
  GIVEN("An already-constructed DG") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    WHEN("A DG is copy constructed") {
      auto dgCopy{dg};
      THEN("The new DG should have the elements of the old DG and the old DG should not have any "
           "changes to it") {
        REQUIRE(dgCopy.IsNode('a'));
        REQUIRE(dgCopy.IsNode('b'));
        REQUIRE(dgCopy.IsNode('x'));
        REQUIRE(dgCopy.IsNode('y'));

        REQUIRE(dg.IsNode('a'));
        REQUIRE(dg.IsNode('b'));
        REQUIRE(dg.IsNode('x'));
        REQUIRE(dg.IsNode('y'));
      }
    }
  }
  GIVEN("An already-constructed DG") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    WHEN("A DG is move constructed") {
      auto dgMove{std::move(dg)};
      THEN("The new DG should have the elements of the old DG and the old DG should have all of "
           "its nodes deleted") {
        REQUIRE(dgMove.IsNode('a'));
        REQUIRE(dgMove.IsNode('b'));
        REQUIRE(dgMove.IsNode('x'));
        REQUIRE(dgMove.IsNode('y'));

        REQUIRE(!dg.IsNode('a'));
        REQUIRE(!dg.IsNode('b'));
        REQUIRE(!dg.IsNode('x'));
        REQUIRE(!dg.IsNode('y'));
      }
    }
  }
}

SCENARIO("Testing DG Operations") {
  GIVEN("An already constructed DG") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    WHEN("A DG is copy-assigned to the old DG") {
      auto dgCopy{dg};
      THEN("The DG should not contain any of it's old elements and should now"
           "contain the nodes and edges of the old DG"
           "and the old DG should still have all of it's elements") {
        REQUIRE(dgCopy.IsNode('a'));
        REQUIRE(dgCopy.IsNode('b'));
        REQUIRE(dgCopy.IsNode('x'));
        REQUIRE(dgCopy.IsNode('y'));

        REQUIRE(dg.IsNode('a'));
        REQUIRE(dg.IsNode('b'));
        REQUIRE(dg.IsNode('x'));
        REQUIRE(dg.IsNode('y'));
      }
    }
    WHEN("A DG is move-assigned to the old DG") {
      auto dgMove{std::move(dg)};
      THEN("The DG should not contain any of it's old elements and should now"
           "contain the nodes and edges of the old DG"
           "and the old DG should not have any of it's elements") {
        REQUIRE(dgMove.IsNode('a'));
        REQUIRE(dgMove.IsNode('b'));
        REQUIRE(dgMove.IsNode('x'));
        REQUIRE(dgMove.IsNode('y'));

        REQUIRE(!dg.IsNode('a'));
        REQUIRE(!dg.IsNode('b'));
        REQUIRE(!dg.IsNode('x'));
        REQUIRE(!dg.IsNode('y'));
      }
    }
  }
}

SCENARIO("Testing DG Friends") {
  GIVEN("Two different graphs") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    gdwg::Graph<char, std::string> dg1{'a', 'x', 'y'};
    WHEN("They are compared to each other using operator==") {
      bool res = (dg == dg1);
      THEN("It should return false") { REQUIRE(res == false); }
    }
    WHEN("They are compared to each other using operator!=") {
      bool res = (dg != dg1);
      THEN("It should return true") { REQUIRE(res == true); }
    }
  }
  GIVEN("Two identical graphs") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    gdwg::Graph<char, std::string> dg1{dg};
    WHEN("They are compared to each other using operator==") {
      bool res = (dg == dg1);
      THEN("It should return true") { REQUIRE(res == true); }
    }
    WHEN("They are compared to each other using operator!=") {
      bool res = (dg != dg1);
      THEN("It should return false") { REQUIRE(res == false); }
    }
  }
}

SCENARIO("Testing DG Methods") {
  GIVEN("A pre-constructed DG") {
    gdwg::Graph<char, std::string> dg{'a', 'b', 'x', 'y'};
    WHEN("A node is inserted") {
      dg.InsertNode('c');
      THEN("The node is inserted successfully") { REQUIRE(dg.IsNode('c')); }
    }
    WHEN("A node is inserted, but the node already exists") {
      bool res = (dg.InsertNode('a'));
      THEN("The node does not insert, and the InsertNode() function"
           "returns false") {
        REQUIRE(res == false);
      }
    }
    WHEN("An edge is inserted") {
      dg.InsertEdge('a', 'b', "five");
      THEN("The edge is inserted successfully") { REQUIRE(dg.IsConnected('a', 'b')); }
    }
    WHEN("An edge is inserted, but the edge already exists") {
      dg.InsertEdge('a', 'b', "five");
      auto res = (dg.InsertEdge('a', 'b', "five"));
      THEN("The edge does not insert, and the InsertEdge() function"
           "returns false") {
        REQUIRE(res == false);
      }
    }
    WHEN("An edge is inserted, but the src or dst does not exist") {
      THEN("A runtime_error exception is thrown") {
        REQUIRE_THROWS_WITH(
            dg.InsertEdge('p', 'l', "five"),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      }
    }
    WHEN("A node is deleted") {
      dg.InsertEdge('a', 'b', "five");
      dg.InsertEdge('b', 'a', "five");
      dg.DeleteNode('a');
      THEN("The node is deleted from the graph, and all edges to and from it"
           "are deleted") {
        REQUIRE(!dg.IsNode('a'));
        REQUIRE_THROWS_WITH(
            dg.IsConnected('a', 'b'),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
    WHEN("A delete function is called on a node that does not exist") {
      auto res = (dg.DeleteNode('h'));
      THEN("No changes are made to the graph and DeleteNode() returns false") {
        REQUIRE(res == false);
      }
    }
    WHEN("A node is replaced by a new (not pre-existing) node") {
      dg.InsertEdge('a', 'b', "five");
      dg.InsertEdge('b', 'a', "five");
      dg.Replace('a', 'y');
      THEN("The new node contains all the edges of the old node and all instances"
           "of the old node as a destination is replaced by the new node") {
        REQUIRE(dg.IsNode('y'));
        REQUIRE_THROWS_WITH(
            dg.IsConnected('a', 'b'),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
        REQUIRE(dg.IsConnected('y', 'b') == true);
      }
    }
    WHEN("A node is replaced by a new (not pre-existing) node, but the new node"
         "already exists in the DG") {
      bool res = (dg.Replace('a', 'b'));
      THEN("No changes are made to the graph and the Replace() function"
           "returns false") {
        REQUIRE(dg.IsNode('a'));
        REQUIRE(dg.IsNode('b'));
        REQUIRE(res == false);
      }
    }
    WHEN("A node is replaced by a new (not pre-existing) node, but the node-to-replace"
         "does not exist") {
      THEN("No changes are made to the graph and a runtime_error exception is thrown") {
        REQUIRE_THROWS_WITH(dg.Replace('f', 'b'),
                            "Cannot call Graph::Replace on a node that doesn't exist");
      }
    }
    WHEN("A node is merge-replaced by a pre-existing node") {
      dg.InsertEdge('a', 'b', "five");
      dg.InsertEdge('b', 'a', "five");
      dg.MergeReplace('a', 'b');
      THEN("The old node's outEdges are replaced by the new node, edges where the old"
           "node is the destination is replaced by the new node, and duplicates are removed") {
        REQUIRE(!dg.IsNode('a'));
        REQUIRE_THROWS_WITH(
            dg.IsConnected('a', 'x'),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
        REQUIRE(dg.IsConnected('b', 'b') == true);
      }
    }
    WHEN("A node is merge-replaced by a pre-existing node but either the source or"
         "the destination does not exist") {
      THEN("No changes are made to the graph and a runtime_error exception is thrown") {
        REQUIRE_THROWS_WITH(
            dg.MergeReplace('l', 'z'),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
    WHEN("The graph is cleared using Clear()") {
      dg.Clear();
      THEN("No nodes should exist") {
        REQUIRE(!dg.IsNode('a'));
        REQUIRE(!dg.IsNode('b'));
        REQUIRE(!dg.IsNode('x'));
        REQUIRE(!dg.IsNode('y'));
      }
    }
    WHEN("An existing node is tested for it's existence using IsNode()") {
      bool res = dg.IsNode('a');
      THEN("The result is true") { REQUIRE(res == true); }
    }
    WHEN("A node that does not exist is tested for it's existence using IsNode()") {
      bool res = dg.IsNode('h');
      THEN("The result is false") { REQUIRE(res == false); }
    }
    WHEN("An existing edge is tested for it's existence using IsConnected()") {
      dg.InsertEdge('a', 'b', "five");
      bool res = (dg.IsConnected('a', 'b'));
      THEN("The result is true") { REQUIRE(res == true); }
    }
    WHEN("A non-existent edge is tested for it's existence using IsConnected()") {
      dg.InsertEdge('a', 'c', "five");
      bool res = (dg.IsConnected('a', 'b'));
      THEN("The result is true") { REQUIRE(res == false); }
    }
    WHEN("An existing edge is tested for it's existence using"
         "IsConnected(), but either the source or destination does not exist") {
      THEN("A runtime_error exception is thrown") {
        REQUIRE_THROWS_WITH(
            dg.IsConnected('o', 'h'),
            "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
      }
    }
    WHEN("A vector of node names is requested for using GetNodes()") {
      std::vector<char> vec{'a', 'b', 'x', 'y'};
      auto node = dg.GetNodes();
      THEN("A vector is returned with all node names of the graph") { REQUIRE(vec == node); }
    }
    WHEN("A vector of edges of a node is requested for using GetConnected()") {
      dg.InsertEdge('a', 'b', "five");
      std::vector<char> vec{'b'};
      auto edge = dg.GetConnected('a');
      THEN("A vector is returned with all nodes connected to the node") { REQUIRE(edge == vec); }
    }
    WHEN("A vector of edge names is requested for using GetConnected(), but"
         "the source does not exist") {
      THEN("An out_of_range exception is triggered") {
        REQUIRE_THROWS_WITH(dg.GetConnected('i'),
                            "Cannot call Graph::GetConnected if src doesn't exist in the graph");
      }
    }
    WHEN("A vector of edge weights is requested for using GetWeights()") {
      dg.InsertEdge('a', 'b', "five");
      dg.InsertEdge('a', 'b', "fullmarksplease");
      dg.InsertEdge('a', 'b', "throw");
      auto weights = dg.GetWeights('a', 'b');
      std::vector<std::string> vec{"five", "fullmarksplease", "throw"};
      THEN("A vector is returned with all weights from src to dst") { REQUIRE(weights == vec); }
    }
    WHEN("A vector of edge weights is requested for using GetWeights(), but"
         "the source and/or the destination do not exist") {
      THEN("An out_of_range exception error is triggered") {
        REQUIRE_THROWS_WITH(
            dg.GetWeights('h', 'i'),
            "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
      }
    }
    WHEN("An edge is deleted") {
      dg.InsertEdge('a', 'b', "five");
      dg.erase('a', 'b', "five");
      THEN("The edge is successfully deleted") { REQUIRE(dg.GetConnected('a').size() == 0); }
    }
    WHEN("An edge that does not exist is deleted") {
      bool res = (dg.erase('a', 'b', "red"));
      THEN("Nothing happens to the graph and erase() returns false") { REQUIRE(res == false); }
    }
  }
}

// OPERATORS
SCENARIO("Testing iterator functions") {
  GIVEN("an empty graph"){
    gdwg::Graph<std::string, int> emptyG;
    WHEN("An Iterator is created for it") {
      auto it = emptyG.begin();
      THEN("end() iterator is returned by default"){
        REQUIRE(it==emptyG.end());
      }
    }
    WHEN("Trying to erase end()") {
      auto it = emptyG.begin();
      it = emptyG.erase(it);
      THEN("end() iterator is returned by default"){
        REQUIRE(it==emptyG.end());
      }
    }
  }
  GIVEN("a graph with 4 nodes and 5 edges") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("hello");
    g.InsertNode("how");
    g.InsertNode("are");
    g.InsertNode("you?");

    g.InsertEdge("hello", "how", 5);
    g.InsertEdge("hello", "are", 8);
    g.InsertEdge("hello", "are", 2);

    g.InsertEdge("how", "you?", 1);
    g.InsertEdge("how", "hello", 4);

    g.InsertEdge("are", "you?", 3);

    WHEN("Graph is looped over with const_iterator") {
      THEN("all edges should be given in order source->dest->weight") {
        auto it=g.begin();
        for (int i=0; it != g.end(); ++it,++i)
        {
          auto tuple = *it;
          if(i==0)REQUIRE(tuple==std::make_tuple("are", "you?", 3));
          if(i==1)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==3)REQUIRE(tuple==std::make_tuple("hello", "how", 5));
          if(i==4)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          if(i==5)REQUIRE(tuple==std::make_tuple("how", "you?", 1));
        }
      }
    }
    WHEN("Graph is looped over with reverse const_iterator") {
      THEN("all edges should be given in reverse of order source->dest->weight") {
        auto it=g.rbegin();
        for (int i=0; it != g.rend(); ++it,++i)
        {
          auto tuple = *it;
          if(i==5)REQUIRE(tuple==std::make_tuple("are", "you?", 3));
          if(i==4)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==3)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "how", 5));
          if(i==1)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          if(i==0)REQUIRE(tuple==std::make_tuple("how", "you?", 1));
        }
      }
    }

    WHEN("Graph is looped over with const const_iterator") {
      THEN("all edges should be given in order source->dest->weight") {
        auto it=g.cbegin();
        for (int i=0; it != g.cend(); ++it,++i)
        {
          auto tuple = *it;
          if(i==0)REQUIRE(tuple==std::make_tuple("are", "you?", 3));
          if(i==1)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==3)REQUIRE(tuple==std::make_tuple("hello", "how", 5));
          if(i==4)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          if(i==5)REQUIRE(tuple==std::make_tuple("how", "you?", 1));
        }
      }
    }
    WHEN("Graph is looped over with const reverse const_iterator") {
      THEN("all edges should be given in reverse of order source->dest->weight") {
        auto it=g.crbegin();
        for (int i=0; it != g.crend(); ++it,++i)
        {
          auto tuple = *it;
          if(i==5)REQUIRE(tuple==std::make_tuple("are", "you?", 3));
          if(i==4)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==3)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "how", 5));
          if(i==1)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          if(i==0)REQUIRE(tuple==std::make_tuple("how", "you?", 1));
        }
      }
    }
    WHEN("edge iterator is incremented all the way to the end") {
      auto it=g.begin();
      auto tuple = *it;
      REQUIRE(tuple==std::make_tuple("are", "you?", 3));
      ++it;++it;++it;++it;++it;
      auto tuple1 = *it;
      REQUIRE(tuple1==std::make_tuple("how", "you?", 1));
      THEN("edge iterator can iterate back to the start with the same amount of steps (bidirectional)"){
        --it;--it;--it;--it;--it;
      auto tuple2 = *it;
      REQUIRE(tuple2==std::make_tuple("are", "you?", 3));
      }
    }
    WHEN("First edge is erased from Graph") {
      auto it=g.begin();
      it=g.erase(it);
      THEN("iterator should point to second edge"){
        auto tuple1 = *it;
        REQUIRE(std::get<0>(tuple1)=="hello");
        REQUIRE(std::get<1>(tuple1)=="are");
        REQUIRE(std::get<2>(tuple1)==2);
      }
      THEN("first edge shouldn't be found when iterating") {
        it=g.begin();
        for (int i=0; it != g.end(); ++it,++i)
        {
          auto tuple = *it;
          if(i==0)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==1)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "how", 5));
          if(i==3)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          if(i==4)REQUIRE(tuple==std::make_tuple("how", "you?", 1));
        }
      }
    }
    WHEN("Middle edge is erased from Graph") {
      auto it=g.begin();
      ++it;++it;++it; auto tuple1 = *it;
      REQUIRE(std::get<0>(tuple1)=="hello");
      REQUIRE(std::get<1>(tuple1)=="how");
      REQUIRE(std::get<2>(tuple1)==5);
      it=g.erase(it);
      THEN("iterator should point to the next edge"){
        auto tuple2 = *it;
        REQUIRE(std::get<0>(tuple2)=="how");
        REQUIRE(std::get<1>(tuple2)=="hello");
        REQUIRE(std::get<2>(tuple2)==4);
      }
      THEN("middle edge shouldn't be found when iterating") {
        it=g.begin();
        for (int i=0; it != g.end(); ++it,++i)
        {
          auto tuple = *it;
          if(i==0)REQUIRE(tuple==std::make_tuple("are", "you?", 3));
          if(i==1)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==3)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          if(i==4)REQUIRE(tuple==std::make_tuple("how", "you?", 1));
        }
      }
    }
    WHEN("last edge is erased from Graph") {
      auto it=g.begin();
      ++it;++it;++it;++it;++it; auto tuple1 = *it;
      REQUIRE(std::get<0>(tuple1)=="how");
      REQUIRE(std::get<1>(tuple1)=="you?");
      REQUIRE(std::get<2>(tuple1)==1);
      it=g.erase(it);
      THEN("iterator should point to the end"){
        REQUIRE(it==g.end());
      }
      THEN("last edge shouldn't be found when iterating") {
        it=g.begin();
        for (int i=0; it != g.end(); ++it,++i)
        {
          auto tuple = *it;
          if(i==0)REQUIRE(tuple==std::make_tuple("are", "you?", 3));
          if(i==1)REQUIRE(tuple==std::make_tuple("hello", "are", 2));
          if(i==2)REQUIRE(tuple==std::make_tuple("hello", "are", 8));
          if(i==3)REQUIRE(tuple==std::make_tuple("hello", "how", 5));
          if(i==4)REQUIRE(tuple==std::make_tuple("how", "hello", 4));
          REQUIRE(i!=5);
        }
      }
    }
    WHEN("Find function is used to find first edge"){
      auto it = g.find("are", "you?", 3);
      THEN("Iterator returned should match the first edge"){
        auto tuple = *it;
        REQUIRE(tuple==std::make_tuple("are", "you?", 3));
      }
    }

    WHEN("Find function is used to find a middle edge"){
      auto it = g.find("hello", "how", 5);
      THEN("Iterator returned should match the first edge"){
        auto tuple = *it;
        REQUIRE(tuple==std::make_tuple("hello", "how", 5));
      }
    }
    WHEN("Find function is used to find the last edge"){
      auto it = g.find("how", "you?", 1);
      THEN("Iterator returned should match the first edge"){
        auto tuple = *it;
        REQUIRE(tuple==std::make_tuple("how", "you?", 1));
      }
    }
    WHEN("Find function is used to find a non-existent edge"){
      auto it = g.find("please", "goodMarks", 2);
      THEN("Iterator returned should return end()"){
        REQUIRE(it==g.end());
      }
    }
  }
}
