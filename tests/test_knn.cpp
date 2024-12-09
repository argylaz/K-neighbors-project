#include "../lib/acutest.h"
#include "../lib/knn.hpp"
using namespace std;

/* Testing the GreedySearch method */
void test_GreedySearch() {
    // Test using simple example solved by hand
    // Using <vector<int>> because GreedySearch and EuclideanDistance work with vectors.
    Graph<vector<int>> G;
    
    // Add vertices 0,1,2,3,4
    G.add_vertex({0}); G.add_vertex({1}); G.add_vertex({2}); G.add_vertex({3}); G.add_vertex({4}); 

    // Add some random edges
    G.add_edge({0}, {1}); G.add_edge({0}, {2});
    G.add_edge({1}, {2});
    G.add_edge({2}, {0}); G.add_edge({2}, {4});
    G.add_edge({3}, {2});
    G.add_edge({4}, {0}); G.add_edge({4}, {2}); G.add_edge({4}, {3});

    // Initialising variables for the results of the algorithm
    set<gIndex> L;
    set<gIndex> V;

    

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< set<gIndex>, set<gIndex> > result = GreedySearch<vector<int>>(G, {0}, {3}, 2, 3);
    L = result.first;
    V = result.second;


    // Test that the sets returned contain the correct values (L = {2,4}, V = {0,1,2,4})
    TEST_ASSERT(L.find(2) != L.end());
    TEST_ASSERT(L.find(4) != L.end());

    TEST_ASSERT(V.find(0) != V.end());
    TEST_ASSERT(V.find(1) != V.end());
    TEST_ASSERT(V.find(2) != V.end());
    TEST_ASSERT(V.find(4) != V.end());
    TEST_ASSERT(L.size() == 2);
    TEST_ASSERT(V.size() == 4);


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
    set<gIndex> L1;
    set<gIndex> V1;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< set<gIndex>, set<gIndex> > result1 = GreedySearch<vector<float>>(G1, {0.1}, {3.1}, 2, 3);
    L1 = result1.first;
    V1 = result1.second;

    // Test that the sets returned contain the correct values (L = {2,4}, V = {0,1,2,4})
    TEST_ASSERT(L1.find(2) != L1.end());
    TEST_ASSERT(L1.find(4) != L1.end());

    TEST_ASSERT(V1.find(0) != V1.end());
    TEST_ASSERT(V1.find(1) != V1.end());
    TEST_ASSERT(V1.find(2) != V1.end());
    TEST_ASSERT(V1.find(4) != V1.end());
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
    G2.add_vertex({0,0}); G2.add_vertex({1,0}); G2.add_vertex({2,0}); G2.add_vertex({3,0}); G2.add_vertex({4,0});

    // Add some random edges

    G2.add_edge({0,0}, {1,0}); G2.add_edge({0,0}, {2,0});
    G2.add_edge({1,0}, {2,0});
    G2.add_edge({2,0}, {0,0}); G2.add_edge({2,0}, {4,0});
    G2.add_edge({3,0}, {2,0});
    G2.add_edge({4,0}, {0,0}); G2.add_edge({4,0}, {2,0}); G2.add_edge({4,0}, {3,0});

    // Initialising variables for the results of the algorithm
    set<gIndex> L2;
    set<gIndex> V2;

    /* Running the algorithm with s = 'a', xq = 'd', k = 2 and L = 3*/
    pair< set<gIndex>, set<gIndex> > result2 = GreedySearch<vector<int>>(G2, {0,0}, {3,0}, 2, 3);
    L2 = result2.first;
    V2 = result2.second;


    // Test that the sets returned contain the correct values (L = {2,4}, V = {1,2,3})
    TEST_ASSERT(L2.find(2) != L2.end());
    TEST_ASSERT(L2.find(4) != L2.end());

    TEST_ASSERT(V2.find(0) != V2.end());
    TEST_ASSERT(V2.find(1) != V2.end());
    TEST_ASSERT(V2.find(2) != V2.end());
    TEST_ASSERT(V2.find(4) != V2.end());
    TEST_ASSERT(L2.size() == 2);
    TEST_ASSERT(V2.size() == 4);

    // Test that empty sets are returned if L < k
    result2 = GreedySearch<vector<int>>(G, {0,0}, {3}, 2, 1);
    L2 = result.first;
    V2 = result.second;
    TEST_ASSERT( L2.size() ==  0 );
    TEST_ASSERT( V2.size() ==  0 );

}




