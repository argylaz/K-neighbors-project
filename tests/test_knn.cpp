#include "../lib/acutest.h"
#include "../lib/knn.hpp"
using namespace std;

/* Testing the GreedySearch method */
void test_GreedySearch() {
    // Test using simple example solved by hand
    // Using <vector<int>> because GreedySearch and EuclideanDistance work with vectors.
    TestGraph<vector<int>> G;
    
    // Add vertices 0,1,2,3,4,5
    G.add_vertex({0}); G.add_vertex({1}); G.add_vertex({2}); G.add_vertex({3}); G.add_vertex({4}); 

    // Add some random edges
    G.add_edge({0}, {1}); G.add_edge({0}, {2});
    G.add_edge({1}, {2});
    G.add_edge({2}, {0}); G.add_edge({2}, {4});
    G.add_edge({3}, {2});
    G.add_edge({4}, {0}); G.add_edge({4}, {2}); G.add_edge({4}, {3});

    // Initialising variables for the results of the algorithm
    vector<gIndex> L;
    vector<gIndex> V;

    cout<<1<< endl;
    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< vector<gIndex>, vector<gIndex> > result = GreedySearch<vector<int>>(G, {0}, {3}, 2, 3);
    L = result.first;
    V = result.second;

    cout<<2<< endl;
    // Test that the sets returned contain the correct values (L = {2,4}, V = {0,1,2,4})
    TEST_ASSERT(find(L.begin(), L.end(), 2) != L.end());
    TEST_ASSERT(find(L.begin(), L.end(), 4) != L.end());

    TEST_ASSERT(find(V.begin(), V.end(), 0) != V.end());
    TEST_ASSERT(find(V.begin(), V.end(), 1) != V.end());
    TEST_ASSERT(find(V.begin(), V.end(), 2) != V.end());
    TEST_ASSERT(find(V.begin(), V.end(), 4) != V.end());
    TEST_ASSERT(L.size() == 2);
    TEST_ASSERT(V.size() == 4);

    cout<<3<< endl;
    // Test that empty sets are returned if L < k
    result = GreedySearch<vector<int>>(G, {0}, {3}, 2, 1);
    L = result.first;
    V = result.second;
    TEST_ASSERT( L.size() ==  0 );
    TEST_ASSERT( V.size() ==  0 );

    // Testing for floats

    TestGraph<vector<float>> G1;
    
    // Add vertices 0,1,2,3,4
    G1.add_vertex({0.1}); G1.add_vertex({1.1}); G1.add_vertex({2.1}); G1.add_vertex({3.1}); G1.add_vertex({4.1});

    // Add some random edges
    G1.add_edge({0.1}, {1.1}); G1.add_edge({0.1}, {2.1});
    G1.add_edge({1.1}, {2.1});
    G1.add_edge({2.1}, {0.1}); G1.add_edge({2.1}, {4.1});
    G1.add_edge({3.1}, {2.1});
    G1.add_edge({4.1}, {0.1}); G1.add_edge({4.1}, {2.1}); G1.add_edge({4.1}, {3.1});

    // Initialising variables for the results of the algorithm
    vector<gIndex> L1;
    vector<gIndex> V1;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< vector<gIndex>, vector<gIndex> > result1 = GreedySearch<vector<float>>(G1, {0.1}, {3}, 2, 3);
    L1 = result1.first;
    V1 = result1.second;

    // Test that the sets returned contain the correct values (L = {3,5}, V = {1,2,3})
       // Test that the sets returned contain the correct values (L = {2,4}, V = {0,1,2,4})
    TEST_ASSERT(find(L1.begin(), L1.end(), 2) != L1.end());
    TEST_ASSERT(find(L1.begin(), L1.end(), 4) != L1.end());

    TEST_ASSERT(find(V1.begin(), V1.end(), 0) != V1.end());
    TEST_ASSERT(find(V1.begin(), V1.end(), 1) != V1.end());
    TEST_ASSERT(find(V1.begin(), V1.end(), 2) != V1.end());
    TEST_ASSERT(find(V1.begin(), V1.end(), 4) != V1.end());
    TEST_ASSERT(L1.size() == 2);
    TEST_ASSERT(V1.size() == 4);

    // Test that empty sets are returned if L < k
    result1 = GreedySearch<vector<float>>(G1, {0.1}, {3}, 2, 1);
    L1 = result.first;
    V1 = result.second;
    TEST_ASSERT( L1.size() ==  0 );
    TEST_ASSERT( V1.size() ==  0 );


    // Testing for 2D vectors

    TestGraph<vector<int>> G2;
    
    // Add vertices 1,2,3,4,5
    G2.add_vertex({5,0}); G2.add_vertex({1,0}); G2.add_vertex({2,0}); G2.add_vertex({3,0}); G2.add_vertex({4,0});

    // Add some random edges

    G2.add_edge({0,0}, {1,0}); G2.add_edge({0,0}, {2,0});
    G2.add_edge({1,0}, {2,0});
    G2.add_edge({2,0}, {0,0}); G2.add_edge({2,0}, {4,0});
    G2.add_edge({3,0}, {2,0});
    G2.add_edge({4,0}, {0,0}); G2.add_edge({4,0}, {2,0}); G2.add_edge({4,0}, {3,0});

    // Initialising variables for the results of the algorithm
    vector<gIndex> L2;
    vector<gIndex> V2;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair<vector<gIndex>, vector<gIndex> > result2 = GreedySearch<vector<int>>(G2, {0,0}, {3}, 2, 3);
    L2 = result2.first;
    V2 = result2.second;


    // Test that the sets returned contain the correct values (L = {3,5}, V = {1,2,3})
    TEST_ASSERT(find(L2.begin(), L2.end(), 2) != L2.end());
    TEST_ASSERT(find(L2.begin(), L2.end(), 4) != L2.end());

    TEST_ASSERT(find(V2.begin(), V2.end(), 0) != V2.end());
    TEST_ASSERT(find(V2.begin(), V2.end(), 1) != V2.end());
    TEST_ASSERT(find(V2.begin(), V2.end(), 2) != V2.end());
    TEST_ASSERT(find(V2.begin(), V2.end(), 4) != V2.end());
    TEST_ASSERT(L2.size() == 2);
    TEST_ASSERT(V2.size() == 4);

    // Test that empty sets are returned if L < k
    result2 = GreedySearch<vector<int>>(G, {0,0}, {3}, 2, 1);
    L2 = result.first;
    V2 = result.second;
    TEST_ASSERT( L2.size() ==  0 );
    TEST_ASSERT( V2.size() ==  0 );

}



