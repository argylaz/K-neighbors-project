#include "../lib/acutest.h"			// Unit testing library
#include "../lib/filterGraph.hpp"


/* Testing the constructor with .bin file argument (reading data from .bin file)*/
void test_read_from_bin() {
    // Create vector of vectors with data
    vector<vector<float>> vectors = { 
        {1.0f, 0.0f, 0.0f},                   // second entry is always 0 (timestamp that is ignored)
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 2.0f},
        {1.0f, 0.0f, 3.0f},
        {1.0f, 0.0f, 4.0f},
    };

    /* Create a .bin file with the expected format */
    try {
        make_bin("sift/test.bin", vectors);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
    }
    
    FilterGraph<vector<float>, float> G("sift/test.bin", 1);   // num of dimensions of the vectors is 1 (+2 filters)

    // Now test that the graph was created properly
    set<vector<float>> S = G.get_vertices();
    cout << S.size() << G.get_filters(0)[0];
    TEST_ASSERT(S.find({0.0f}) != S.end()); TEST_ASSERT(G.get_filters(0)[0] == 1.0f);  // Test that the entry exist with the correct filter
    TEST_ASSERT(S.find({1.0f}) != S.end()); TEST_ASSERT(G.get_filters(1)[0] == 0.0f);
    TEST_ASSERT(S.find({2.0f}) != S.end()); TEST_ASSERT(G.get_filters(2)[0] == 0.0f);
    TEST_ASSERT(S.find({3.0f}) != S.end()); TEST_ASSERT(G.get_filters(3)[0] == 1.0f);
    TEST_ASSERT(S.find({4.0f}) != S.end()); TEST_ASSERT(G.get_filters(4)[0] == 1.0f);
}

void test_get_filters() {
    
    // First template is Graph data Type, the second tempalet is the Filter data Type 
    FilterGraph<vector<int>,int> G(true);
    TEST_ASSERT( G.add_vertex({10}, {1}) == true );
    TEST_ASSERT( G.add_vertex({20}, {1}) == true );
    TEST_ASSERT( G.add_vertex({30}, {2}) == true );
    TEST_ASSERT( G.add_vertex({40}, {2}) == true );
    
    vector<int> filter;
    filter = G.get_filters(G.get_index_from_vertex({10}));
    // TEST_ASSERT( filter == {1});
    filter = G.get_filters(G.get_index_from_vertex({20}));
    // TEST_ASSERT( filter == {1});
    filter = G.get_filters(G.get_index_from_vertex({30}));
    // TEST_ASSERT( filter == {2});
    filter = G.get_filters(G.get_index_from_vertex({40}));
    // TEST_ASSERT( filter == {2});


};


void test_add_vertex() {

    // First template is Graph data Type, the second tempalet is the Filter data Type 
    FilterGraph<vector<int>,char> G(true);
    TEST_ASSERT( G.add_vertex({10}, {'a'}) == true );
    TEST_ASSERT( G.add_vertex({20}, {'b'}) == true );
    TEST_ASSERT( G.add_vertex({30}, {'c'}) == true );

    TEST_ASSERT(G.get_vertices_count() == 3);

    // Empty filter vector --> return false
    TEST_ASSERT( G.add_vertex({30},{}) == false );


};

void test_get_filters_set(){
    
    // First template is Graph data Type, the second tempalet is the Filter data Type 
    FilterGraph<vector<int>,int> G(true);
    TEST_ASSERT( G.add_vertex({10}, {1}) == true );
    TEST_ASSERT( G.add_vertex({20}, {1}) == true );
    TEST_ASSERT( G.add_vertex({30}, {1}) == true );
    TEST_ASSERT( G.add_vertex({40}, {2}) == true );
    TEST_ASSERT( G.add_vertex({50}, {2}) == true );
    TEST_ASSERT( G.add_vertex({60}, {2}) == true );
    TEST_ASSERT( G.add_vertex({70}, {3}) == true );
    TEST_ASSERT( G.add_vertex({80}, {4}) == true );
    TEST_ASSERT( G.add_vertex({90}, {5}) == true );

    set<vector<int>> filters = G.get_filters_set();
    // The set contains all the discrete values of the filters
    TEST_ASSERT( filters.size() == 5);
    // The set should be filters = {1,2,3,4,5}
    TEST_ASSERT(filters.find({1}) != filters.end());
    TEST_ASSERT(filters.find({2}) != filters.end());
    TEST_ASSERT(filters.find({3}) != filters.end());
    TEST_ASSERT(filters.find({4}) != filters.end());
    TEST_ASSERT(filters.find({5}) != filters.end());
};





// List of all tests to be executed
TEST_LIST = {
    { "read_from_bin", test_read_from_bin },
    { "get_filters", test_get_filters },
    { "add_vertex", test_add_vertex },
    { "get_filters_set", test_get_filters_set },
    { NULL, NULL }
};