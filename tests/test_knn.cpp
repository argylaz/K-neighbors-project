#include "../lib/acutest.h"
#include "../lib/knn.hpp"
using namespace std;

/* Testing the GreedySearch method */
void test_GreedySearch() {
    // Test using simple example solved by hand
    // Using <vector<int>> because GreedySearch and EuclideanDistance work with vectors.
    TestGraph<vector<int>> G;
    
    // Add vertices 1,2,3,4,5
    G.add_vertex({1}); G.add_vertex({2}); G.add_vertex({3}); G.add_vertex({4}); G.add_vertex({5});

    // Add some random edges
    G.add_edge({1},{2}); G.add_edge({1},{3});
    G.add_edge({2}, {3});
    G.add_edge({3}, {1});G.add_edge({3}, {5});
    G.add_edge({4}, {3});
    G.add_edge({5}, {1}); G.add_edge({5}, {3}); G.add_edge({5}, {4});

    // Initialising variables for the results of the algorithm
    set<vector<int>> L;
    set<vector<int>> V;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< set<vector<int>>, set<vector<int>> > result = GreedySearch<vector<int>>(G, {1}, {4}, 2, 3);
    L = result.first;
    V = result.second;


    // Test that the sets returned contain the correct values (L = {3,5}, V = {1,2,3})
    TEST_ASSERT(L.find({3}) != L.end());
    TEST_ASSERT(L.find({5}) != L.end());

    TEST_ASSERT(V.find({1}) != V.end());
    TEST_ASSERT(V.find({2}) != V.end());
    TEST_ASSERT(V.find({3}) != V.end());
    TEST_ASSERT(V.find({5}) != V.end());
    TEST_ASSERT(L.size() == 2);
    TEST_ASSERT(V.size() == 4);

    // Test that empty sets are returned if L < k
    result = GreedySearch<vector<int>>(G, {1}, {4}, 2, 1);
    L = result.first;
    V = result.second;
    TEST_ASSERT( L.size() ==  0 );
    TEST_ASSERT( V.size() ==  0 );

    // Testing for floats

    TestGraph<vector<float>> G1;
    
    // Add vertices 1,2,3,4,5
    G1.add_vertex({1.1}); G1.add_vertex({2.1}); G1.add_vertex({3.1}); G1.add_vertex({4.0}); G1.add_vertex({5.1});

    // Add some random edges
    G1.add_edge({1.1},{2.1}); G1.add_edge({1.1},{3.1});
    G1.add_edge({2.1}, {3.1});
    G1.add_edge({3.1}, {1.1});G1.add_edge({3.1}, {5.1});
    G1.add_edge({4.1}, {3.1});
    G1.add_edge({5.1}, {1.1}); G1.add_edge({5.1}, {3.1}); G1.add_edge({5.1}, {4.1});

    // Initialising variables for the results of the algorithm
    set<vector<float>> L1;
    set<vector<float>> V1;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< set<vector<float>>, set<vector<float>> > result1 = GreedySearch<vector<float>>(G1, {1.1}, {4.1}, 2, 3);
    L1 = result1.first;
    V1 = result1.second;

    // Test that the sets returned contain the correct values (L = {3,5}, V = {1,2,3})
    TEST_ASSERT(L1.find({3.1}) != L1.end());
    TEST_ASSERT(L1.find({5.1}) != L1.end());

    TEST_ASSERT(V1.find({1.1}) != V1.end());
    TEST_ASSERT(V1.find({2.1}) != V1.end());
    TEST_ASSERT(V1.find({3.1}) != V1.end());
    TEST_ASSERT(V1.find({5.1}) != V1.end());
    TEST_ASSERT(L1.size() == 2);
    TEST_ASSERT(V1.size() == 4);

    // Test that empty sets are returned if L < k
    result1 = GreedySearch<vector<float>>(G1, {1.1}, {4.1}, 2, 1);
    L1 = result1.first;
    V1 = result1.second;
    TEST_ASSERT( L1.size() ==  0 );
    TEST_ASSERT( V1.size() ==  0 );


    // Testing for 2D vectors

    TestGraph<vector<int>> G2;
    
    // Add vertices 1,2,3,4,5
    G2.add_vertex({1,0}); G2.add_vertex({2,0}); G2.add_vertex({3,0}); G2.add_vertex({4,0}); G2.add_vertex({5,0});

    // Add some random edges
    G2.add_edge({1,0},{2,0}); G2.add_edge({1,0},{3,0});
    G2.add_edge({2,0}, {3,0});
    G2.add_edge({3,0}, {1,0});G2.add_edge({3,0}, {5,0});
    G2.add_edge({4,0}, {3,0});
    G2.add_edge({5,0}, {1,0}); G2.add_edge({5,0}, {3,0}); G2.add_edge({5,0}, {4,0});

    // Initialising variables for the results of the algorithm
    set<vector<int>> L2;
    set<vector<int>> V2;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< set<vector<int>>, set<vector<int>> > result2 = GreedySearch<vector<int>>(G2, {1,0}, {4,0}, 2, 3);
    L2 = result2.first;
    V2 = result2.second;


    // Test that the sets returned contain the correct values (L = {3,5}, V = {1,2,3})
    TEST_ASSERT(L2.find({3,0}) != L2.end());
    TEST_ASSERT(L2.find({5,0}) != L2.end());

    TEST_ASSERT(V2.find({1,0}) != V2.end());
    TEST_ASSERT(V2.find({2,0}) != V2.end());
    TEST_ASSERT(V2.find({3,0}) != V2.end());
    TEST_ASSERT(V2.find({5,0}) != V2.end());
    TEST_ASSERT(L2.size() == 2);
    TEST_ASSERT(V2.size() == 4);

    // Test that empty sets are returned if L < k
    result2 = GreedySearch<vector<int>>(G2, {1,0}, {4,0}, 2, 1);
    L2 = result2.first;
    V2 = result2.second;
    TEST_ASSERT( L2.size() ==  0 );
    TEST_ASSERT( V2.size() ==  0 );

}



