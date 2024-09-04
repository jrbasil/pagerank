#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::showpoint;
using std::setprecision;
using std::string;
using std::vector;
using std::make_pair;
using std::pair;
using std::map;
using std::unordered_map;
using std::unordered_set;

/// @brief Object class for a graph implemented as an adjacency list mapping each vertex to the set of inflows
class graph
{
private: // private member variables of this class
    unordered_map <int, unordered_set<int> > _inf_map; // map of size-based key to set of inflowing vertices
    unordered_map <string, int> _key_map; // map of string name key to size-based key value
    unordered_map <int, int> _out_map; // array of outdegrees dereferenced by vertex key
    int _size; // value for equivalent size of inf_map, out_map and key_map containers

public: // public member functions of this class
    bool insert(const string &from, const string &to);
    map <string, double> rank(int power_iterations);

    graph() { _size = 0; }

    // getters defined in-place
    const unordered_map <int, unordered_set<int> >& inf_map() const { return _inf_map; }
    const unordered_map <string, int>& key_map() const { return _key_map; }
    const unordered_map <int, int>& out_map() const { return _out_map; }
    int size() const { return _size; }
};

/**
 * @brief Attempt to insert a (from) vertex to the set of vertices inflowing to a base (to) vertex.
 *        Also insert, if not already inserted, either parameter (from and/or to) vertex to the vertex maps.
 *
 * @param from  string key for a vertex that is inflowing to the base vertex
 * @param to    string key for a base vertex
 * @return      boolean value indicating whether or not the vertex was inserted to the set of inflowing vertices;
 *              returns false if the vertex was already inserted to the set of inflowing vertices.
 */
bool graph::insert(const string &from, const string &to)
{
    auto insert_from = _key_map.insert(make_pair(from, _size)); // attempt insert parameter vertex to graph
    bool inserted_from = insert_from.second; // store boolean result of insert

    if (inserted_from) // if newly inserted to key map
    {
        _inf_map.insert(make_pair(_size, unordered_set <int> ())); // insert empty set of inflows
        _out_map.insert(make_pair(_size, 0)); // insert initial outdegree
        _size++; // update size-based key
    }
    int key_from = insert_from.first->second; // store key regardless of insert result

    auto insert_to = _key_map.insert(make_pair(to, _size)); // attempt insert parameter vertex to graph
    bool inserted_to = insert_to.second; // store boolean result of insert

    if (inserted_to) // if newly inserted to key map
    {
        _inf_map.insert(make_pair(_size, unordered_set <int> ())); // insert empty set of inflows
        _out_map.insert(make_pair(_size, 0)); // insert initial outdegree
        _size++; // update size-based key
    }
    int key_to = insert_to.first->second; // store key regardless of insert result

    bool inserted_inf = _inf_map.at(key_to).insert(key_from).second; // attempt insert inflow from mapped keys
    if (inserted_inf) _out_map.at(key_from)++; // increment outdegree

    return inserted_inf; // return boolean result of insert
}

/**
 * @brief Get a map from each vertex name to the corresponding computed rank based on the number of power iterations.
 *
 * @param power_iterations number of times to repeat the rank computation using the previous rank as a base
 * @return                 ordered @c map of string name key to rank value for a given number of power iterations.
 */
map <string, double> graph::rank(const int power_iterations)
{
    vector <double> inf_ranks(_size, (1 / static_cast <double> (_size))); // store ranks for inflow vertices
    vector <double> base_ranks = inf_ranks; // create array for storing ranks for base vertices

    for (int p = power_iterations; p > 1; p--) // for each power iteration
    {
        auto base_it = _inf_map.begin(); // reinitialize begin iterator for base vertex
        while (base_it != _inf_map.end()) // for each base vertex
        {
            double base_rank = 0; // reinitialize sum of incoming ranks
            unordered_set <int> inf = base_it->second;
            auto inf_it = inf.begin(); // get inflow vertices from base vertex
            while (inf_it != inf.end()) // for each inflowing vertex
            {
                int inf_key = *(inf_it); // get the inflow key
                int out = _out_map.at(inf_key); // get the outdegree from the inflow key
                double inf_rank = inf_ranks.at(inf_key); // dereference rank for this inflow from array
                base_rank += (inf_rank / static_cast <double> (out)); // add rank for this inflow to sum for all inflows
                inf_it++; // move iterator forward to next possible inflow vertex
            }
            base_ranks.at(base_it->first) = base_rank; // store this base rank for all inflows in base ranks array
            base_it++; // move iterator forward to next possible base vertex
        }
        inf_ranks = base_ranks; // update base ranks for next power iteration
    }

    map <string, double> mapped_ranks; // declare ordered map variable for organizing page name and rank pairs
    for (int i = 0; i < _size; i++) // for each rank
    {
        auto it = _key_map.begin(); // reinitialize key_map begin iterator
        for (int it_pos = 0; it_pos < i; it_pos++) it++; // move iterator to position of rank
        mapped_ranks.insert(make_pair(it->first, base_ranks.at(it->second))); // insert page name and rank
    }

    return mapped_ranks; // return map of ranks alphabetized by page name key
}

#endif // GRAPH_H