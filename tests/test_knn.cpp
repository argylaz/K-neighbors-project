#include "../lib/acutest.h"
#include "../lib/knn.hpp"
using namespace std;

/* Testing the find_min_Euclidean method */
void test_find_min_Euclidean() {
    // Test the method for vector<int>
    vector<vector<int>> v1 = {
        {1,2},
        {3,4},
        {5,6},
        {7,8},
    };
    set<vector<int>> S1(v1.begin(), v1.end());
    
    vector<vector<int>> v1c = {
        {1,1},
        {3,3},
        {5,5},
        {7,7},
    };

    // Test that the returned element is correct
    vector<int> min1 = find_min_Euclidean<int>(S1, v1c[0]);
    TEST_ASSERT(min1[0] == 1 && min1[1] == 2);
    
    vector<int> min2 = find_min_Euclidean<int>(S1, v1c[1]);
    TEST_ASSERT(min2[0] == 3 && min2[1] == 4);

    vector<int> min3 = find_min_Euclidean<int>(S1, v1c[2]);
    TEST_ASSERT(min3[0] == 5 && min3[1] == 6);
       
    vector<int> min4 = find_min_Euclidean<int>(S1, v1c[3]);
    TEST_ASSERT(min4[0] == 7 && min4[1] == 8);


    // Now do the same for vector<float>
    vector<vector<float>> v2 = {
        {1.1f,2.2f},
        {3.3f,4.4f},
        {5.5f,6.6f},
        {7.7f,8.8f},
    };
    set<vector<float>> S2(v2.begin(), v2.end());

    vector<vector<float>> v2c = {
        {1.1f,1.1f},
        {3.3f,3.3f},
        {5.5f,5.5f},
        {7.7f,7.7f},
    };

    float e = 1.23e-4;

    // Test that the returned element is correct
    vector<float> minf1 = find_min_Euclidean<float>(S2, v2c[0]);
    TEST_ASSERT(minf1[0] - 1.1f <= e && minf1[1] - 2.2 <= e);
    
    vector<float> minf2 = find_min_Euclidean<float>(S2, v2c[1]);
    TEST_ASSERT(minf2[0] - 3.3f <= e && minf2[1] - 4.4f <= e);

    vector<float> minf3 = find_min_Euclidean<float>(S2, v2c[2]);
    TEST_ASSERT(minf3[0] - 5.5f <= e && minf3[1] - 6.6 <= e);

    vector<float> minf4 = find_min_Euclidean<float>(S2, v2c[3]);
    TEST_ASSERT(minf4[0] - 7.7f <= e && minf4[1] - 8.8f <= e); 
}

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

    // cout << "\nL contains:\n";
    // for(auto i : L){
    //     cout << i[0] << endl;
    // }
    
    // cout << endl;

    // cout << "\nV contains:\n";
    // for(auto v : V){
    //     cout << v[0] << endl;
    // }

    // Test that the sets returned contain the correct values (L = {3,5}, V = {1,2,3})
    TEST_ASSERT(L.find({3}) != L.end());
    TEST_ASSERT(L.find({5}) != L.end());

    TEST_ASSERT(V.find({1}) != V.end());
    TEST_ASSERT(V.find({2}) != V.end());
    TEST_ASSERT(V.find({3}) != V.end());
    TEST_ASSERT(V.find({5}) != V.end());
    TEST_ASSERT(L.size() == 2);
    TEST_ASSERT(V.size() == 4);

    result = GreedySearch<vector<int>>(G, {1}, {4}, 2, 1);
    L = result.first;
    V = result.second;
    TEST_ASSERT( L.size() ==  0 );
    TEST_ASSERT( V.size() ==  0 );

}

/* Testing the RobustPrune method */
void test_RobustPrune() {
    return;
}

/* Testing the Vamana method */
void test_Vamana() {
    return;
}

// List of all tests to be executed
TEST_LIST = {
    {"Find_min_Euclidean", test_find_min_Euclidean},
    {"GreedySearch", test_GreedySearch},
    {"RobustPrune", test_RobustPrune},
    {"Vamana", test_Vamana},
    { NULL, NULL }
};
