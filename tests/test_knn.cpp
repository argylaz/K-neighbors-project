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
    return;
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
