#include "../lib/utils.hpp"			// Unit testing library
#include "../lib/acutest.h"


/* Testing the Euclidean Distance Function */

void test_Euclidean_Distance(){

    /* Testing Integers*/

    vector<int> v1,v2;

    // for( int i = 0 ; i < 3 ; i++){
    //     v1.push_back(i);
    //     v2.push_back(4-i);
    // }

    v1.push_back(0);
    v1.push_back(0);
    v1.push_back(0);

    v2.push_back(0);
    v2.push_back(0);
    v2.push_back(5);


    TEST_ASSERT(Euclidean_Distance(v1,v2) == 5);

    /* Testing Float*/

    vector<float> v3,v4;
}


// List of all tests to be executed
TEST_LIST = {
    { "Euclidean Distance", test_Euclidean_Distance },
    { NULL, NULL }
};
