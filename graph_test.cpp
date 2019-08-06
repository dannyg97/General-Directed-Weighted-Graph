/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.
   
  

*/

#include "assignments/dg/graph.h"
#include "catch.h"


// OPERATORS
SCENARIO("Forward iterator loop") {
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
        ++it;++it;++it;++it;++it;
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
  }
}