/* Testing the RobustPrune method */
void test_RobustPrune() {
    // Test using simple example solved by hand (same as in test_GreedySearch)
    TestGraph<vector<int>> G;
    
    // Add vertices 0,1,2,3,4
    G.add_vertex({0}); G.add_vertex({1}); G.add_vertex({2}); G.add_vertex({3}); G.add_vertex({4});

    // Add some random edges
    G.add_edge({0},{1}); G.add_edge({0},{2});
    G.add_edge({1},{2});
    G.add_edge({2},{0}); G.add_edge({2},{4});
    G.add_edge({3},{2});
    G.add_edge({4},{0}); G.add_edge({4},{2}); G.add_edge({4},{3});

    // Run Robustprune with p = 0, V = {1,2} (out-neighbors of p), a = 1.2 and R = 2
    set<vector<int>> V = {{1},{2}};
    float a = 1.2;
    int R = 2;
    RobustPrune(G, 0, V, a, R);

    // Checking
    // cout << G.exist_edge({0},{1}) << G.exist_edge({0},{2}) << endl;
    // cout << G.exist_edge({1},{2}) << endl;
    // cout << G.exist_edge({2},{0}) << G.exist_edge({2},{4}) << endl;
    // cout << G.exist_edge({3},{2}) << endl;
    // cout << G.exist_edge({4},{0}) << G.exist_edge({4},{2}) << G.exist_edge({4},{3}) << endl;

    // Now check that the algorithm works as intended (only edge removes is the one from 1 to 3)
    TEST_ASSERT(G.exist_edge({0},{1}) && !G.exist_edge({0},{2}));
    TEST_ASSERT(G.exist_edge({1},{2}));
    TEST_ASSERT(G.exist_edge({2},{0}) && G.exist_edge({2},{4}));
    TEST_ASSERT(G.exist_edge({3},{2}));
    TEST_ASSERT(G.exist_edge({4},{0}) && G.exist_edge({4},{2}) && G.exist_edge({4},{3}));

    // Then we run RobustPrune one more time with p=4, V={0,2,3}, a=1.2 and R = 1
    V.clear(); V = {{0},{2},{3}};
    R = 1;
    RobustPrune(G, 4, V, a, R);

    // Checking
    // cout << G.exist_edge({0},{1}) << G.exist_edge({0},{2}) << endl;
    // cout << G.exist_edge({1},{2}) << endl;
    // cout << G.exist_edge({2},{0}) << G.exist_edge({2},{4}) << endl;
    // cout << G.exist_edge({3},{2}) << endl;
    // cout << G.exist_edge({4},{0}) << G.exist_edge({4},{2}) << G.exist_edge({4},{3}) << endl;

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G.exist_edge({0},{1}));
    TEST_ASSERT(G.exist_edge({1},{2}));
    TEST_ASSERT(G.exist_edge({2},{0}) && G.exist_edge({2},{4}));
    TEST_ASSERT(G.exist_edge({3},{2}));
    TEST_ASSERT(!G.exist_edge({4},{0}) && !G.exist_edge({4},{2}) && G.exist_edge({4},{3}));
}

/* Testing the Vamana method */
void test_Vamana() {
    return;
}

// List of all tests to be executed
TEST_LIST = {
    {"GreedySearch", test_GreedySearch},
    {"RobustPrune", test_RobustPrune},
    {"Vamana", test_Vamana},
    { NULL, NULL }
};