/* Testing FilteredGreedySearch method  using an example solved by hand */
void test_FilteredGreedySearch() {
    
    FilterGraph<vector<int>,int> G(true);

    // added some random filters
    // Adding vertices 0,1,2,3,4
    G.add_vertex({0},{0}); G.add_vertex({1},{1}); G.add_vertex({2},{1}); G.add_vertex({3},{0}); G.add_vertex({4},{1});

    // Adding some edges 
    G.add_edge({0}, {1}); G.add_edge({0}, {3});
    G.add_edge({1}, {2}); G.add_edge({1}, {4});
    G.add_edge({2}, {3}); G.add_edge({2}, {4});
    G.add_edge({3}, {1}); G.add_edge({3}, {4});
    G.add_edge({4}, {0}); G.add_edge({4}, {1});

    // Run FilteredGreedySearch with 
    int k = 2;
    int L = 3;
    vector<int> xquery = {2};
    set<int> filter = {1};

    // Call find medoid and get the set of starting nodes
    // int threshold = 2;
    // set<vector<int>> S = get_nodes_from_gIndex_map(G, FindMedoid(G, threshold));
    set<vector<int>> S = G.get_vertices();

    // printing S set
    // for( vector<int> i : S){
    //     print_vector(i);
    // }

    auto result = FilteredGreedySearch<vector<int>, int>(G, S, xquery, k, L, filter);
    set<gIndex> Lout = result.first;
    set<gIndex> V = result.second;

    // cout << endl;
    // for( gIndex s : Lout){
    //     cout << s << endl;
    // }

    // Printing Graph
    // cout << endl;
    // G.print_graph();

    // Test that the returned values are the ones expected (L = {1,4}, V = {1,2,4})
    TEST_ASSERT(Lout.size() == 2);
    TEST_ASSERT(Lout.find(1) != Lout.end());  // 1 in L
    TEST_ASSERT(Lout.find(4) != Lout.end());  // 4 in L

    TEST_ASSERT(V.size() == 3);
    TEST_ASSERT(V.find(1) != V.end());  // 1 on V
    TEST_ASSERT(V.find(2) != V.end());  // 2 in V
    TEST_ASSERT(V.find(4) != V.end());  // 4 in V
}




