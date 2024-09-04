#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#define TEST_SIZE 100000

#include "../src/graph.h"
#include "catch.hpp"
using std::to_string;
using std::is_sorted;
using std::sqrt;

TEST_CASE("Inserts succeed")
{
    graph test_graph;

    int i = 0;
    while (i < TEST_SIZE) REQUIRE(test_graph.insert(to_string(i), to_string(++i)));

    REQUIRE(test_graph.insert(to_string(i), to_string(0)));
}

TEST_CASE("Inserts persist")
{
    graph test_graph;

    int i = 0;
    while (i < TEST_SIZE) test_graph.insert(to_string(i), to_string(++i));

    test_graph.insert(to_string(i), to_string(0));
    REQUIRE(test_graph.size() == TEST_SIZE + 1);
    REQUIRE(i == TEST_SIZE);
}

TEST_CASE("Maps align")
{
    graph test_graph;

    int i = 0;
    while (i < TEST_SIZE) test_graph.insert(to_string(i), to_string(++i));

    test_graph.insert(to_string(i), to_string(0));

    auto key_map = test_graph.key_map();
    auto inf_map = test_graph.inf_map();

    int j = 0;
    for (auto inf_it = inf_map.begin(); inf_it != inf_map.end(); inf_it++)
        REQUIRE(key_map.at(to_string(j++)) == inf_it->first);

    REQUIRE(j == TEST_SIZE + 1);
}

TEST_CASE("Ranks sort")
{
    graph test_graph;

    int i = 0;
    while (i < sqrt(TEST_SIZE)) test_graph.insert(to_string(i), to_string(++i));

    test_graph.insert(to_string(i), to_string(0));

    int j = 0;
    while (j < sqrt(TEST_SIZE))
    {
        auto rank = test_graph.rank(j++);
        REQUIRE(is_sorted(rank.begin(), rank.end()));
    }
}

TEST_CASE("Ranks compute (even distribution)")
{
    int j = 1;
    while (j < sqrt(TEST_SIZE))
    {
        graph test_graph;
        int i = 0;
        while (i < j) test_graph.insert(to_string(i), to_string(++i));

        test_graph.insert(to_string(i), to_string(0));

        auto rank = test_graph.rank(j++);
        for (pair <string, float> p : rank) REQUIRE(p.second == (1 / static_cast <float> (i + 1)));
    }
}

#endif // CATCH_CONFIG_MAIN