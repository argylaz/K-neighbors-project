#include "../lib/utils.hpp"			// Unit testing library
#include "../lib/acutest.h"


/* Testing the Euclidean Distance Function */

void test_Euclidean_Distance() {

    /* Testing Integers*/

    vector<int> v1,v2;
    float error = 1.23e-4;

    // Euclidian distances of the examples calculated on an online calculator (https://www.omnicalculator.com/math/euclidean-distance)
    float results[] = {1, 1.414214, 1.73205, 2};

    for(int i = 1 ; i < 8 ; i += 2) {  // Testing for a few different vectors up to 4D
        v1.push_back(i);
        v2.push_back(i+1);
        TEST_ASSERT(abs(Euclidean_Distance(v1,v2) - results[(i-1)/2]) <= error);
    }

    /* Testing Float*/

    vector<float> v3,v4;

    // Euclidian distances of the examples calculated on an online calculator (https://www.omnicalculator.com/math/euclidean-distance)
    float resultsf[] = {1.33, 6.23354, 7.16365, 7.5147};

    v3.push_back(1.23);
    v4.push_back(2.56);
    cout << Euclidean_Distance(v3,v4);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[0]) <= error);

    
    v3.push_back(0.92);
    v4.push_back(7.01);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[1]) <= error);

    
    v3.push_back(5.28);
    v4.push_back(1.75);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[2]) <= error);

    
    v3.push_back(4.76);
    v4.push_back(2.49);
    TEST_ASSERT(abs(Euclidean_Distance(v3,v4) - resultsf[3]) <= error);
}

/* Testing fvec_to_graph */
void test_fvec_to_graph() {

}

/* Testing ivec_to_graph */
void test_ivec_to_graph() {

}


// List of all tests to be executed
TEST_LIST = {
    { "Euclidean Distance", test_Euclidean_Distance },
    { "fvec to graph", test_fvec_to_graph },
    { "ivec to graph", test_ivec_to_graph },
    { NULL, NULL }
};
