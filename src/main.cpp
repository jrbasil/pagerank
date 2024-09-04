#include "graph.h"
#include <iostream>
#include <iomanip>
#include <map>
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::showpoint;
using std::setprecision;
using std::string;
using std::pair;
using std::map;

///// @brief The function called by the executable of this program
int main()
{
    graph page_graph; // instantiate a graph for pages
    int insertions, power_iterations; // declare variables to store input values

    cin >> insertions >> power_iterations; // read in the number of insertions and the number of power iterations
    string from, to; // declare variables to store input values

    for (int i = 0; i < insertions; i++) // for each insertion
    {
        cin >> from >> to; // read in the page names for insertion
        page_graph.insert(from, to); // pass page names as arguments for invocation to insert
    }

    map <string, double> page_ranks = page_graph.rank(power_iterations); // store the page ranks

    /// @credit: https://stackoverflow.com/questions/22515592/how-to-use-setprecision-in-c
    cout << fixed << showpoint << setprecision(2); // show two decimal places for floating point value read out

    for (pair <string, double> page_rank : page_ranks) // for each page rank
        cout << page_rank.first << " " << page_rank.second << endl; // read out each page name and corresponding rank

    return 0;
}