/* Testing the RobustPrune method */
void test_RobustPrune() {
    
    /* Testing for ints */

    Graph<vector<int>> G1;
    
    // Add vertices 0,1,2,3,4
    G1.add_vertex({0}); G1.add_vertex({1}); G1.add_vertex({2}); G1.add_vertex({3}); G1.add_vertex({4});

    // Add some edges
    G1.add_edge({0}, {1}); G1.add_edge({0}, {2});
    G1.add_edge({1}, {2});
    G1.add_edge({2}, {0}); G1.add_edge({2}, {4});
    G1.add_edge({3}, {2});
    G1.add_edge({4}, {0}); G1.add_edge({4}, {2}); G1.add_edge({4}, {3});

    // Run Robustprune with p = {0}, V = {1, 2} (out-neighbors of p), a = 1.2 and R = 2
    set<gIndex> V1 = {1, 2};
    float a = 1.2;
    int R = 2;

    RobustPrune(G1, {0}, V1, a, R);

    // Now check that the algorithm works as intended (only edge removes is the one from 1 to 3)
    TEST_ASSERT(G1.exist_edge({0}, {1}) && !G1.exist_edge({0}, {2}));
    TEST_ASSERT(G1.exist_edge({1}, {2}));
    TEST_ASSERT(G1.exist_edge({2}, {0}) && G1.exist_edge({2}, {4}));
    TEST_ASSERT(G1.exist_edge({3}, {2}));
    TEST_ASSERT(G1.exist_edge({4}, {0}) && G1.exist_edge({4}, {2}) && G1.exist_edge({4}, {3}));


    // Then we run RobustPrune one more time with p = {4}, V = {0, 2, 3}, a=1.2 and R = 1
    V1.clear(); V1 = {0, 2, 3};
    V1.clear();

    R = 1;
    RobustPrune(G1, {4}, V1, a, R);

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G1.exist_edge({0}, {1}));
    TEST_ASSERT(G1.exist_edge({1}, {2}));
    TEST_ASSERT(G1.exist_edge({2}, {0}) && G1.exist_edge({2}, {4}));
    TEST_ASSERT(G1.exist_edge({3}, {2}));
    TEST_ASSERT(!G1.exist_edge({4}, {0}) && !G1.exist_edge({4}, {2}) && G1.exist_edge({4}, {3}));





    // Testing for floats, shortened version of the int test, same data with {x.1} form

    Graph<vector<float>> G2;
    
    G2.add_vertex({0.1}); G2.add_vertex({1.1}); G2.add_vertex({2.1}); G2.add_vertex({3.1}); G2.add_vertex({4.1});

    // Add some random edges
    G2.add_edge({0.1}, {1.1}); G2.add_edge({0.1}, {2.1});
    G2.add_edge({1.1}, {2.1});
    G2.add_edge({2.1}, {0.1}); G2.add_edge({2.1}, {4.1});
    G2.add_edge({3.1}, {2.1});
    G2.add_edge({4.1}, {0.1}); G2.add_edge({4.1}, {2.1}); G2.add_edge({4.1}, {3.1});

    // Run Robustprune with p = {0.1}, V = {1, 2} (out-neighbors of p), a = 1.2 and R = 2
    set<gIndex> V2 = {1, 2};
    a = 1.2;
    R = 2;
    RobustPrune(G2, {0.1}, V2, a, R);

    // Then we run RobustPrune one more time with p = {4.1}, V = {0, 2, 3}, a=1.2 and R = 1
    V2.clear(); V2 = {0, 2, 3};
    R = 1;
    RobustPrune(G2, {4.1}, V2, a, R);

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G2.exist_edge({0.1}, {1.1}) && !G2.exist_edge({0.1}, {2.1}));
    TEST_ASSERT(G2.exist_edge({1.1}, {2.1}));
    TEST_ASSERT(G2.exist_edge({2.1}, {0.1}) && G2.exist_edge({2.1}, {4.1}));
    TEST_ASSERT(G2.exist_edge({3.1}, {2.1}));
    TEST_ASSERT(!G2.exist_edge({4.1}, {0.1}) && !G2.exist_edge({4.1}, {2.1}) && G2.exist_edge({4.1}, {3.1}));
    



    
    // Testing for 2D float vectors, shortened version of the int test, same data with {x.1,9.9} form

    Graph<vector<float>> G3;
    
    G3.add_vertex({0.1, 9.9}); G3.add_vertex({1.1, 9.9}); G3.add_vertex({2.1, 9.9}); G3.add_vertex({3.1, 9.9}); G3.add_vertex({4.1, 9.9});

    // Add some random edges
    G3.add_edge({0.1, 9.9}, {1.1, 9.9}); G3.add_edge({0.1, 9.9}, {2.1, 9.9});
    G3.add_edge({1.1, 9.9}, {2.1, 9.9});
    G3.add_edge({2.1, 9.9}, {0.1, 9.9}); G3.add_edge({2.1, 9.9}, {4.1, 9.9});
    G3.add_edge({3.1, 9.9}, {2.1, 9.9});
    G3.add_edge({4.1, 9.9}, {0.1, 9.9}); G3.add_edge({4.1, 9.9}, {2.1, 9.9}); G3.add_edge({4.1, 9.9}, {3.1, 9.9});

    // Run Robustprune with p = {0.1,9.9}, V = {{1.1,9.9},{2.1,9.9}} (out-neighbors of p), a = 1.2 and R = 2
    set<gIndex> V3 = {1, 2};
    a = 1.2;
    R = 2;
    RobustPrune(G3, {0.1, 9.9}, V3, a, R);

    // Then we run RobustPrune one more time with p = {4.1,9.9}, V = {{0.1,9.9}, {2.1,9.9}, {3.1,9.9}}, a=1.2 and R = 1
    V3.clear(); V3 = {0, 2, 3};
    R = 1;
    RobustPrune(G3, {4.1, 9.9}, V3, a, R);

    // Now check that the algorithm works as intended (edges from 4 to 0 and from 4 to 2 removed)
    TEST_ASSERT(G3.exist_edge({0.1, 9.9}, {1.1, 9.9}) && !G3.exist_edge({0.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(G3.exist_edge({1.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(G3.exist_edge({2.1, 9.9}, {0.1, 9.9}) && G3.exist_edge({2.1, 9.9}, {4.1, 9.9}));
    TEST_ASSERT(G3.exist_edge({3.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(!G3.exist_edge({4.1, 9.9}, {0.1, 9.9}) && !G3.exist_edge({4.1, 9.9}, {2.1, 9.9}) && G3.exist_edge({4.1, 9.9}, {3.1, 9.9}));


}



/* Testing the FilteredRobustPrune method */
void test_FilteredRobustPrune() {
    
    /* Testing for ints */

    FilterGraph<vector<int>, int> G(true);
    
    // added some random filters
    // Adding vertices 0,1,2,3,4
    G.add_vertex({0},{0}); G.add_vertex({1},{1}); G.add_vertex({2},{1}); G.add_vertex({3},{0}); G.add_vertex({4},{1});

    // Adding some edges 
    G.add_edge({0}, {1}); G.add_edge({0}, {3});
    G.add_edge({1}, {2}); G.add_edge({1}, {4});
    G.add_edge({2}, {3}); G.add_edge({2}, {4});
    G.add_edge({3}, {1}); G.add_edge({3}, {4});
    G.add_edge({4}, {0}); G.add_edge({4}, {1});

    // Run Robustprune with p = {0}, V = {0, 3, 4} (out-neighbors of p), a = 1.2 and R = 2
    set<gIndex> V1 = {0, 3, 4};
    float a = 2;
    int R = 2;

    FilteredRobustPrune(G, {2}, V1, a, R);


    // Now check that the algorithm works as intended (only edge removes is the one from 1 to 3)
    TEST_ASSERT(G.exist_edge({0}, {1}) && G.exist_edge({0}, {3}) && !G.exist_edge({0}, {2}));
    TEST_ASSERT(G.exist_edge({1}, {2}) && G.exist_edge({1}, {4}));
    TEST_ASSERT(G.exist_edge({2}, {0}) && G.exist_edge({2}, {3}) && !G.exist_edge({2}, {4}));
    TEST_ASSERT(G.exist_edge({3}, {1}) && G.exist_edge({3}, {4}));
    TEST_ASSERT(G.exist_edge({4}, {0}) && G.exist_edge({4}, {1}));

    V1.clear();

}

/* Testing the Vamana method */
void test_Vamana() {
    /* Testing the vamana method with a simple example with integers solved by hand */
    Graph<vector<int>> G1;

    // Add vertices 0,1,2,3
    G1.add_vertex({0}); G1.add_vertex({1}); G1.add_vertex({2}); G1.add_vertex({3});

    // Run vamana indexing algorithm with L = 2 and R = 2
    Vamana(G1, 2, 2);

    // Test that the new form of the graph is the one one expected
    // 0->1, 1->0, 1->2, 2->1, 2->3, 3->2 are the exact edges the graph should have
    // G1.print_graph();
    TEST_ASSERT(G1.get_edge_count() >= 6);
    TEST_ASSERT(G1.exist_edge({0}, {1}));
    TEST_ASSERT(G1.exist_edge({1}, {0}));
    TEST_ASSERT(G1.exist_edge({1}, {2}));
    TEST_ASSERT(G1.exist_edge({2}, {1}));
    TEST_ASSERT(G1.exist_edge({2}, {3}));
    TEST_ASSERT(G1.exist_edge({3}, {2}));


    /* Testing the vamana method for 2d float vectors, shortened version of the int test, same data with {x.1,9.9} form*/
    Graph<vector<float>> G2;

    // Add vertices 0,1,2,3
    G2.add_vertex({0.1, 9.9}); G2.add_vertex({1.1, 9.9}); G2.add_vertex({2.1, 9.9}); G2.add_vertex({3.1, 9.9});

    // Run vamana indexing algorithm with L = 2 and R = 2
    Vamana(G2, 2, 2);

    // Test that the new form of the graph is the one one expected 
    // 0->1, 1->0, 1->2, 2->1, 2->3, 3->2 are the exact edges the graph should have
    // G2.print_graph();
    TEST_ASSERT(G2.get_edge_count() >= 6);
    TEST_ASSERT(G2.exist_edge({0.1, 9.9}, {1.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({1.1, 9.9}, {0.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({1.1, 9.9}, {2.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({2.1, 9.9}, {1.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({2.1, 9.9}, {3.1, 9.9}));
    TEST_ASSERT(G2.exist_edge({3.1, 9.9}, {2.1, 9.9}));

}



void test_FilteredVamana() {
    
    /* Testing the vamana method with a simple example with integers solved by hand */
    FilterGraph<vector<int>, int> G;

    // Add vertices 0,1,2,3, 4 with filters 0,1,1,0,1 equivalently
    G.add_vertex({0},{0}); G.add_vertex({1},{1}); G.add_vertex({2},{1}); G.add_vertex({3},{0}); G.add_vertex({4},{1});

    map<int, gIndex> MedoidMap = FindMedoid(G, 4);
    // Run vamana indexing algorithm with L = 2 and R = 2
    FilteredVamana<vector<int>, int>(G, 3, 2, MedoidMap, 2);

    // Check that the only edges that exist are the following: 0->3, 3->0, 1->2, 2->1, 2->4, 4->2
    TEST_ASSERT(G.exist_edge({0},{3})); TEST_ASSERT(G.exist_edge({3},{0}));
    TEST_ASSERT(G.exist_edge({1},{2})); TEST_ASSERT(G.exist_edge({2},{1}));
    TEST_ASSERT(G.exist_edge({2},{4})); TEST_ASSERT(G.exist_edge({4},{2}));
}


void test_Find_Medoid() {

    FilterGraph<vector<int>,int> G;
    
    G.add_vertex({10}, {1});
    G.add_vertex({20}, {1});
    G.add_vertex({30}, {1});
    G.add_vertex({40}, {2});
    G.add_vertex({50}, {2});
    G.add_vertex({60}, {2});
    G.add_vertex({70}, {2});
    G.add_vertex({80}, {3});
    G.add_vertex({81}, {3});
    G.add_vertex({90}, {4});
    G.add_vertex({91}, {4});
    G.add_vertex({100}, {5});
    G.add_vertex({110}, {5});
    G.add_vertex({120}, {5});
    
    int threshold = 2;
    map<int, gIndex> MedoidMap1 = FindMedoid(G, threshold);

    TEST_ASSERT(MedoidMap1.size() == 5);

    for (int i = 1; i <= 5; i++) {

        // print_vector(G.get_filters(MedoidMap[{i}]));
        set<int> Fk = G.get_filters( MedoidMap1[i]);
        TEST_ASSERT(Fk.find(i) != Fk.end());
    }

}




void test_StichedVamana() {
    FilterGraph<vector<float>, int> G;
    
    /* Float vertices of x.x form for no ambiguity regarding which closest of the nearby points is the closest
    The vertices with each filter are 0:{0,1,2}, 1:{0,2,3}, 2:{0,3,4}, 3:{5,6} to make the corresponding
    subgraphs to stich.*/
    G.add_vertex({0.0}, {0, 1, 2});
    G.add_vertex({1.1}, {0});
    G.add_vertex({2.2}, {0, 1});
    G.add_vertex({3.3}, {1, 2});
    G.add_vertex({4.4}, {2});
    G.add_vertex({5.5}, {3});
    G.add_vertex({6.6}, {3});

    StichedVamana<vector<float>, int>(G, 2, 2, 2);
    // G.print_graph();

    TEST_ASSERT(G.get_edge_count() == 11);
    TEST_ASSERT(G.exist_edge({0.0}, {1.1}) && G.exist_edge({0.0}, {2.2}));
    TEST_ASSERT(G.exist_edge({1.1}, {0.0}) && G.exist_edge({1.1}, {2.2}));
    TEST_ASSERT(G.exist_edge({2.2}, {1.1}) && G.exist_edge({2.2}, {3.3}));
    TEST_ASSERT(G.exist_edge({3.3}, {2.2}) && G.exist_edge({3.3}, {4.4}));
    TEST_ASSERT(G.exist_edge({4.4}, {3.3}));
    TEST_ASSERT(G.exist_edge({5.5}, {6.6}));
    TEST_ASSERT(G.exist_edge({6.6}, {5.5}));
}



// List of all tests to be executed
TEST_LIST = {
    {"GreedySearch", test_GreedySearch},
    {"FilteredGreedySearch", test_FilteredGreedySearch},
    {"RobustPrune", test_RobustPrune},
    {"FilteredRobustPrune", test_FilteredRobustPrune},
    {"Vamana", test_Vamana},
    {"Filtered Vamana", test_FilteredVamana},
    {"Find_Medoid", test_Find_Medoid },
    {"StichedVamana", test_StichedVamana},
    { NULL, NULL }
};
