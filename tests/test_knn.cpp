#include "../lib/acutest.h"
#include "../lib/utils.hpp";
#include "../lib/knn.hpp"
using namespace std;

/* Testing the find_min_Euclidean method */
void test_find_min_Euclidean() {
    
}

/* Testing the GreedySearch method */
void test_GreedySearch() {
    // Test using simple example solved by hand
    TestGraph<char> G;
    
    // Add vertices a,b,c,d,e
    G.add_vertex('a'); G.add_vertex('b'); G.add_vertex('c'); G.add_vertex('d'); G.add_vertex('e');

    // Add some random edges
    G.add_edge('a','b'); G.add_edge('a','c');
    G.add_edge('b', 'c');
    G.add_edge('c', 'a');G.add_edge('c', 'e');
    G.add_edge('d', 'c');
    G.add_edge('e', 'a'); G.add_edge('e', 'c'); G.add_edge('e', 'd');

    // Initialising variables for the results of the algorithm
    set<char> L;
    set<char> V;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair<set<char>, set<char>> result = GreedySearch(G, 1, 4, 2, 3);
    L = result.first;
    V = result.second;

    // Test that the sets returned contain the correct values (L = {c,e}, V = {a,c,e,d})
    TEST_ASSERT(L.find('c') != L.end());
    TEST_ASSERT(L.find('e') != L.end());

    TEST_ASSERT(V.find('a') != V.end());
    TEST_ASSERT(V.find('c') != V.end());
    TEST_ASSERT(V.find('d') != V.end());
    TEST_ASSERT(V.find('e') != V.end());
}

/* Testing the RobustPrune method */
void test_RobustPrune() {

}

/* Testing the Vamana method */
void test_Vamana() {

}

// List of all tests to be executed
TEST_LIST = {
    {"Find_min_Euclidean", test_find_min_Euclidean},
    {"GreedySearch", test_GreedySearch},
    {"RobustPrune", test_RobustPrune},
    {"Vamana", test_Vamana},
    { NULL, NULL }
};
