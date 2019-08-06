/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

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


// OPERATORS
SCENARIO("Iterator functions") {
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