/* Testing the RobustPrune method */
void test_RobustPrune() {
    
    // Testing for ints

    TestGraph<vector<int>> G1;
    
    // Add vertices 0,1,2,3,4
    G1.add_vertex({0}); G1.add_vertex({1}); G1.add_vertex({2}); G1.add_vertex({3}); G1.add_vertex({4});

    // Add some random edges
    G1.add_edge({0}, {1}); G1.add_edge({0}, {2});
    G1.add_edge({1}, {2});
    G1.add_edge({2}, {0}); G1.add_edge({2}, {4});
    G1.add_edge({3}, {2});
    G1.add_edge({4}, {0}); G1.add_edge({4}, {2}); G1.add_edge({4}, {3});

    // Run Robustprune with p = {0}, V = {{1},{2}} (out-neighbors of p), a = 1.2 and R = 2
    vector<gIndex> V1 = {1, 2};
    float a = 1.2;
    int R = 2;
    RobustPrune(G1, {0}, V1, a, R);

    // Now check that the algorithm works as intended (only edge removes is the one from 1 to 3)
    TEST_ASSERT(G1.exist_edge({0}, {1}) && !G1.exist_edge({0}, {2}));
    TEST_ASSERT(G1.exist_edge({1}, {2}));
    TEST_ASSERT(G1.exist_edge({2}, {0}) && G1.exist_edge({2}, {4}));
    TEST_ASSERT(G1.exist_edge({3}, {2}));
    TEST_ASSERT(G1.exist_edge({4}, {0}) && G1.exist_edge({4}, {2}) && G1.exist_edge({4}, {3}));

    // Then we run RobustPrune one more time with p = {4}, V = {{0},{2},{3}}, a=1.2 and R = 1
    V1.clear(); V1 = {0, 2, 3};
    R = 1;
    RobustPrune(G1, {4}, V1, a, R);

    // Checking
    // cout << G1.exist_edge({0}, {1}) << G1.exist_edge({0}, {2}) << endl;
    // cout << G1.exist_edge({1}, {2}) << endl;
    // cout << G1.exist_edge({2}, {0}) << G1.exist_edge({2}, {4}) << endl;
    // cout << G1.exist_edge({3}, {2}) << endl;
    // cout << G1.exist_edge({4}, {0}) << G1.exist_edge({4}, {2}) << G1.exist_edge({4}, {3}) << endl;

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G1.exist_edge({0}, {1}));
    TEST_ASSERT(G1.exist_edge({1}, {2}));
    TEST_ASSERT(G1.exist_edge({2}, {0}) && G1.exist_edge({2}, {4}));
    TEST_ASSERT(G1.exist_edge({3}, {2}));
    TEST_ASSERT(!G1.exist_edge({4}, {0}) && !G1.exist_edge({4}, {2}) && G1.exist_edge({4}, {3}));



    // Testing for floats, shortened version of the int test, same data with {x.1} form

    TestGraph<vector<float>> G2;
    
    G2.add_vertex({0.1}); G2.add_vertex({1.1}); G2.add_vertex({2.1}); G2.add_vertex({3.1}); G2.add_vertex({4.1});

    // Add some random edges
    G2.add_edge({0.1}, {1.1}); G2.add_edge({0.1}, {2.1});
    G2.add_edge({1.1}, {2.1});
    G2.add_edge({2.1}, {0.1}); G2.add_edge({2.1}, {4.1});
    G2.add_edge({3.1}, {2.1});
    G2.add_edge({4.1}, {0.1}); G2.add_edge({4.1}, {2.1}); G2.add_edge({4.1}, {3.1});

    // Run Robustprune with p = {0.1}, V = {{1.1},{2.1}} (out-neighbors of p), a = 1.2 and R = 2
    vector<gIndex> V2 = {1, 2};
    a = 1.2;
    R = 2;
    RobustPrune(G2, {0.1}, V2, a, R);

    // Then we run RobustPrune one more time with p = {4.1}, V = {{0.1},{2.1},{3.1}}, a=1.2 and R = 1
    V2.clear(); V2 = {0, 2, 3};
    R = 1;
    RobustPrune(G2, {4.1}, V2, a, R);

    // Checking
    // cout << G2.exist_edge({0.1}, {1.1}) << G2.exist_edge({0.1}, {2.1}) << endl;
    // cout << G2.exist_edge({1.1}, {2.1}) << endl;
    // cout << G2.exist_edge({2.1}, {0.1}) << G2.exist_edge({2.1}, {4.1}) << endl;
    // cout << G2.exist_edge({3.1}, {2.1}) << endl;
    // cout << G2.exist_edge({4.1}, {0.1}) << G2.exist_edge({4.1}, {2.1}) << G2.exist_edge({4.1}, {3.1}) << endl;

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G2.exist_edge({0.1}, {1.1}) && !G2.exist_edge({0.1}, {2.1}));
    TEST_ASSERT(G2.exist_edge({1.1}, {2.1}));
    TEST_ASSERT(G2.exist_edge({2.1}, {0.1}) && G2.exist_edge({2.1}, {4.1}));
    TEST_ASSERT(G2.exist_edge({3.1}, {2.1}));
    TEST_ASSERT(!G2.exist_edge({4.1}, {0.1}) && !G2.exist_edge({4.1}, {2.1}) && G2.exist_edge({4.1}, {3.1}));
    
    
    
    // Testing for 2D float vectors, shortened version of the int test, same data with {x.1,9.9} form

    TestGraph<vector<float>> G3;
    
    G3.add_vertex({0.1, 9.9}); G3.add_vertex({1.1, 9.9}); G3.add_vertex({2.1, 9.9}); G3.add_vertex({3.1, 9.9}); G3.add_vertex({4.1, 9.9});

    // Add some random edges
    G3.add_edge({0.1, 9.9}, {1.1, 9.9}); G3.add_edge({0.1, 9.9}, {2.1, 9.9});
    G3.add_edge({1.1, 9.9}, {2.1, 9.9});
    G3.add_edge({2.1, 9.9}, {0.1, 9.9}); G3.add_edge({2.1, 9.9}, {4.1, 9.9});
    G3.add_edge({3.1, 9.9}, {2.1, 9.9});
    G3.add_edge({4.1, 9.9}, {0.1, 9.9}); G3.add_edge({4.1, 9.9}, {2.1, 9.9}); G3.add_edge({4.1, 9.9}, {3.1, 9.9});

    // Run Robustprune with p = {0.1,9.9}, V = {{1.1,9.9},{2.1,9.9}} (out-neighbors of p), a = 1.2 and R = 2
    vector<gIndex> V3 = {1, 2};
    a = 1.2;
    R = 2;
    RobustPrune(G3, {0.1, 9.9}, V3, a, R);

    // Then we run RobustPrune one more time with p = {4.1,9.9}, V = {{0.1,9.9}, {2.1,9.9}, {3.1,9.9}}, a=1.2 and R = 1
    V3.clear(); V3 = {0, 2, 3};
    R = 1;
    RobustPrune(G3, {4.1, 9.9}, V3, a, R);

    // Checking
    // cout << G3.exist_edge({0.1, 9.9}, {1.1, 9.9}) << G3.exist_edge({0.1, 9.9}, {2.1, 9.9}) << endl;
    // cout << G3.exist_edge({1.1, 9.9}, {2.1, 9.9}) << endl;
    // cout << G3.exist_edge({2.1, 9.9}, {0.1, 9.9}) << G3.exist_edge({2.1, 9.9}, {4.1, 9.9}) << endl;
    // cout << G3.exist_edge({3.1, 9.9}, {2.1, 9.9}) << endl;
    // cout << G3.exist_edge({4.1, 9.9}, {0.1, 9.9}) << G3.exist_edge({4.1, 9.9}, {2.1, 9.9}) << G3.exist_edge({4.1, 9.9}, {3.1, 9.9}) << endl;

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G3.exist_edge({0.1, 9.9}, {1.1, 9.9}) && !G3.exist_edge({0.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(G3.exist_edge({1.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(G3.exist_edge({2.1, 9.9}, {0.1, 9.9}) && G3.exist_edge({2.1, 9.9}, {4.1, 9.9}));
    TEST_ASSERT(G3.exist_edge({3.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(!G3.exist_edge({4.1, 9.9}, {0.1, 9.9}) && !G3.exist_edge({4.1, 9.9}, {2.1, 9.9}) && G3.exist_edge({4.1, 9.9}, {3.1, 9.9}));


}

/* Testing the Vamana method */
void test_Vamana() {
    /* Testing the vamana method with a simple example with integers solved by hand */
    TestGraph<vector<int>> G1;

    // Add vertices 0,1,2,3
    G1.add_vertex({0}); G1.add_vertex({1}); G1.add_vertex({2}); G1.add_vertex({3});

    // Add edges 0->2, 0->3, 1->2, 1->3, 2->0, 2->1, 3->0, 3->1
    // just for test because they don't matter because rDirectional will remove them
    G1.add_edge({0}, {2}); G1.add_edge({0}, {3});
    G1.add_edge({1}, {2}); G1.add_edge({1}, {3});
    G1.add_edge({2}, {0}); G1.add_edge({2}, {1});
    G1.add_edge({3}, {0}); G1.add_edge({3}, {1});

    // Run vamana indexing algorithm with L = 2 and R = 2
    Vamana(G1, 2, 2);

    // Test that the new form of the graph is the one one expected
    // 0->1, 1->0, 1->2, 2->1, 2->3, 3->2 are the exact edges the graph should have
    // cout << G.get_edge_count();
    // TEST_ASSERT(G.get_edge_count() == 6);
    TEST_ASSERT(G1.exist_edge({0}, {1}));
    TEST_ASSERT(G1.exist_edge({1}, {0}));
    TEST_ASSERT(G1.exist_edge({1}, {2}));
    TEST_ASSERT(G1.exist_edge({2}, {1}));
    TEST_ASSERT(G1.exist_edge({2}, {3}));
    TEST_ASSERT(G1.exist_edge({3}, {2}));

    TEST_ASSERT(!G1.exist_edge({0}, {2}));
    TEST_ASSERT(!G1.exist_edge({0}, {3}));
    TEST_ASSERT(!G1.exist_edge({1}, {3}));
    TEST_ASSERT(!G1.exist_edge({2}, {0}));
    TEST_ASSERT(!G1.exist_edge({3}, {0}));
    TEST_ASSERT(!G1.exist_edge({3}, {1}));



    /* Testing the vamana method for 2d float vectors, shortened version of the int test, same data with {x.1,9.9} form*/
    TestGraph<vector<float>> G2;

    // Add vertices 0,1,2,3
    G2.add_vertex({0.1, 9.9}); G2.add_vertex({1.1, 9.9}); G2.add_vertex({2.1, 9.9}); G2.add_vertex({3.1, 9.9});

    // Run vamana indexing algorithm with L = 2 and R = 2
    Vamana(G2, 2, 2);

    // Test that the new form of the graph is the one one expected 
    // 0->1, 1->0, 1->2, 2->1, 2->3, 3->2 are the exact edges the graph should have
    // TEST_ASSERT(G.get_edge_count() == 6);
    TEST_ASSERT(G2.exist_edge({0.1, 9.9}, {1.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({1.1, 9.9}, {0.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({1.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({2.1, 9.9}, {1.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({2.1, 9.9}, {3.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({3.1, 9.9}, {2.1, 9.9}));

    TEST_ASSERT(!G2.exist_edge({0.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(!G2.exist_edge({0.1, 9.9}, {3.1, 9.9}));
    TEST_ASSERT(!G2.exist_edge({1.1, 9.9}, {3.1, 9.9}));
    TEST_ASSERT(!G2.exist_edge({2.1, 9.9}, {0.1, 9.9}));
    TEST_ASSERT(!G2.exist_edge({3.1, 9.9}, {0.1, 9.9}));
    TEST_ASSERT(!G2.exist_edge({3.1, 9.9}, {1.1, 9.9}));
    
}

// List of all tests to be executed
TEST_LIST = {
    {"GreedySearch", test_GreedySearch},
    {"RobustPrune", test_RobustPrune},
    {"Vamana", test_Vamana},
    { NULL, NULL }
};
